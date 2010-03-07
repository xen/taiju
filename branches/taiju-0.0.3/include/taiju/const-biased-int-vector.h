#ifndef TAIJU_CONST_BIASED_INT_VECTOR_H
#define TAIJU_CONST_BIASED_INT_VECTOR_H

#include "const-bit-vector.h"

namespace taiju {

class ConstBiasedIntVector
{
public:
	ConstBiasedIntVector();
	~ConstBiasedIntVector() { clear(); }

	UInt64 operator[](UInt64 id) const;

	bool is_zero(UInt64 id) const;
	UInt64 as_non_zero(UInt64 id) const;

	bool empty() const { return uses_bases_.empty(); }
	UInt64 num_objs() const { return uses_bases_.num_objs(); }
	UInt64 size() const;

	void clear();
	void swap(ConstBiasedIntVector *target);
	const void *map(Mapper mapper);
	void read(Reader reader);
	void write(Writer writer) const;

private:
	ConstBitVector uses_bases_;
	ConstVector<bool> is_ones_;
	ConstVector<UInt8> bases_;
	ConstBitVector uses_extras_;
	ConstVector<UInt32> extras_;

	// Disallows copies.
	ConstBiasedIntVector(const ConstBiasedIntVector &);
	ConstBiasedIntVector &operator=(const ConstBiasedIntVector &);
};

inline UInt64 ConstBiasedIntVector::operator[](UInt64 id) const
{
	return is_zero(id) ? 0 : as_non_zero(id);
}

inline bool ConstBiasedIntVector::is_zero(UInt64 id) const
{
	if (uses_bases_[id])
		return false;
	return !is_ones_[id - uses_bases_.rank(id)];
}

inline UInt64 ConstBiasedIntVector::as_non_zero(UInt64 id) const
{
	if (!uses_bases_[id])
		return 1;

	UInt64 base_id = uses_bases_.rank(id) - 1;
	UInt64 value = bases_[base_id];
	if (uses_extras_[base_id])
	{
		UInt64 extra_id = uses_extras_.rank(base_id) - 1;
		value += BIASED_INT_VECTOR_EXTRA_OFFSET
			+ (static_cast<UInt64>(extras_[extra_id]) << 8);
	}
	else
		value += BIASED_INT_VECTOR_BASE_OFFSET;

	return value;
}

}  // namespace taiju

#endif  // TAIJU_CONST_BIASED_INT_VECTOR_H
