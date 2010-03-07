#ifndef TAIJU_WRITER_H
#define TAIJU_WRITER_H

#include "exception.h"
#include "int-types.h"

#include <iostream>

namespace taiju {

class Writer
{
public:
	Writer(std::ostream *out);
	Writer(const Writer &writer) : out_(writer.out_) {}

	template <typename T>
	void write(const T &obj) { write(&obj, 1); }

	// Disallows writing pointers.
	template <typename T>
	void write(const T *objs);

	template <typename T>
	void write(const T *objs, UInt64 num_objs);

private:
	std::ostream *out_;

	// Disallows operator=().
	Writer &operator=(const Writer &);
};

inline Writer::Writer(std::ostream *out) : out_(out)
{
	if (out == NULL)
		TAIJU_THROW("failed to initialize Writer: the argument is NULL");
}

template <typename T>
void Writer::write(const T *objs, UInt64 num_objs)
{
	if (num_objs > std::numeric_limits<std::size_t>::max() / sizeof(T))
		TAIJU_THROW("failed to write data: too large");

	const char *objs_ptr = reinterpret_cast<const char *>(objs);
	if (!out_->write(objs_ptr, static_cast<std::size_t>(sizeof(T) * num_objs)))
		TAIJU_THROW("failed to write data: the stream has an error");
}

}  // namespace taiju

#endif  // TAIJU_WRITER_H
