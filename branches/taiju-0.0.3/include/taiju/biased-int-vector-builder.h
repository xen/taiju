#ifndef TAIJU_BIASED_INT_VECTOR_BUILDER_H
#define TAIJU_BIASED_INT_VECTOR_BUILDER_H

#include "bit-vector-builder.h"

namespace taiju {

class BiasedIntVectorBuilder
{
public:
	BiasedIntVectorBuilder();
	~BiasedIntVectorBuilder() { clear(); }

	bool empty() const { return uses_bases_.empty(); }
	UInt64 num_objs() const { return uses_bases_.num_objs(); }
	UInt64 size() const;

	bool is_open() const { return uses_bases_.is_open(); }
	bool finished() const { return finished_; }

	void open();
	void close();

	void append(UInt64 obj);
	void finish();

	void clear() { close(); }
	void swap(BiasedIntVectorBuilder *target);
	void write(Writer writer);

private:
	BitVectorBuilder uses_bases_;
	VectorBuilder<bool> is_ones_;
	VectorBuilder<UInt8> bases_;
	BitVectorBuilder uses_extras_;
	VectorBuilder<UInt32> extras_;
	bool finished_;

	// Disallows copies.
	BiasedIntVectorBuilder(const BiasedIntVectorBuilder &);
	BiasedIntVectorBuilder &operator=(const BiasedIntVectorBuilder &);
};

}  // namespace taiju

#endif  // TAIJU_BIASED_INT_VECTOR_BUILDER_H
