#ifndef TAIJU_LINE_READER_H
#define TAIJU_LINE_READER_H

#include <iostream>
#include <vector>

namespace taiju {

class LineReader
{
public:
	LineReader() : stream_(NULL), buf_(), pos_(0), avail_(0) {}
	~LineReader() { clear(); }

	void open(std::istream *stream, std::size_t buf_size = 0);
	void close();

	bool read(const char **line_ptr = NULL, std::size_t *length_ptr = NULL);

	void clear() { close(); }
	void swap(LineReader *target);

private:
	std::istream *stream_;
	std::vector<char> buf_;
	std::size_t pos_;
	std::size_t avail_;

	// Disallows copies.
	LineReader(const LineReader &);
	LineReader &operator=(const LineReader &);

private:
	bool fill();
};

}  // namespace taiju

#endif  // TAIJU_LINE_READER_H
