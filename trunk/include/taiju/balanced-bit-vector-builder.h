#ifndef TAIJU_BALANCED_BIT_VECTOR_BUILDER_H
#define TAIJU_BALANCED_BIT_VECTOR_BUILDER_H

#include "bit-vector-builder.h"

namespace taiju {

class BalancedBitVectorBuilder
{
public:
	BalancedBitVectorBuilder();
	~BalancedBitVectorBuilder() { clear(); }

	bool empty() const { return objs_.empty(); }
	UInt64 num_objs() const { return objs_.num_objs(); }
	UInt64 num_units() const { return objs_.num_units(); }
	UInt64 num_ones() const { return objs_.num_ones(); }
	UInt64 num_zeros() const { return objs_.num_zeros(); }
	UInt64 size() const;

	bool is_open() const { return objs_.is_open(); }
	bool finished() const { return finished_; }

	void open(IndexType index_type = DEFAULT_INDEX_TYPE);
	void close();

	void append(bool bit);
	void finish();

	void clear() { close(); }
	void swap(BalancedBitVectorBuilder *target);
	void write(Writer writer);

private:
	BitVectorBuilder objs_;
	BitVectorBuilder is_fars_;
	BitVectorBuilder is_pioneers_;
	VectorBuilder<UInt32> pioneers_;
	bool finished_;

	// Disallows copies.
	BalancedBitVectorBuilder(const BalancedBitVectorBuilder &);
	BalancedBitVectorBuilder &operator=(const BalancedBitVectorBuilder &);
};

}  // namespace taiju

#endif  // TAIJU_BALANCED_BIT_VECTOR_BUILDER_H
