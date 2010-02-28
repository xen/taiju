#ifndef TAIJU_EXCEPTION_H
#define TAIJU_EXCEPTION_H

#include <cstddef>
#include <exception>

namespace taiju {

class Exception : public std::exception
{
public:
	explicit Exception(const char *msg = NULL) throw() : msg_(msg) {}
	Exception(const Exception &rhs) throw() : msg_(rhs.msg_) {}
	virtual ~Exception() throw() {}

	virtual const char *what() const throw()
	{ return (msg_ != NULL) ? msg_ : ""; }

private:
	const char *msg_;

	// Disallows operator=.
	Exception &operator=(const Exception &);
};

}  // namespace taiju

#define TAIJU_INT_TO_STR(value) #value
#define TAIJU_LINE_TO_STR(line) TAIJU_INT_TO_STR(line)
#define TAIJU_LINE_STR TAIJU_LINE_TO_STR(__LINE__)

#define TAIJU_THROW(msg) throw taiju::Exception( \
	__FILE__ ":" TAIJU_LINE_STR ": throw: " msg)

#ifdef TAIJU_DEBUG
#define TAIJU_ASSERT(cond) do { \
	if (!(cond)) \
	{ \
		throw taiju::Exception( \
			__FILE__ ":" TAIJU_LINE_STR ": assert: " #cond); \
	} \
} while (false)
#else  // TAIJU_DEBUG
#define TAIJU_ASSERT(cond)
#endif  // TAIJU_DEBUG

#endif  // TAIJU_EXCEPTION_H
