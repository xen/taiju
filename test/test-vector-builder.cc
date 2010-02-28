#include <taiju/const-vector.h>
#include <taiju/vector-builder.h>

#include <cassert>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

void test_int_vector_builder()
{
	static const std::size_t NUM_OBJS = (1 << 20) + (std::rand() % 100);

	std::vector<int> objs(NUM_OBJS);
	for (std::size_t i = 0; i < NUM_OBJS; ++i)
		objs[i] = std::rand();

	taiju::VectorBuilder<int> builder;
	builder.open();
	for (std::size_t i = 0; i < objs.size(); ++i)
		builder.append(objs[i]);
	assert(builder.num_objs() == objs.size());

	std::stringstream stream;
	builder.write(&stream);

	std::string str = stream.str();
	assert(str.size() == builder.size());

	taiju::ConstVector<int> vec;

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

void test_bool_vector_builder()
{
	static const std::size_t NUM_OBJS = (1 << 20) + (std::rand() % 100);

	std::vector<bool> objs(NUM_OBJS);
	for (std::size_t i = 0; i < NUM_OBJS; ++i)
		objs[i] = (std::rand() & 1) == 1;

	taiju::VectorBuilder<bool> builder;
	builder.open();
	for (std::size_t i = 0; i < objs.size(); ++i)
		builder.append(objs[i]);
	assert(builder.num_objs() == objs.size());

	std::stringstream stream;
	builder.write(&stream);

	std::string str = stream.str();
	assert(str.size() == builder.size());

	taiju::ConstVector<bool> vec;

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

	std::cerr << "VectorBuilder<int> ";
	test_int_vector_builder();

	std::cerr << "VectorBuilder<bool> ";
	test_bool_vector_builder();

	return 0;
}
