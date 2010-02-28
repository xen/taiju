#include <taiju/file-map.h>

#include <taiju/exception.h>

#if defined _WIN32 || defined _WIN64

#include <sys/types.h>
#include <sys/stat.h>

#include <Windows.h>

#include <utility>

namespace taiju {

FileMap::FileMap() :
	file_handle_(NULL), map_handle_(NULL), address_(NULL), size_(0) {}

void FileMap::open(const char *file_name)
{
	FileMap temp;

	struct __stat64 st;
	if (::_stat64(file_name, &st) != 0)
		TAIJU_THROW("failed to map file: ::_stat64() failed");
	temp.size_ = static_cast<std::size_t>(st.st_size);

	std::printf("%s: %lu\n", file_name, temp.size_);

	temp.file_handle_ = ::CreateFile(file_name, GENERIC_READ, FILE_SHARE_READ,
		NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (temp.file_handle_ == NULL)
		TAIJU_THROW("failed to map file: ::CreateFile() failed");

	temp.map_handle_ = ::CreateFileMapping(temp.file_handle_, NULL,
		PAGE_READONLY, 0, 0, NULL);
	if (temp.map_handle_ == NULL)
		TAIJU_THROW("failed to map file: ::OpenFileMapping() failed");

	temp.address_ = ::MapViewOfFile(temp.map_handle_, FILE_MAP_READ, 0, 0, 0);
	if (temp.address_ == NULL)
		TAIJU_THROW("failed to map file: ::MapViewOfFile() failed");

	swap(&temp);
}

void FileMap::close()
{
	if (address_ != NULL)
	{
		::UnmapViewOfFile(address_);
		address_ = NULL;
	}

	if (map_handle_ != NULL)
	{
		::CloseHandle(map_handle_);
		map_handle_ = NULL;
	}

	if (file_handle_ != NULL)
	{
		::CloseHandle(file_handle_);
		file_handle_ = NULL;
	}

	size_ = 0;
}

void FileMap::swap(FileMap *target)
{
	std::swap(file_handle_, target->file_handle_);
	std::swap(map_handle_, target->map_handle_);
	std::swap(address_, target->address_);
	std::swap(size_, target->size_);
}

}  // namespace taiju

#else  // defined _WIN32 || defined _WIN64

#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>

#include <utility>

namespace taiju {

FileMap::FileMap() : fd_(-1), address_(MAP_FAILED), size_(0) {}

void FileMap::open(const char *file_name)
{
	FileMap temp;

	struct stat st;
	if (::stat(file_name, &st) != 0)
		TAIJU_THROW("failed to map file: ::stat() failed");
	temp.size_ = static_cast<std::size_t>(st.st_size);

	temp.fd_ = ::open(file_name, O_RDONLY);
	if (temp.fd_ == -1)
		TAIJU_THROW("failed to map file: ::open() failed");

	temp.address_ = ::mmap(NULL, temp.size_, PROT_READ,
		MAP_SHARED, temp.fd_, 0);
	if (temp.address_ == MAP_FAILED)
		TAIJU_THROW("failed to map file: ::mmap() failed");

	swap(&temp);
}

void FileMap::close()
{
	if (address_ != MAP_FAILED)
	{
		::munmap(address_, size_);
		address_ = MAP_FAILED;
	}

	if (fd_ != -1)
	{
		::close(fd_);
		fd_ = -1;
	}

	size_ = 0;
}

void FileMap::swap(FileMap *target)
{
	std::swap(fd_, target->fd_);
	std::swap(address_, target->address_);
	std::swap(size_, target->size_);
}

}  // namespace taiju

#endif  // defined _WIN32 || defined _WIN64
