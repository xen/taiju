#ifndef TAIJU_MEMORY_BUF_H
#define TAIJU_MEMORY_BUF_H

#include <streambuf>

namespace taiju {

class MemoryBuf : public std::streambuf
{
public:
	MemoryBuf(void *buf, std::size_t size);

#ifdef _MSC_VER
	std::streamsize _Xsgetn_s(char *s, std::size_t, std::streamsize n);
#else
	std::streamsize xsgetn(char *s, std::streamsize n);
#endif
	std::streamsize xsputn(const char *s, std::streamsize n);

private:
	// Disallows copies.
	MemoryBuf(const MemoryBuf &);
	MemoryBuf &operator=(const MemoryBuf &);
};

}  // namespace taiju

#endif  // TAIJU_MEMORY_BUF_H
