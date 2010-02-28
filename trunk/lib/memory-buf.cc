#include <taiju/memory-buf.h>

#include <cstring>
#include <limits>

namespace taiju {

MemoryBuf::MemoryBuf(void *buf, std::size_t size) : std::streambuf()
{
	char *ptr = static_cast<char *>(buf);
	setg(ptr, ptr, ptr + size);
	setp(ptr, ptr + size);
}

#ifdef _MSC_VER
std::streamsize MemoryBuf::_Xsgetn_s(char *s, std::size_t, std::streamsize n)
#else
std::streamsize MemoryBuf::xsgetn(char *s, std::streamsize n)
#endif
{
	std::streamsize avail = static_cast<std::streamsize>(egptr() - gptr());
	if (n > avail)
		n = avail;
	std::memcpy(s, gptr(), n);
	setg(eback(), gptr() + n, egptr());
	return n;
}

std::streamsize MemoryBuf::xsputn(const char *s, std::streamsize n)
{
	std::streamsize avail = static_cast<std::streamsize>(epptr() - pptr());
	if (n > avail)
		n = avail;
	std::memcpy(pptr(), s, n);

	std::streamsize left = n;
	while (left > 0)
	{
		static const int MAX_INT = std::numeric_limits<int>::max();
		int skip = (left < static_cast<std::streamsize>(MAX_INT))
			? static_cast<int>(left) : MAX_INT;
		pbump(skip);
		left -= skip;
	}
	return n;
}

}  // namespace taiju
