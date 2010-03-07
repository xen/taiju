#ifndef TAIJU_CONST_BIT_VECTOR_H
#define TAIJU_CONST_BIT_VECTOR_H

#include "const-vector.h"

namespace taiju {

class ConstBitVector
{
public:
	ConstBitVector() : num_zeros_(0), objs_(), ranks_(), selects_() {}
	~ConstBitVector() { clear(); }

	bool operator[](UInt64 id) const { return objs_[id]; }
	UInt64 rank(UInt64 id) const;
	UInt64 select(UInt64 id) const;
	Int64 excess(UInt64 id) const { return (2 * rank(id)) - id - 1; }

	const ConstVector<UInt64> &units() const { return objs_.units(); }

	bool empty() const { return objs_.empty(); }
	UInt64 num_objs() const { return objs_.num_objs(); }
	UInt64 num_units() const { return objs_.num_units(); }
	UInt64 num_ones() const { return num_objs() - num_zeros_; }
	UInt64 num_zeros() const { return num_zeros_; }
	UInt64 size() const;

	void clear();
	void swap(ConstBitVector *target);
	const void *map(Mapper mapper);
	void read(Reader reader);
	void write(Writer writer) const;

private:
	UInt64 num_zeros_;
	ConstVector<bool> objs_;
	ConstVector<UInt64> ranks_;
	ConstVector<UInt32> selects_;

	static const UInt8 table_[9][256];

	// Disallows copies.
	ConstBitVector(const ConstBitVector &);
	ConstBitVector &operator=(const ConstBitVector &);

	UInt64 select_block(UInt64 id) const;
	UInt64 select_unit(UInt64 id, UInt64 block_id) const;

	static UInt64 count_ones(UInt64 unit);
};

inline UInt64 ConstBitVector::rank(UInt64 id) const
{
	TAIJU_ASSERT(id < num_objs());

	UInt64 index = ranks_[id / NUM_BITS_PER_BLOCK];
	UInt64 offset = index >> 24;

	UInt64 unit_id = id / NUM_BITS_PER_UNIT;
	offset += ((index << 8) >> ((unit_id % NUM_UNITS_PER_BLOCK) * 8)) & 0xFF;

	UInt64 bit_id = id % NUM_BITS_PER_UNIT;
	UInt64 mask = ~0ULL >> (NUM_BITS_PER_UNIT - bit_id - 1);

	return offset + (count_ones(units()[unit_id] & mask) >> 56);
}

inline UInt64 ConstBitVector::count_ones(UInt64 unit)
{
	unit = ((unit & 0xAAAAAAAAAAAAAAAAULL) >> 1)
		+ (unit & 0x5555555555555555ULL);
	unit = ((unit & 0xCCCCCCCCCCCCCCCCULL) >> 2)
		+ (unit & 0x3333333333333333ULL);
	unit = ((unit >> 4) + unit) & 0x0F0F0F0F0F0F0F0FULL;
	unit += unit << 8;
	unit += unit << 16;
	unit += unit << 32;
	return unit;
}

}  // namespace taiju

#endif  // TAIJU_BIT_VECTOR_H
