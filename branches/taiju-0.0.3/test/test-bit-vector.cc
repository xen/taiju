#include <taiju/const-bit-vector.h>
#include <taiju/bit-vector-builder.h>

#include <cassert>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

void test_bit_vector(const std::vector<bool> &vec,
	taiju::IndexType index_type)
{
	taiju::BitVectorBuilder builder;
	builder.open(index_type);

	for (std::size_t i = 0; i < vec.size(); ++i)
		builder.append(vec[i]);
	assert(builder.num_objs() == vec.size());

	std::stringstream stream;
	builder.write(&stream);

	std::string str = stream.str();
	assert(str.size() == builder.size());

	taiju::ConstBitVector bv;

	bv.map(str.data());
	assert(bv.num_objs() == builder.num_objs());
	assert(bv.size() == builder.size());

	for (std::size_t i = 0; i < vec.size(); ++i)
		assert(bv[i] == vec[i]);

	bv.read(&stream);
	assert(bv.num_objs() == builder.num_objs());
	assert(bv.size() == builder.size());

	for (std::size_t i = 0; i < vec.size(); ++i)
		assert(bv[i] == vec[i]);

	taiju::UInt64 rank = 0;
	taiju::Int64 excess = 0;
	for (taiju::UInt64 i = 0; i < bv.num_objs(); ++i)
	{
		if (bv[i])
		{
			++rank;
			++excess;
		}
		else
		{
			--excess;
			assert(bv.select(i - rank + 1) == i);
		}
		assert(bv.rank(i) == rank);
		assert(bv.excess(i) == excess);
	}

	std::cerr << "ok" << std::endl;
}

int main()
{
	std::srand(static_cast<unsigned int>(std::time(NULL)));

	static const std::size_t ARRAY_SIZE = (1 << 20) + (std::rand() % 100);

	std::vector<bool> vec(ARRAY_SIZE);
	for (std::size_t i = 0; i < ARRAY_SIZE; ++i)
		vec[i] = (std::rand() & 1) == 1;

	std::cerr << "BitVectorBuilder with ONLY_RANK_INDEX: ";
	test_bit_vector(vec, taiju::ONLY_RANK_INDEX);

	std::cerr << "BitVectorBuilder with WITH_SELECT_INDEX: ";
	test_bit_vector(vec, taiju::WITH_SELECT_INDEX);

	return 0;
}
