#ifndef TAIJU_MAPPER_H
#define TAIJU_MAPPER_H

#include "exception.h"
#include "int-types.h"

namespace taiju {

class Mapper
{
public:
	Mapper(const void *address);
	Mapper(const Mapper &mapper) : address_(mapper.address_) {}

	Mapper &operator=(const Mapper &mapper);

	template <typename T>
	const T *map(UInt64 num_objs = 1);

	const void *address() const { return address_; }

private:
	const void *address_;
};

inline Mapper::Mapper(const void *address) : address_(address)
{
	if (address == NULL)
		TAIJU_THROW("failed to initialize Mapper: the argument is NULL");
}

inline Mapper &Mapper::operator=(const Mapper &mapper)
{
	address_ = mapper.address_;
	return *this;
}

template <typename T>
const T *Mapper::map(UInt64 num_objs)
{
	const T *obj_ptr = reinterpret_cast<const T *>(address_);
	address_ = obj_ptr + num_objs;
	return obj_ptr;
}

}  // namespace taiju

#endif  // TAIJU_MAPPER_H
