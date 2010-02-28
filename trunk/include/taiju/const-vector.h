#ifndef TAIJU_CONST_VECTOR_H
#define TAIJU_CONST_VECTOR_H

#include "mapper.h"
#include "reader.h"
#include "vector.h"
#include "writer.h"

namespace taiju {

template <typename T>
class ConstVector
{
public:
	ConstVector() : ptr_(NULL), num_objs_(0), objs_() {}
	~ConstVector() { clear(); }

	const T *begin() const { return ptr_; }
	const T *end() const { return ptr_ + num_objs_; }
	const T &operator[](UInt64 id) const;

	bool empty() const { return num_objs_ == 0; }
	UInt64 num_objs() const { return num_objs_; }
	UInt64 size() const { return sizeof(UInt64) + sizeof(T) * num_objs_; }

	void clear();
	void swap(ConstVector *target);
	const void *map(Mapper mapper);
	void read(Reader reader);
	void write(Writer writer) const;

private:
	const T *ptr_;
	UInt64 num_objs_;
	Vector<T> objs_;

	// Disallows copies.
	ConstVector(const ConstVector &);
	ConstVector &operator=(const ConstVector &);
};

template <typename T>
inline const T &ConstVector<T>::operator[](UInt64 id) const
{
	TAIJU_ASSERT(id < num_objs_);

	return ptr_[id];
}

template <typename T>
void ConstVector<T>::clear()
{
	ptr_ = NULL;
	num_objs_ = 0;
	objs_.clear();
}

template <typename T>
void ConstVector<T>::swap(ConstVector *target)
{
	std::swap(ptr_, target->ptr_);
	std::swap(num_objs_, target->num_objs_);
	objs_.swap(&target->objs_);
}

template <typename T>
const void *ConstVector<T>::map(Mapper mapper)
{
	ConstVector temp;

	temp.num_objs_ = *mapper.map<UInt64>();
	temp.ptr_ = mapper.map<T>(temp.num_objs_);

	swap(&temp);

	return mapper.address();
}

template <typename T>
void ConstVector<T>::read(Reader reader)
{
	ConstVector temp;

	reader.read(&temp.num_objs_);
	if (!temp.objs_.resize(temp.num_objs_))
		TAIJU_THROW("failed to read ConstVector: Vector::resize() failed");

	if (temp.num_objs_ > 0)
	{
		reader.read(&temp.objs_[0], temp.num_objs_);
		temp.ptr_ = &temp.objs_[0];
	}

	swap(&temp);
}

template <typename T>
void ConstVector<T>::write(Writer writer) const
{
	writer.write(num_objs_);
	writer.write(ptr_, num_objs_);
}

}  // namespace taiju

#include "const-bool-vector.h"

#endif  // TAIJU_CONST_VECTOR_H
