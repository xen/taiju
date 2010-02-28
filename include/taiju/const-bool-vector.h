#ifndef TAIJU_CONST_BOOL_VECTOR_H
#define TAIJU_CONST_BOOL_VECTOR_H

#include "const-vector.h"

namespace taiju {

template <>
class ConstVector<bool>
{
public:
	ConstVector() : num_objs_(0), units_() {}
	~ConstVector() { clear(); }

	bool operator[](UInt64 obj_id) const;
	const ConstVector<UInt64> &units() const { return units_; }

	bool empty() const { return num_objs_ == 0; }
	UInt64 num_objs() const { return num_objs_; }
	UInt64 num_units() const { return units_.num_objs(); }
	UInt64 size() const { return sizeof(UInt64) + units_.size(); }

	void clear();
	void swap(ConstVector *target);
	const void *map(Mapper mapper);
	void read(Reader reader);
	void write(Writer writer) const;

private:
	UInt64 num_objs_;
	ConstVector<UInt64> units_;

	// Disallows copies.
	ConstVector(const ConstVector &);
	ConstVector &operator=(const ConstVector &);
};

typedef ConstVector<bool> ConstBoolVector;

inline bool ConstVector<bool>::operator[](UInt64 obj_id) const
{
	TAIJU_ASSERT(obj_id < num_objs_);

	UInt64 unit_id = obj_id / NUM_BITS_PER_UNIT;
	UInt64 bit_id = obj_id % NUM_BITS_PER_UNIT;
	return ((units_[unit_id] >> bit_id) & 1) == 1;
}

}  // namespace taiju

#endif  // TAIJU_CONST_BOOL_VECTOR_H
