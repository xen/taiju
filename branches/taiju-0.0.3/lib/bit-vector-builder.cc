#include <taiju/bit-vector-builder.h>

namespace taiju {

BitVectorBuilder::BitVectorBuilder() : index_type_(DEFAULT_INDEX_TYPE),
	num_zeros_(0), objs_(), ranks_(), selects_(),
	rank_(0xFFFFFFUL), finished_(false) {}

UInt64 BitVectorBuilder::size() const
{
	UInt64 total_size = sizeof(UInt64) + objs_.size()
		+ ranks_.size() + selects_.size();

	if (num_objs() > NUM_BITS_PER_BLOCK * ranks_.num_objs())
		total_size += sizeof(UInt64);

	if (index_type_ == WITH_SELECT_INDEX && num_zeros_ > 0 &&
		((num_zeros_ - 1) / NUM_BITS_PER_BLOCK) + 1 >= selects_.num_objs())
		total_size += sizeof(UInt32);

	return total_size;
}

void BitVectorBuilder::open(IndexType index_type)
{
	BitVectorBuilder temp;

	switch (index_type)
	{
	case WITH_SELECT_INDEX:
	case ONLY_RANK_INDEX:
		temp.index_type_ = index_type;
		break;
	default:
		TAIJU_THROW("failed to open BitVectorBuilder: invalid IndexType");
	}

	temp.objs_.open();
	temp.ranks_.open();
	temp.selects_.open();

	swap(&temp);
}

void BitVectorBuilder::close()
{
	index_type_ = DEFAULT_INDEX_TYPE;
	num_zeros_ = 0;
	objs_.clear();
	ranks_.clear();
	selects_.clear();
	rank_ = 0xFFFFFFUL;
	finished_ = false;
}

void BitVectorBuilder::append(bool obj)
{
	if (finished())
		TAIJU_THROW("failed to append: the builder is finished");

	append_rank(obj);

	if (index_type_ == WITH_SELECT_INDEX)
		append_select(obj);

	objs_.append(obj);

	if (!obj)
		++num_zeros_;
}

void BitVectorBuilder::finish()
{
	finished_ = true;
	objs_.finish();

	if (num_objs() > NUM_BITS_PER_BLOCK * ranks_.num_objs())
		flush_rank();
	ranks_.finish();

	if (index_type_ == WITH_SELECT_INDEX && num_zeros_ > 0 &&
		((num_zeros_ - 1) / NUM_BITS_PER_BLOCK) + 1 >= selects_.num_objs())
		flush_select();
	selects_.finish();
}

void BitVectorBuilder::swap(BitVectorBuilder *target)
{
	std::swap(index_type_, target->index_type_);
	std::swap(num_zeros_, target->num_zeros_);
	objs_.swap(&target->objs_);
	ranks_.swap(&target->ranks_);
	selects_.swap(&target->selects_);
	std::swap(rank_, target->rank_);
	std::swap(finished_, target->finished_);
}

void BitVectorBuilder::write(Writer writer)
{
	finish();

	writer.write(num_zeros_);
	objs_.write(writer);
	ranks_.write(writer);
	selects_.write(writer);
}

void BitVectorBuilder::append_rank(bool obj)
{
	if (num_objs() > NUM_BITS_PER_BLOCK * ranks_.num_objs())
	{
		if (num_objs() % NUM_BITS_PER_BLOCK == 0)
		{
			flush_rank();
			rank_ = (num_ones() << 24) | 0xFFFFFFUL;
		}
		else if (num_objs() % NUM_BITS_PER_UNIT == 0)
		{
			UInt64 diff = num_ones() - (rank_ >> 24);
			UInt64 unit_id = (num_objs() / NUM_BITS_PER_UNIT)
				% NUM_UNITS_PER_BLOCK;
			rank_ &= ~(0xFFULL << ((unit_id - 1) * 8));
			rank_ |= diff << ((unit_id - 1) * 8);
		}
	}
}

void BitVectorBuilder::append_select(bool obj)
{
	if (!obj)
	{
		if (num_zeros_ % NUM_BITS_PER_BLOCK == 0)
		{
			if (num_zeros_ / NUM_BITS_PER_BLOCK >= selects_.num_objs())
				flush_select();
		}
	}
}

void BitVectorBuilder::flush_select()
{
	selects_.append(static_cast<UInt32>(num_objs() / NUM_BITS_PER_BLOCK));
}

}  // namespace taiju
