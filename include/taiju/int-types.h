#ifndef TAIJU_INT_TYPES_H
#define TAIJU_INT_TYPES_H

#include <cstddef>
#include <limits>

namespace taiju {

typedef signed char Int8;
typedef signed short Int16;
typedef signed int Int32;
typedef signed long long Int64;

typedef unsigned char UInt8;
typedef unsigned short UInt16;
typedef unsigned int UInt32;
typedef unsigned long long UInt64;

enum TrieType
{
	PODS_TRIE,
	LOB_TRIE,
	LOUDS_TRIE,
	PLOUDS_TRIE,
	BP_TRIE,
	DFUDS_TRIE,
	DEFAULT_TRIE_TYPE = PODS_TRIE
};

enum IndexType
{
	ONLY_RANK_INDEX,
	WITH_SELECT_INDEX,
	DEFAULT_INDEX_TYPE = ONLY_RANK_INDEX
};

enum NodeOrder
{
	ASCENDING_LABEL_ORDER,
	DESCENDING_LABEL_ORDER,
	TOTAL_WEIGHT_ORDER,
	MAX_WEIGHT_ORDER,
	RANDOM_ORDER,
	LABEL_ORDER = ASCENDING_LABEL_ORDER,
	DEFAULT_NODE_ORDER = LABEL_ORDER
};

enum
{
	NUM_BITS_PER_UNIT = 64,
	NUM_BITS_PER_BLOCK = 256,
	NUM_UNITS_PER_BLOCK = NUM_BITS_PER_BLOCK / NUM_BITS_PER_UNIT
};

enum
{
	BIASED_INT_VECTOR_BASE_OFFSET = 2,
	BIASED_INT_VECTOR_EXTRA_OFFSET = BIASED_INT_VECTOR_BASE_OFFSET + 256
};

enum
{
	TEMP_FILE_DEFAULT_BUF_SIZE = 1 << 20,
	INT_ARRAY_BUILDER_BUF_SIZE = 1 << 22,
	VECTOR_BUILDER_BUF_SIZE = 1 << 22,
	LINE_READER_DEFAULT_BUF_SIZE = 1 << 12
};

}  // namespace taiju

#endif  // TAIJU_INT_TYPES_H
