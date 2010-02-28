#ifndef TAIJU_CONST_BALANCED_BIT_VECTOR_H
#define TAIJU_CONST_BALANCED_BIT_VECTOR_H

#include "const-bit-vector.h"

namespace taiju {

class ConstBalancedBitVector
{
public:
	ConstBalancedBitVector() : objs_(), is_fars_(), is_pioneers_(), pioneers_() {}
	~ConstBalancedBitVector() { clear(); }

	bool operator[](UInt64 id) const { return objs_[id]; }
	UInt64 rank(UInt64 id) const { return objs_.rank(id); }
	UInt64 select(UInt64 id) const { return objs_.select(id); }
	Int64 excess(UInt64 id) const { return objs_.excess(id); }
	UInt64 findclose(UInt64 id) const;

	const ConstVector<UInt64> &units() const { return objs_.units(); }

	bool empty() const { return objs_.empty(); }
	UInt64 num_objs() const { return objs_.num_objs(); }
	UInt64 num_units() const { return objs_.num_units(); }
	UInt64 num_ones() const { return objs_.num_ones(); }
	UInt64 num_zeros() const { return objs_.num_zeros(); }
	UInt64 size() const;

	void clear();
	void swap(ConstBalancedBitVector *target);
	const void *map(Mapper mapper);
	void read(Reader reader);
	void write(Writer writer) const;

private:
	ConstBitVector objs_;
	ConstBitVector is_fars_;
	ConstBitVector is_pioneers_;
	ConstVector<UInt32> pioneers_;

	static const UInt8 table_[8][256];

	// Disallows copies.
	ConstBalancedBitVector(const ConstBalancedBitVector &);
	ConstBalancedBitVector &operator=(const ConstBalancedBitVector &);

	static UInt64 lookup(UInt64 unit, UInt64 excess);
};

}  // namespace taiju

#endif  // TAIJU_CONST_BALANCED_BIT_VECTOR_H
