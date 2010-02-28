#ifndef TAIJU_BOOL_VECTOR_H
#define TAIJU_BOOL_VECTOR_H

#include "vector.h"

namespace taiju {

template <>
class Vector<bool>
{
private:
	class Reference
	{
	public:
		Reference(Vector *vec, UInt64 obj_id) : vec_(vec), obj_id_(obj_id) {}

		operator bool() const;
		Reference &operator=(bool obj);

	private:
		Vector *vec_;
		UInt64 obj_id_;

		// Copyable.
	};

public:
	Vector() : units_(), num_objs_(0) {}
	~Vector() { clear(); }

	bool front() const;
	bool back() const;
	bool operator[](UInt64 obj_id) const;
	const Vector<UInt64> &units() const { return units_; }

	Reference front();
	Reference back();
	Reference operator[](UInt64 obj_id);
	Vector<UInt64> &units() { return units_; }

	bool empty() const { return num_objs_ == 0; }
	UInt64 num_objs() const { return num_objs_; }
	UInt64 num_units() const { return units_.num_objs(); }
	UInt64 size() const { return units_.size(); }

	bool resize(UInt64 num_objs);
	bool resize(UInt64 num_objs, bool obj);

	bool reserve(UInt64 num_objs);

	bool append();
	bool append(bool bit);

	bool push(bool bit) { return append(bit); }
	void pop();

	void clear();
	void swap(Vector *target);

private:
	Vector<UInt64> units_;
	UInt64 num_objs_;

	// Disallows copies.
	Vector(const Vector &);
	Vector &operator=(const Vector &);
};

typedef Vector<bool> BoolVector;

inline Vector<bool>::Reference::operator bool() const
{
	TAIJU_ASSERT(obj_id_ < vec_->num_objs());

	return (*static_cast<const Vector *>(vec_))[obj_id_];
}

inline Vector<bool>::Reference &Vector<bool>::Reference::operator=(bool obj)
{
	TAIJU_ASSERT(obj_id_ < vec_->num_objs());

	UInt64 unit_id = obj_id_ / NUM_BITS_PER_UNIT;
	UInt64 bit_id = obj_id_ % NUM_BITS_PER_UNIT;
	if (obj)
		vec_->units_[unit_id] |= 1ULL << bit_id;
	else
		vec_->units_[unit_id] &= ~(1ULL << bit_id);
	return *this;
}

inline bool Vector<bool>::front() const
{
	TAIJU_ASSERT(num_objs_ > 0);

	return units_.front() & 1ULL;
}

inline bool Vector<bool>::back() const
{
	TAIJU_ASSERT(num_objs_ > 0);

	return (*this)[num_objs_ - 1];
}

inline bool Vector<bool>::operator[](UInt64 obj_id) const
{
	TAIJU_ASSERT(obj_id < num_objs_);

	UInt64 unit_id = obj_id / NUM_BITS_PER_UNIT;
	UInt64 bit_id = obj_id % NUM_BITS_PER_UNIT;
	return ((units_[unit_id] >> bit_id) & 1) == 1;
}

inline Vector<bool>::Reference Vector<bool>::front()
{
	TAIJU_ASSERT(num_objs_ > 0);

	return Reference(this, 0);
}

inline Vector<bool>::Reference Vector<bool>::back()
{
	TAIJU_ASSERT(num_objs_ > 0);

	return Reference(this, num_objs_ - 1);
}

inline Vector<bool>::Reference Vector<bool>::operator[](UInt64 obj_id)
{
	TAIJU_ASSERT(obj_id < num_objs_);

	return Reference(this, obj_id);
}

inline bool Vector<bool>::resize(UInt64 num_objs)
{
	UInt64 num_units = (num_objs + NUM_BITS_PER_UNIT - 1) / NUM_BITS_PER_UNIT;
	if (!units_.resize(num_units))
		return false;
	num_objs_ = num_objs;
	return true;
}

inline bool Vector<bool>::resize(UInt64 num_objs, bool obj)
{
	UInt64 unit_id = num_objs_ / NUM_BITS_PER_UNIT;
	UInt64 bit_id = num_objs_ % NUM_BITS_PER_UNIT;
	if (bit_id > 0)
	{
		if (obj)
			units_[unit_id] |= ~0ULL << bit_id;
		else
			units_[unit_id] &= ~(~0ULL << bit_id);
	}

	UInt64 num_units = (num_objs + NUM_BITS_PER_UNIT - 1) / NUM_BITS_PER_UNIT;
	if (!units_.resize(num_units, obj ? ~0ULL : 0ULL))
		return false;
	num_objs_ = num_objs;
	return true;
}

inline bool Vector<bool>::reserve(UInt64 num_objs)
{
	UInt64 num_units = (num_objs + NUM_BITS_PER_UNIT - 1) / NUM_BITS_PER_UNIT;
	return units_.reserve(num_units);
}

inline bool Vector<bool>::append()
{
	if (num_objs_ == units_.num_objs() * NUM_BITS_PER_UNIT)
	{
		if (!units_.append())
			return false;
	}
	++num_objs_;
	return true;
}

inline bool Vector<bool>::append(bool obj)
{
	if (num_objs_ == units_.num_objs() * NUM_BITS_PER_UNIT)
	{
		if (!units_.append())
			return false;
	}

	UInt64 unit_id = num_objs_ / NUM_BITS_PER_UNIT;
	UInt64 bit_id = num_objs_ % NUM_BITS_PER_UNIT;
	if (obj)
		units_[unit_id] |= 1ULL << bit_id;
	else
		units_[unit_id] &= ~(1ULL << bit_id);
	++num_objs_;
	return true;
}

inline void Vector<bool>::pop()
{
	TAIJU_ASSERT(num_objs_ > 0);

	--num_objs_;
	if (num_objs_ == (units_.num_objs() - 1) * NUM_BITS_PER_UNIT)
		units_.pop();
}

inline void Vector<bool>::clear()
{
	units_.clear();
	num_objs_ = 0;
}

inline void Vector<bool>::swap(Vector *target)
{
	units_.swap(&target->units_);
	std::swap(num_objs_, target->num_objs_);
}

}  // namespace taiju

#endif  // TAIJU_BOOL_VECTOR_H
