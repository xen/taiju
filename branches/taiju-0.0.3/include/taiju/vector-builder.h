#ifndef TAIJU_VECTOR_BUILDER_H
#define TAIJU_VECTOR_BUILDER_H

#include "temp-file.h"
#include "vector.h"
#include "writer.h"

namespace taiju {

template <typename T>
class VectorBuilder
{
public:
	VectorBuilder() : num_objs_(0), file_(), finished_(false) {}
	~VectorBuilder() { clear(); }

	bool empty() const { return num_objs_ == 0; }
	UInt64 num_objs() const { return num_objs_; }
	UInt64 size() const { return sizeof(UInt64) + sizeof(T) * num_objs_; }

	bool is_open() const { return file_.is_open(); }
	bool finished() const { return finished_; }

	void open(UInt64 buf_size = 0);
	void close();

	void append(const T &obj);
	void finish() { finished_ = true; }

	void clear() { close(); }
	void swap(VectorBuilder *target);
	void write(Writer writer);

private:
	UInt64 num_objs_;
	TempFile file_;
	bool finished_;

	// Disallows copies.
	VectorBuilder(const VectorBuilder &);
	VectorBuilder &operator=(const VectorBuilder &);

	void flush();
};

template <typename T>
void VectorBuilder<T>::open(UInt64 buf_size)
{
	VectorBuilder temp;

	temp.file_.open(buf_size);

	swap(&temp);
}

template <typename T>
void VectorBuilder<T>::close()
{
	num_objs_ = 0;
	file_.clear();
	finished_ = false;
}

template <typename T>
void VectorBuilder<T>::append(const T &obj)
{
	if (finished())
		TAIJU_THROW("failed to push: the builder is finished");

	file_.write(obj);
	++num_objs_;
}

template <typename T>
void VectorBuilder<T>::swap(VectorBuilder *target)
{
	std::swap(num_objs_, target->num_objs_);
	file_.swap(&target->file_);
}

template <typename T>
void VectorBuilder<T>::write(Writer writer)
{
	finish();

	Vector<char> buf;
	if (!buf.resize(VECTOR_BUILDER_BUF_SIZE))
		TAIJU_THROW("failed to write IntArray: Vector::resize() failed");

	writer.write(num_objs_);

	file_.seek();
	UInt64 size_left = sizeof(T) * num_objs();
	while (size_left > 0)
	{
		UInt64 size_avail = VECTOR_BUILDER_BUF_SIZE;
		if (size_avail > size_left)
			size_avail = size_left;

		file_.read(&buf[0], size_avail);
		writer.write(&buf[0], size_avail);

		size_left -= size_avail;
	}
}

}  // namespace taiju

#include "bool-vector-builder.h"

#endif  // TAIJU_VECTOR_BUILDER_H
