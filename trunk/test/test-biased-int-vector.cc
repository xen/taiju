#include <taiju/const-biased-int-vector.h>
#include <taiju/biased-int-vector-builder.h>

#include <cassert>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

void TestBiasedIntVector(const std::vector<taiju::UInt64> &objs)
{
	taiju::BiasedIntVectorBuilder builder;
	builder.open();

	for (std::size_t i = 0; i < objs.size(); ++i)
		builder.append(objs[i]);
	assert(builder.num_objs() == objs.size());

	double num_bits_per_value = 1.0 * builder.size() / builder.num_objs();
	std::cerr << num_bits_per_value << " bits/value: ";

	assert(builder.size() > sizeof(taiju::UInt64) * objs.size() / 8);
	assert(builder.size() < sizeof(taiju::UInt64) * objs.size() / 7);

	std::stringstream stream;
	builder.write(&stream);

	std::string str = stream.str();
	assert(str.size() == builder.size());

	taiju::ConstBiasedIntVector vec;

	vec.map(str.data());
	assert(vec.num_objs() == builder.num_objs());
	assert(vec.size() == builder.size());

	for (std::size_t i = 0; i < objs.size(); ++i)
		assert(vec[i] == objs[i]);

	vec.read(&stream);
	assert(vec.num_objs() == builder.num_objs());
	assert(vec.size() == builder.size());

	for (std::size_t i = 0; i < objs.size(); ++i)
		assert(vec[i] == objs[i]);

	std::cerr << "ok" << std::endl;
}

int main()
{
	std::srand(static_cast<unsigned int>(std::time(NULL)));

	static const std::size_t ARRAY_SIZE = (1 << 20) + (std::rand() % 100);

	std::vector<taiju::UInt64> objs(ARRAY_SIZE);
	for (std::size_t i = 0; i < ARRAY_SIZE; ++i)
	{
		double rate = 1.0 * std::rand() / RAND_MAX;
		if (rate < 0.3)
			objs[i] = 0;
		else if (rate < 0.6)
			objs[i] = 1;
		else if (rate < 0.9)
			objs[i] = taiju::BIASED_INT_VECTOR_BASE_OFFSET
				+ (std::rand() % 256);
		else
		{
			objs[i] = taiju::BIASED_INT_VECTOR_EXTRA_OFFSET
				+ ((static_cast<taiju::UInt64>(RAND_MAX) + 1) * std::rand()
					+ std::rand()) % (1ULL << 40);
		}
	}

	std::cerr << "BiasedIntVectorBuilder: ";
	TestBiasedIntVector(objs);

	return 0;
}
