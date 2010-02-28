#include <taiju/const-bool-vector.h>

namespace taiju {

void ConstVector<bool>::clear()
{
	num_objs_ = 0;
	units_.clear();
}

void ConstVector<bool>::swap(ConstVector<bool> *target)
{
	std::swap(num_objs_, target->num_objs_);
	units_.swap(&target->units_);
}

const void *ConstVector<bool>::map(Mapper mapper)
{
	ConstVector temp;

	temp.num_objs_ = *mapper.map<UInt64>();
	mapper = temp.units_.map(mapper);

	swap(&temp);

	return mapper.address();
}

void ConstVector<bool>::read(Reader reader)
{
	ConstVector temp;

	reader.read(&temp.num_objs_);
	temp.units_.read(reader);

	swap(&temp);
}

void ConstVector<bool>::write(Writer writer) const
{
	writer.write(num_objs_);
	units_.write(writer);
}

}  // namespace taiju
