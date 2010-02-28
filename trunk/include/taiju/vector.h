#ifndef TAIJU_VECTOR_H
#define TAIJU_VECTOR_H

#include "exception.h"
#include "int-types.h"

#include <vector>

namespace taiju {

template <typename T>
class Vector
{
public:
	typedef typename std::vector<T>::iterator Iterator;
	typedef typename std::vector<T>::const_iterator ConstIterator;

	Vector() : buf_() {}
	~Vector() { clear(); }

	ConstIterator begin() const { return buf_.begin(); }
	ConstIterator end() const { return buf_.end(); }
	const T &front() const;
	const T &back() const;
	const T &operator[](UInt64 id) const;

	Iterator begin() { return buf_.begin(); }
	Iterator end() { return buf_.end(); }
	T &front();
	T &back();
	T &operator[](UInt64 id);

	bool empty() const { return buf_.empty(); }
	UInt64 num_objs() const { return buf_.size(); }
	UInt64 size() const { return sizeof(T) * buf_.size(); }

	bool resize(UInt64 num_objs);
	bool resize(UInt64 num_objs, const T &obj);

	bool reserve(UInt64 num_objs);

	bool append();
	bool append(const T &obj);

	bool push(const T &obj) { return append(obj); }
	void pop();

	void clear() { std::vector<T>(0).swap(buf_); }
	void swap(Vector *target) { buf_.swap(target->buf_); }

private:
	std::vector<T> buf_;

	// Disallows copies.
	Vector(const Vector &);
	Vector &operator=(const Vector &);
};

template <typename T>
inline const T &Vector<T>::front() const
{
	TAIJU_ASSERT(!buf_.empty());

	return buf_.front();
}

template <typename T>
inline const T &Vector<T>::back() const
{
	TAIJU_ASSERT(!buf_.empty());

	return buf_.back();
}

template <typename T>
inline const T &Vector<T>::operator[](UInt64 id) const
{
	TAIJU_ASSERT(id < buf_.size());

	return buf_[static_cast<std::size_t>(id)];
}

template <typename T>
inline T &Vector<T>::front()
{
	TAIJU_ASSERT(!buf_.empty());

	return buf_.front();
}

template <typename T>
inline T &Vector<T>::back()
{
	TAIJU_ASSERT(!buf_.empty());

	return buf_.back();
}

template <typename T>
inline T &Vector<T>::operator[](UInt64 id)
{
	TAIJU_ASSERT(id < buf_.size());

	return buf_[static_cast<std::size_t>(id)];
}

template <typename T>
inline bool Vector<T>::resize(UInt64 num_objs)
{
	if (num_objs > std::numeric_limits<std::size_t>::max())
		return false;

	try
	{
		buf_.resize(static_cast<std::size_t>(num_objs));
		return true;
	}
	catch (...)
	{
		return false;
	}
}

template <typename T>
inline bool Vector<T>::resize(UInt64 num_objs, const T &obj)
{
	if (num_objs > std::numeric_limits<std::size_t>::max())
		return false;

	try
	{
		buf_.resize(static_cast<std::size_t>(num_objs), obj);
		return true;
	}
	catch (...)
	{
		return false;
	}
}

template <typename T>
inline bool Vector<T>::reserve(UInt64 num_objs)
{
	if (num_objs > std::numeric_limits<std::size_t>::max())
		return false;

	try
	{
		buf_.reserve(static_cast<std::size_t>(num_objs));
		return true;
	}
	catch (...)
	{
		return false;
	}
}

template <typename T>
inline bool Vector<T>::append()
{
	if (buf_.size() >= std::numeric_limits<std::size_t>::max())
		return false;

	try
	{
		buf_.resize(buf_.size() + 1);
		return true;
	}
	catch (...)
	{
		return false;
	}
}

template <typename T>
inline bool Vector<T>::append(const T &obj)
{
	if (buf_.size() >= std::numeric_limits<std::size_t>::max())
		return false;

	try
	{
		buf_.push_back(obj);
		return true;
	}
	catch (...)
	{
		return false;
	}
}

template <typename T>
inline void Vector<T>::pop()
{
	TAIJU_ASSERT(!buf_.empty());

	buf_.pop_back();
}

}  // namespace taiju

#include "bool-vector.h"

#endif  // TAIJU_VECTOR_H
