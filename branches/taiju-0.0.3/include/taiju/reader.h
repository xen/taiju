#ifndef TAIJU_READER_H
#define TAIJU_READER_H

#include "exception.h"
#include "int-types.h"

#include <iostream>

namespace taiju {

class Reader
{
public:
	Reader(std::istream *in);
	Reader(const Reader &reader) : in_(reader.in_) {}

	template <typename T>
	void read(T *objs, UInt64 num_objs = 1);

private:
	std::istream *in_;

	// Disallows operator=().
	Reader &operator=(const Reader &);
};

inline Reader::Reader(std::istream *in) : in_(in)
{
	if (in == NULL)
		TAIJU_THROW("failed to initialize Reader: the argument is NULL");
}

template <typename T>
void Reader::read(T *objs, UInt64 num_objs)
{
	if (num_objs > std::numeric_limits<std::size_t>::max() / sizeof(T))
		TAIJU_THROW("failed to read data: too large");

	char *objs_ptr = reinterpret_cast<char *>(objs);
	if (!in_->read(objs_ptr, static_cast<std::size_t>(sizeof(T) * num_objs)))
		TAIJU_THROW("failed to read data: the stream has an error");
}

}  // namespace taiju

#endif  // TAIJU_READER_H
