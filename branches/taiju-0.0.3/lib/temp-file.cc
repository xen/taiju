#include <taiju/temp-file.h>

#include <taiju/exception.h>

#include <cstdlib>
#include <utility>

namespace taiju {

void TempFile::open(UInt64 buf_size)
{
	if (buf_size > std::numeric_limits<std::size_t>::max())
		TAIJU_THROW("failed to open TempFile: too large buffer size");
	else if (buf_size == 0)
		buf_size = TEMP_FILE_DEFAULT_BUF_SIZE;

	TempFile temp;

#if defined _WIN32 || defined _WIN64
	temp.file_name_ = ::_tempnam(NULL, "taiju");
	if (temp.file_name_ == NULL)
		TAIJU_THROW("failed to open TempFile: ::_tempnam() failed");
#ifdef _MSC_VER
	if (::fopen_s(&temp.file_, temp.file_name_, "wb+") != 0)
		TAIJU_THROW("failed to open TempFile: ::fopen_s() failed");
#else  // _MSC_VER
	temp.file_ = std::fopen(temp.file_name_, "wb+");
	if (temp.file_ == NULL)
		TAIJU_THROW("failed to open TempFile: std::fopen() failed");
#endif  // _MSC_VER

#else  // defined _WIN32 || defined _WIN64
	temp.file_ = std::tmpfile();
	if (temp.file_ == NULL)
		TAIJU_THROW("failed to open TempFile: std::tmpfile() failed");
#endif  // defined _WIN32 || defined _WIN64

	if (std::setvbuf(temp.file_, NULL, _IOFBF,
		static_cast<std::size_t>(buf_size)) != 0)
		TAIJU_THROW("failed to open TempFile: std::setvbuf() failed");

	swap(&temp);
}

void TempFile::close()
{
	if (is_open())
	{
		std::fclose(file_);
		file_ = NULL;

#if defined _WIN32 || defined _WIN64
		if (file_name_ != NULL)
			std::remove(file_name_);
#endif
	}

#if defined _WIN32 || defined _WIN64
	if (file_name_ != NULL)
	{
		std::free(file_name_);
		file_name_ = NULL;
	}
#endif
}

void TempFile::read(void *buf, UInt64 size)
{
	if (size > std::numeric_limits<std::size_t>::max())
		TAIJU_THROW("failed to read data: too large");

	if (size == 0)
		return;

	if (std::fread(buf, static_cast<std::size_t>(size), 1, file_) != 1)
		TAIJU_THROW("failed to read data: std::fread() failed");
}

void TempFile::write(const void *buf, UInt64 size)
{
	if (size > std::numeric_limits<std::size_t>::max())
		TAIJU_THROW("failed to write data: too large");

	if (size == 0)
		return;

	if ( std::fwrite(buf, static_cast<std::size_t>(size), 1, file_) != 1)
		TAIJU_THROW("failed to write data: std::fwrite() failed");
}

void TempFile::flush()
{
	if (std::fflush(file_) != 0)
		TAIJU_THROW("failed to flush file: std::fflush() failed");
}

UInt64 TempFile::tell()
{
	long result = std::ftell(file_);
	if (result == -1)
		TAIJU_THROW("failed to get file position: std::ftell() failed");
	return static_cast<UInt64>(result);
}

void TempFile::seek(Int64 offset, int whence)
{
	if (offset < std::numeric_limits<long>::min() ||
		offset > std::numeric_limits<long>::max())
		TAIJU_THROW("failed to seek file position: out of range");

	if (std::fseek(file_, static_cast<long>(offset), whence) != 0)
		TAIJU_THROW("failed to set file position: std::fseek() failed");
}

void TempFile::swap(TempFile *target)
{
	std::swap(file_, target->file_);
#if defined _WIN32 || defined _WIN64
	std::swap(file_name_, target->file_name_);
#endif
}

}  // namespace taiju
