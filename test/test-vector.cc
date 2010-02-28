#include <taiju/vector.h>

#include <cassert>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <vector>

void test_int_vector()
{
	static const std::size_t NUM_OBJS = (1 << 20) + (std::rand() % 100);

	std::vector<int> objs(NUM_OBJS);
	for (std::size_t i = 0; i < NUM_OBJS; ++i)
		objs[i] = (std::rand() & 1) == 1;

	taiju::Vector<int> vec;
	assert(vec.empty());

	for (std::size_t i = 0; i < objs.size(); ++i)
	{
		vec.push(objs[i]);
		assert(!vec.empty());
		assert(vec.num_objs() == i + 1);
		assert(vec.size() == sizeof(int) * vec.num_objs());
		assert(vec.front() == objs[0]);
		assert(vec.back() == objs[i]);
	}

	for (std::size_t i = 0; i < objs.size(); ++i)
	{
		assert(vec[i] == objs[i]);
		assert(*(vec.begin() + i) == objs[i]);
		assert(*(vec.end() - static_cast<std::size_t>(
			vec.num_objs()) + i) == objs[i]);
	}

	const taiju::Vector<int> &const_vec = vec;
	for (std::size_t i = 0; i < objs.size(); ++i)
	{
		assert(const_vec[i] == objs[i]);
		assert(*(const_vec.begin() + i) == objs[i]);
		assert(*(const_vec.end() - static_cast<std::size_t>(
			const_vec.num_objs()) + i) == objs[i]);
	}

	vec.append();
	assert(vec.num_objs() == NUM_OBJS + 1);

	vec.pop();
	assert(vec.num_objs() == NUM_OBJS);

	vec.clear();
	assert(vec.empty());

	vec.resize(100, 200);
	assert(vec.num_objs() == 100);
	for (taiju::UInt64 i = 0; i < vec.num_objs(); ++i)
		assert(vec[i] == 200);

	vec.resize(1000);
	assert(vec.num_objs() == 1000);

	std::cerr << "ok" << std::endl;
}

void test_bool_vector()
{
	static const std::size_t NUM_OBJS = (1 << 20) + (std::rand() % 100);

	std::vector<bool> objs(NUM_OBJS);
	for (std::size_t i = 0; i < NUM_OBJS; ++i)
		objs[i] = (std::rand() & 1) == 1;

	taiju::Vector<bool> vec;
	assert(vec.empty());

	for (std::size_t i = 0; i < objs.size(); ++i)
	{
		vec.push(objs[i]);
		assert(!vec.empty());
		assert(vec.num_objs() == i + 1);
		assert(vec.num_units() == (vec.num_objs() + 63) / 64);
		assert(vec.size() == 8 * ((vec.num_objs() + 63) / 64));
		assert(vec.front() == objs[0]);
		assert(vec.back() == objs[i]);
	}

	for (std::size_t i = 0; i < objs.size(); ++i)
		assert(vec[i] == objs[i]);

	const taiju::Vector<bool> &const_vec = vec;
	for (std::size_t i = 0; i < objs.size(); ++i)
		assert(const_vec[i] == objs[i]);

	vec.append();
	assert(vec.num_objs() == NUM_OBJS + 1);

	vec.pop();
	assert(vec.num_objs() == NUM_OBJS);

	vec.clear();
	assert(vec.empty());

	vec.resize(100, true);
	assert(vec.num_objs() == 100);
	for (taiju::UInt64 i = 0; i < vec.num_objs(); ++i)
		assert(vec[i] == true);

	vec.resize(200, false);
	assert(vec.num_objs() == 200);
	for (taiju::UInt64 i = 0; i < 100; ++i)
		assert(vec[i] == true);
	for (taiju::UInt64 i = 100; i < vec.num_objs(); ++i)
		assert(vec[i] == false);

	vec.resize(1000);
	assert(vec.num_objs() == 1000);

	std::cerr << "ok" << std::endl;
}

int main()
{
	std::srand(static_cast<unsigned int>(std::time(NULL)));

	std::cerr << "Vector<int>: ";
	test_int_vector();

	std::cerr << "Vector<bool>: ";
	test_bool_vector();

	return 0;
}
