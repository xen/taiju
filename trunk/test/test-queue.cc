#include <taiju/queue-file.h>

#include <cassert>
#include <iostream>

void test_without_swap()
{
	static const taiju::UInt64 QUEUE_SIZE = 1 << 20;

	taiju::QueueFile queue;
	queue.open();

	for (taiju::UInt64 i = 0; i < QUEUE_SIZE; ++i)
		queue.write(i);

	for (taiju::UInt64 i = 0; i < QUEUE_SIZE; ++i)
	{
		taiju::UInt64 value;
		assert(queue.read(&value));
		assert(value == i);
	}

	std::cerr << "ok" << std::endl;
}

void test_with_swap()
{
	static const taiju::UInt64 MAX_VALUE = 1 << 20;

	taiju::QueueFile queue;
	queue.open();

	taiju::UInt64 value = 0;
	queue.write(value);
	while (queue.read(&value))
	{
		assert(value <= MAX_VALUE);

		value *= 2;
		if (++value <= MAX_VALUE)
		{
			queue.write(value);
			if (++value <= MAX_VALUE)
				queue.write(value);
		}
	}

	std::cerr << "ok" << std::endl;
}

int main()
{
	std::cerr << "QueueFile without swap: ";
	test_without_swap();

	std::cerr << "QueueFile with swap: ";
	test_with_swap();

	return 0;
}
