#include <taiju/balanced-bit-vector-builder.h>

#include <taiju/const-bit-vector.h>
#include <taiju/memory-buf.h>

namespace taiju {
namespace {

void restore_bit_vector(BitVectorBuilder *builder,
	ConstBitVector *objs, Vector<char> *buf)
{
	if (!buf->resize(builder->size()))
		TAIJU_THROW("failed to restore BitVector: Vector::resize() failed");

	MemoryBuf stream_buf(&(*buf)[0],
		static_cast<std::size_t>(buf->num_objs()));
	std::iostream stream(&stream_buf);

	builder->write(&stream);
	objs->map(&(*buf)[0]);
}

void build_is_fars(const ConstBitVector &const_objs,
	BitVectorBuilder *builder)
{
	Vector<bool> is_fars;
	if (!is_fars.resize(const_objs.num_ones()))
		TAIJU_THROW("failed to build is_fars: Vector::resize() failed");

	Vector<UInt64> stack;
	for (UInt64 i = 0; i < const_objs.num_objs(); ++i)
	{
		if (const_objs[i])
		{
			if (!stack.push(i))
				TAIJU_THROW("failed to build is_fars: Vector::push() failed");
		}
		else if (!stack.empty())
		{
			UInt64 id = stack.back();
			UInt64 is_far_id = const_objs.rank(id) - 1;
			is_fars[is_far_id] = (i - id) > NUM_BITS_PER_UNIT;
			stack.pop();
		}
	}
	for (UInt64 i = builder->num_objs(); i < is_fars.num_objs(); ++i)
		builder->append(is_fars[i]);
	builder->finish();
}

void build_fars(const ConstBitVector &const_objs,
	const ConstBitVector &is_fars, Vector<UInt64> *fars)
{
	if (!fars->resize(is_fars.num_ones(), 0))
		TAIJU_THROW("failed to build fars: Vector::resize() failed");

	Vector<UInt64> stack;
	for (UInt64 i = 0; i < const_objs.num_objs(); ++i)
	{
		if (const_objs[i])
		{
			if (!stack.push(i))
				TAIJU_THROW("failed to build fars: Vector::push() failed");
		}
		else if (!stack.empty())
		{
			UInt64 id = stack.back();
			UInt64 is_far_id = const_objs.rank(id) - 1;
			if (is_fars[is_far_id])
			{
				UInt64 far_id = is_fars.rank(is_far_id) - 1;
				(*fars)[far_id] = i;
			}
			stack.pop();
		}
	}
}

void build_is_pioneers(const Vector<UInt64> &fars,
	BitVectorBuilder *is_pioneers, VectorBuilder<UInt32> *pioneers)
{
	UInt32 prev_far = 0;
	UInt64 pioneer_id = 0;
	for (std::size_t i = 0; i < fars.num_objs(); ++i)
	{
		UInt32 far = static_cast<UInt32>(fars[i] / NUM_BITS_PER_UNIT);

		bool is_pioneer = (prev_far != far);
		is_pioneers->append(is_pioneer);
		if (is_pioneer)
		{
			if (pioneers->num_objs() <= pioneer_id)
				pioneers->append(far);
			++pioneer_id;
		}

		prev_far = far;
	}
	is_pioneers->finish();
	pioneers->finish();
}

}  // namespace

BalancedBitVectorBuilder::BalancedBitVectorBuilder() : objs_(), is_fars_(),
	is_pioneers_(), pioneers_(), finished_(false) {}

UInt64 BalancedBitVectorBuilder::size() const
{
	return objs_.size() + is_fars_.size() + is_pioneers_.size()
		+ pioneers_.size();
}

void BalancedBitVectorBuilder::open(IndexType index_type)
{
	BalancedBitVectorBuilder temp;

	temp.objs_.open(index_type);
	temp.is_fars_.open(ONLY_RANK_INDEX);
	temp.is_pioneers_.open(ONLY_RANK_INDEX);
	temp.pioneers_.open();

	swap(&temp);
}

void BalancedBitVectorBuilder::close()
{
	objs_.clear();
	is_fars_.clear();
	is_pioneers_.clear();
	pioneers_.clear();
	finished_ = false;
}

void BalancedBitVectorBuilder::append(bool bit)
{
	if (finished())
		TAIJU_THROW("failed to append: the builder is finished");

	objs_.append(bit);
}

void BalancedBitVectorBuilder::finish()
{
	finished_ = true;
	objs_.finish();

	if (is_pioneers_.finished())
		return;

	ConstBitVector const_objs;
	Vector<char> const_objs_buf;
	restore_bit_vector(&objs_, &const_objs, &const_objs_buf);

	if (!is_fars_.finished())
		build_is_fars(const_objs, &is_fars_);

	ConstBitVector is_fars;
	Vector<char> is_fars_buf;
	restore_bit_vector(&is_fars_, &is_fars, &is_fars_buf);

	Vector<UInt64> fars;
	build_fars(const_objs, is_fars, &fars);

	build_is_pioneers(fars, &is_pioneers_, &pioneers_);
}

void BalancedBitVectorBuilder::swap(BalancedBitVectorBuilder *target)
{
	objs_.swap(&target->objs_);
	is_fars_.swap(&target->is_fars_);
	is_pioneers_.swap(&target->is_pioneers_);
	pioneers_.swap(&target->pioneers_);
	std::swap(finished_, target->finished_);
}

void BalancedBitVectorBuilder::write(Writer writer)
{
	finish();

	objs_.write(writer);
	is_fars_.write(writer);
	is_pioneers_.write(writer);
	pioneers_.write(writer);
}

}  // namespace taiju
