#include <taiju/const-biased-int-vector.h>

namespace taiju {

ConstBiasedIntVector::ConstBiasedIntVector()
	: uses_bases_(), is_ones_(), bases_(), uses_extras_(), extras_() {}

UInt64 ConstBiasedIntVector::size() const
{
	return uses_bases_.size() + is_ones_.size() + bases_.size()
		+ uses_extras_.size() + extras_.size();
}

void ConstBiasedIntVector::clear()
{
	uses_bases_.clear();
	is_ones_.clear();
	bases_.clear();
	uses_extras_.clear();
	extras_.clear();
}

void ConstBiasedIntVector::swap(ConstBiasedIntVector *target)
{
	uses_bases_.swap(&target->uses_bases_);
	is_ones_.swap(&target->is_ones_);
	bases_.swap(&target->bases_);
	uses_extras_.swap(&target->uses_extras_);
	extras_.swap(&target->extras_);
}

const void *ConstBiasedIntVector::map(Mapper mapper)
{
	ConstBiasedIntVector temp;

	mapper = temp.uses_bases_.map(mapper);
	mapper = temp.is_ones_.map(mapper);
	mapper = temp.bases_.map(mapper);
	mapper = temp.uses_extras_.map(mapper);
	mapper = temp.extras_.map(mapper);

	swap(&temp);

	return mapper.address();
}

void ConstBiasedIntVector::read(Reader reader)
{
	ConstBiasedIntVector temp;

	temp.uses_bases_.read(reader);
	temp.is_ones_.read(reader);
	temp.bases_.read(reader);
	temp.uses_extras_.read(reader);
	temp.extras_.read(reader);

	swap(&temp);
}

void ConstBiasedIntVector::write(Writer writer) const
{
	uses_bases_.write(writer);
	is_ones_.write(writer);
	bases_.write(writer);
	uses_extras_.write(writer);
	extras_.write(writer);
}

}  // namespace taiju
