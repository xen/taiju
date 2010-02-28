#ifndef TAIJU_TEMP_FILE_H
#define TAIJU_TEMP_FILE_H

#include "int-types.h"

#include <cstdio>

namespace taiju {

class TempFile
{
public:
#if defined _WIN32 || defined _WIN64
	TempFile() : file_(NULL), file_name_(NULL) {}
#else
	TempFile() : file_(NULL) {}
#endif
	~TempFile() { clear(); }

	bool is_open() const { return file_ != NULL; }

	void open(UInt64 buf_size = 0);
	void close();

	void read(void *buf, UInt64 size = 1);
	template <typename T>
	void read(T *buf, UInt64 size = 1)
	{ read(static_cast<void *>(buf), sizeof(T) * size); }

	void write(const void *buf, UInt64 size);
	template <typename T>
	void write(const T *buf, UInt64 size)
	{ write(static_cast<const void *>(buf), sizeof(T) * size); }

	template <typename T>
	void write(const T &buf) { write(&buf, 1); }

	// Disallows writing a pointer.
	template <typename T>
	void write(const T *buf);

	void flush();

	UInt64 tell();
	void seek(Int64 offset = 0, int whence = SEEK_SET);

	void clear() { close(); }
	void swap(TempFile *target);

private:
	std::FILE *file_;
#if defined _WIN32 || defined _WIN64
	char *file_name_;
#endif

	// Disallows copies.
	TempFile(const TempFile &);
	TempFile &operator=(const TempFile &);
};

}  // namespace taiju

#endif  // TAIJU_TEMP_FILE_H
