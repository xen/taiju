#ifndef TAIJU_BIT_VECTOR_BUILDER_H
#define TAIJU_BIT_VECTOR_BUILDER_H

#include "vector-builder.h"

namespace taiju {

class BitVectorBuilder
{
public:
	BitVectorBuilder();
	~BitVectorBuilder() { clear(); }

	bool empty() const { return objs_.empty(); }
	UInt64 num_objs() const { return objs_.num_objs(); }
	UInt64 num_units() const { return objs_.num_units(); }
	UInt64 num_ones() const { return num_objs() - num_zeros_; }
	UInt64 num_zeros() const { return num_zeros_; }
	UInt64 size() const;

	bool is_open() const { return objs_.is_open(); }
	bool finished() const { return finished_; }

	void open(IndexType index_type = DEFAULT_INDEX_TYPE);
	void close();

	void append(bool obj);
	void finish();

	void clear() { close(); }
	void swap(BitVectorBuilder *target);
	void write(Writer writer);

private:
	IndexType index_type_;
	UInt64 num_zeros_;
	VectorBuilder<bool> objs_;
	VectorBuilder<UInt64> ranks_;
	VectorBuilder<UInt32> selects_;
	UInt64 rank_;
	bool finished_;

	// Disallows copies.
	BitVectorBuilder(const BitVectorBuilder &);
	BitVectorBuilder &operator=(const BitVectorBuilder &);

	void append_rank(bool obj);
	void append_select(bool obj);

	void flush_rank() { ranks_.append(rank_); }
	void flush_select();
};

}  // namespace taiju

#endif  // TAIJU_BIT_VECTOR_BUILDER_H
