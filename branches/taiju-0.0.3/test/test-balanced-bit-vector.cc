#include <taiju/const-balanced-bit-vector.h>
#include <taiju/balanced-bit-vector-builder.h>

#include <cassert>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <stack>
#include <sstream>
#include <string>
#include <vector>

void test_balanced_bit_vector(const std::vector<bool> &objs,
	taiju::IndexType index_type)
{
	taiju::BalancedBitVectorBuilder builder;
	builder.open(index_type);

	for (std::size_t i = 0; i < objs.size(); ++i)
		builder.append(objs[i]);
	assert(builder.num_objs() == objs.size());

	std::stringstream stream;
	builder.write(&stream);

	std::string str = stream.str();
	assert(str.size() == builder.size());

	taiju::ConstBalancedBitVector vec;

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

	taiju::UInt64 rank = 0;
	for (taiju::UInt64 i = 0; i < vec.num_objs(); ++i)
	{
		if (vec[i])
			++rank;
		else
			assert(vec.select(i - rank + 1) == i);
		assert(vec.rank(i) == rank);
	}

	std::stack<taiju::UInt64> stack;
	for (taiju::UInt64 i = 0; i < vec.num_objs(); ++i)
	{
		if (vec[i])
			stack.push(i);
		else if (!stack.empty())
		{
			taiju::UInt64 from = stack.top();
			taiju::UInt64 closer = vec.findclose(from);
			assert(closer == i);
			stack.pop();
		}
	}

	std::cerr << "ok" << std::endl;
}

int main()
{
	std::srand(static_cast<unsigned int>(std::time(NULL)));

	static const std::size_t ARRAY_SIZE = (1 << 20) + (std::rand() % 100);

	std::vector<bool> objs(ARRAY_SIZE);
	for (std::size_t i = 0; i < ARRAY_SIZE; ++i)
		objs[i] = (std::rand() & 1) == 1;

	std::cerr << "BalancedBitVectorBuilder with ONLY_RANK_INDEX: ";
	test_balanced_bit_vector(objs, taiju::ONLY_RANK_INDEX);

	std::cerr << "BalancedBitVectorBuilder with WITH_SELECT_INDEX: ";
	test_balanced_bit_vector(objs, taiju::WITH_SELECT_INDEX);

	return 0;
}
