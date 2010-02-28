#ifndef TAIJU_QUEUE_FILE_H
#define TAIJU_QUEUE_FILE_H

#include "temp-file.h"

namespace taiju {

class QueueFile
{
public:
	QueueFile() : input_(), output_(), avail_(0) {}
	~QueueFile() { clear(); }

	bool is_open() const { return input_.is_open(); }

	void open(UInt64 buf_size = 0);
	void close();

	bool read(void *buf, UInt64 size = 1);
	template <typename T>
	bool read(T *buf, UInt64 size = 1)
	{ return read(static_cast<void *>(buf), sizeof(T) * size); }

	void write(const void *buf, UInt64 size);
	template <typename T>
	void write(const T *buf, UInt64 size)
	{ write(static_cast<const void *>(buf), sizeof(T) * size); }

	template <typename T>
	void write(const T &buf) { write(&buf, 1); }

	// Disallows writing a pointer.
	template <typename T>
	void write(const T *buf);

	void clear() { close(); }
	void swap(QueueFile *target);

private:
	TempFile input_;
	TempFile output_;
	UInt64 avail_;

	// Disallows copies.
	QueueFile(const QueueFile &);
	QueueFile &operator=(const QueueFile &);
};

}  // namespace taiju

#endif  // TAIJU_QUEUE_FILE_H
