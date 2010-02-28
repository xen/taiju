#include <taiju/int-types.h>

#include <cassert>
#include <iostream>

int main()
{
	std::cerr << "signed integers: ";

	assert(sizeof(taiju::Int8) == 1);
	assert(sizeof(taiju::Int16) == 2);
	assert(sizeof(taiju::Int32) == 4);
	assert(sizeof(taiju::Int64) == 8);

	assert(static_cast<taiju::Int8>(-1) < 0);
	assert(static_cast<taiju::Int16>(-1) < 0);
	assert(static_cast<taiju::Int32>(-1) < 0);
	assert(static_cast<taiju::Int64>(-1) < 0);

	std::cerr << "ok" << std::endl;

	std::cerr << "unsigned integers: ";

	assert(sizeof(taiju::UInt8) == 1);
	assert(sizeof(taiju::UInt16) == 2);
	assert(sizeof(taiju::UInt32) == 4);
	assert(sizeof(taiju::UInt64) == 8);

	assert(static_cast<taiju::UInt8>(-1) > 0);
	assert(static_cast<taiju::UInt16>(-1) > 0);
	assert(static_cast<taiju::UInt32>(-1) > 0);
	assert(static_cast<taiju::UInt64>(-1) > 0);

	std::cerr << "ok" << std::endl;

	return 0;
}
