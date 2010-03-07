#include <taiju/queue-file.h>

#include <taiju/exception.h>

#include <utility>

namespace taiju {

void QueueFile::open(UInt64 buf_size)
{
	QueueFile temp;

	temp.input_.open(buf_size);
	temp.output_.open(buf_size);

	swap(&temp);
}

void QueueFile::close()
{
	input_.clear();
	output_.clear();
	avail_ = 0;
}

bool QueueFile::read(void *buf, UInt64 size)
{
	if (avail_ == 0)
	{
		UInt64 output_size = output_.tell();
		input_.seek();
		output_.seek();
		input_.swap(&output_);
		avail_ = output_size;
	}

	if (size > avail_)
		return false;

	input_.read(buf, size);
	avail_ -= size;
	return true;
}

void QueueFile::write(const void *buf, UInt64 size)
{
	output_.write(buf, size);
}

void QueueFile::swap(QueueFile *target)
{
	input_.swap(&target->input_);
	output_.swap(&target->output_);
	std::swap(avail_, target->avail_);
}

}  // namespace taiju
