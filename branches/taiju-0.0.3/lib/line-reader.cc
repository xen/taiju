#include <taiju/line-reader.h>

#include <taiju/exception.h>
#include <taiju/int-types.h>

#include <cstring>

namespace taiju {

void LineReader::open(std::istream *stream, std::size_t buf_size)
{
	if (stream == NULL)
		TAIJU_THROW("failed to open LineReader: the stream is NULL");

	if (buf_size == 0)
		buf_size = LINE_READER_DEFAULT_BUF_SIZE;

	LineReader temp;

	temp.stream_ = stream;
	try
	{
		temp.buf_.resize(buf_size);
	}
	catch (const std::bad_alloc &)
	{
		TAIJU_THROW("failed to open LineReader: std::bad_alloc");
	}

	swap(&temp);
}

void LineReader::close()
{
	stream_ = NULL;
	buf_.clear();
	pos_ = 0;
	avail_ = 0;
}

bool LineReader::read(const char **line_ptr, std::size_t *length_ptr)
{
	std::size_t pos = pos_;
	while (pos < avail_ && buf_[pos] != '\n')
		++pos;
	if (pos == avail_)
	{
		pos -= pos_;
		if (!fill())
			return false;

		while (pos < avail_ && buf_[pos] != '\n')
			++pos;

		if (pos == avail_)
		{
			if (avail_ == buf_.size())
				TAIJU_THROW("failed to read line: too long line");
			else
				TAIJU_THROW("failed to read line: missing end of line");
		}
	}

	if (line_ptr != NULL)
		*line_ptr = &buf_[pos_];
	if (length_ptr != NULL)
		*length_ptr = pos - pos_;
	buf_[pos] = '\0';
	pos_ = pos + 1;
	return true;
}

void LineReader::swap(LineReader *target)
{
	std::swap(stream_, target->stream_);
	buf_.swap(target->buf_);
	std::swap(pos_, target->pos_);
	std::swap(avail_, target->avail_);
}

bool LineReader::fill()
{
	if (!*stream_)
		return false;

	std::memmove(&buf_[0], &buf_[pos_], avail_ - pos_);

	avail_ -= pos_;
	pos_ = 0;

	stream_->read(&buf_[avail_], buf_.size() - avail_);
	avail_ += stream_->gcount();
	return true;
}

}  // namespace taiju
