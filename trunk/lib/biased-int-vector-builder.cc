#include <taiju/biased-int-vector-builder.h>

namespace taiju {

BiasedIntVectorBuilder::BiasedIntVectorBuilder() : uses_bases_(), is_ones_(),
	bases_(), uses_extras_(), extras_(), finished_(false) {}

UInt64 BiasedIntVectorBuilder::size() const
{
	return uses_bases_.size() + is_ones_.size() + bases_.size()
		+ uses_extras_.size() + extras_.size();
}

void BiasedIntVectorBuilder::open()
{
	BiasedIntVectorBuilder temp;

	temp.uses_bases_.open(ONLY_RANK_INDEX);
	temp.is_ones_.open();
	temp.bases_.open();
	temp.uses_extras_.open(ONLY_RANK_INDEX);
	temp.extras_.open();

	swap(&temp);
}

void BiasedIntVectorBuilder::close()
{
	uses_bases_.clear();
	is_ones_.clear();
	bases_.clear();
	uses_extras_.clear();
	extras_.clear();
	finished_ = false;
}

void BiasedIntVectorBuilder::append(UInt64 obj)
{
	if (finished())
		TAIJU_THROW("failed to append: the builder is finished");

	bool uses_base = (obj >= BIASED_INT_VECTOR_BASE_OFFSET);
	uses_bases_.append(uses_base);

	if (!uses_base)
		is_ones_.append(obj == 1);
	else
	{
		bool uses_extra = (obj >= BIASED_INT_VECTOR_EXTRA_OFFSET);
		uses_extras_.append(uses_extra);

		if (uses_extra)
		{
			obj -= BIASED_INT_VECTOR_EXTRA_OFFSET;
			extras_.append(static_cast<UInt32>(obj >> 8));
		}
		else
			obj -= BIASED_INT_VECTOR_BASE_OFFSET;
		bases_.append(static_cast<UInt8>(obj));
	}
}

void BiasedIntVectorBuilder::finish()
{
	finished_ = true;

	uses_bases_.finish();
	is_ones_.finish();
	bases_.finish();
	uses_extras_.finish();
	extras_.finish();
}

void BiasedIntVectorBuilder::swap(BiasedIntVectorBuilder *target)
{
	uses_bases_.swap(&target->uses_bases_);
	is_ones_.swap(&target->is_ones_);
	bases_.swap(&target->bases_);
	uses_extras_.swap(&target->uses_extras_);
	extras_.swap(&target->extras_);
	std::swap(finished_, target->finished_);
}

void BiasedIntVectorBuilder::write(Writer writer)
{
	finish();

	uses_bases_.write(writer);
	is_ones_.write(writer);
	bases_.write(writer);
	uses_extras_.write(writer);
	extras_.write(writer);
}

}  // namespace taiju
