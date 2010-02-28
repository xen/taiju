#include <taiju/bool-vector-builder.h>

namespace taiju {

UInt64 VectorBuilder<bool>::num_units() const
{
	return (num_objs_ + NUM_BITS_PER_UNIT - 1) / NUM_BITS_PER_UNIT;
}

UInt64 VectorBuilder<bool>::size() const
{
	return sizeof(UInt64) * (2 + num_units());
}

void VectorBuilder<bool>::open(UInt64 buf_size)
{
	VectorBuilder temp;

	temp.units_.open(buf_size);

	swap(&temp);
}

void VectorBuilder<bool>::close()
{
	num_objs_ = 0;
	units_.clear();
	unit_ = 0;
	finished_ = false;
}

void VectorBuilder<bool>::append(bool obj)
{
	if (finished())
		TAIJU_THROW("failed to append: the builder is finished");

	if (num_objs_ > 0 && (num_objs_ % NUM_BITS_PER_UNIT) == 0)
		flush();

	if (obj)
		unit_ |= 1ULL << (num_objs_ % NUM_BITS_PER_UNIT);
	++num_objs_;
}

void VectorBuilder<bool>::finish()
{
	finished_ = true;
	if (num_objs_ > NUM_BITS_PER_UNIT * units_.num_objs())
		flush();
	units_.finish();
}

void VectorBuilder<bool>::swap(VectorBuilder<bool> *target)
{
	std::swap(num_objs_, target->num_objs_);
	units_.swap(&target->units_);
	std::swap(unit_, target->unit_);
	std::swap(finished_, target->finished_);
}

void VectorBuilder<bool>::write(Writer writer)
{
	finish();

	writer.write(num_objs_);
	units_.write(writer);
}

void VectorBuilder<bool>::flush()
{
	units_.append(unit_);
	unit_ = 0;
}

}  // namespace taiju
