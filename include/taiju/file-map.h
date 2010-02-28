#ifndef TAIJU_FILE_MAP_H
#define TAIJU_FILE_MAP_H

#include <cstddef>

namespace taiju {

class FileMap
{
public:
	FileMap();
	~FileMap() { clear(); }

	void open(const char *file_name);
	void close();

	template <typename T>
	const T *pointer() const { return static_cast<const T *>(address_); }
	const void *address() const { return address_; }
	std::size_t size() const { return size_; }

	void clear() { close(); }
	void swap(FileMap *target);

private:
#if defined _WIN32 || defined _WIN64
	void *file_handle_;
	void *map_handle_;
#else
	int fd_;
#endif
	void *address_;
	std::size_t size_;

	// Disallows copies.
	FileMap(const FileMap &);
	FileMap &operator=(const FileMap &);
};

}  // namespace taiju

#endif  // TAIJU_FILE_MAP_H
