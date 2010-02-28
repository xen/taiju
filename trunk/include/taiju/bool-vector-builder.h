#ifndef TAIJU_BOOL_VECTOR_BUILDER_H
#define TAIJU_BOOL_VECTOR_BUILDER_H

#include "vector-builder.h"

namespace taiju {

template <>
class VectorBuilder<bool>
{
public:
	VectorBuilder() : num_objs_(0), units_(), unit_(0), finished_(false) {}
	~VectorBuilder() { clear(); }

	bool empty() const { return num_objs_ == 0; }
	UInt64 num_objs() const { return num_objs_; }
	UInt64 num_units() const;
	UInt64 size() const;

	bool is_open() const { return units_.is_open(); }
	bool finished() const { return finished_; }

	void open(UInt64 buf_size = 0);
	void close();

	void append(bool obj);
	void finish();

	void clear() { close(); }
	void swap(VectorBuilder *target);
	void write(Writer writer);

private:
	UInt64 num_objs_;
	VectorBuilder<UInt64> units_;
	UInt64 unit_;
	bool finished_;

	// Disallows copies.
	VectorBuilder(const VectorBuilder &);
	VectorBuilder &operator=(const VectorBuilder &);

	void flush();
};

typedef VectorBuilder<bool> BoolVectorBuilder;

}  // namespace taiju

#endif  // TAIJU_BOOL_VECTOR_BUILDER_H
