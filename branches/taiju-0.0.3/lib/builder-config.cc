#include <taiju/builder-config.h>

#include <taiju/exception.h>

#ifdef _MSC_VER
#include <stdlib.h>
#else
#include <cstdlib>
#endif

#include <cstring>
#include <utility>

namespace taiju {

BuilderConfig::BuilderConfig() : trie_type_(DEFAULT_TRIE_TYPE),
	index_type_(DEFAULT_INDEX_TYPE), node_order_(DEFAULT_NODE_ORDER) {}

void BuilderConfig::parse(int argc, char **argv, int *argc_ptr)
{
	int arg_id = 1;
	for (int i = 1; i < argc; ++i)
	{
		if (std::strncmp(argv[i], "--taiju-", 8) != 0)
		{
			argv[arg_id++] = argv[i];
			continue;
		}

		if (i == argc - 1)
			TAIJU_THROW("failed to parse option: no value");

		const char *key = argv[i] + 8;
		const char *value = argv[++i];

		if (std::strcmp(key, "trie-type") == 0)
			parse_trie_type(value);
		else if (std::strcmp(key, "index-type") == 0)
			parse_index_type(value);
		else if (std::strcmp(key, "node-order") == 0)
			parse_node_order(value);
		else
			TAIJU_THROW("failed to parse option: wrong option");
	}

	if (argc_ptr != NULL)
		*argc_ptr = arg_id;
}

void BuilderConfig::clear()
{
	trie_type_ = DEFAULT_TRIE_TYPE;
	index_type_ = DEFAULT_INDEX_TYPE;
	node_order_ = DEFAULT_NODE_ORDER;
}

void BuilderConfig::swap(BuilderConfig *target)
{
	std::swap(trie_type_, target->trie_type_);
	std::swap(index_type_, target->index_type_);
	std::swap(node_order_, target->node_order_);
}

void BuilderConfig::parse_trie_type(const char *value)
{
	if (std::strcmp(value, "PODS_TRIE") == 0)
		trie_type_ = PODS_TRIE;
	else if (std::strcmp(value, "LOB_TRIE") == 0)
		trie_type_ = LOB_TRIE;
	else if (std::strcmp(value, "LOUDS_TRIE") == 0)
		trie_type_ = LOUDS_TRIE;
	else if (std::strcmp(value, "PLOUDS_TRIE") == 0)
		trie_type_ = PLOUDS_TRIE;
	else if (std::strcmp(value, "BP_TRIE") == 0)
		trie_type_ = BP_TRIE;
	else if (std::strcmp(value, "DFUDS_TRIE") == 0)
		trie_type_ = DFUDS_TRIE;
	else if (std::strcmp(value, "DEFAULT") == 0)
		trie_type_ = DEFAULT_TRIE_TYPE;
	else
		TAIJU_THROW("failed to parse TrieType: wrong value");
}

void BuilderConfig::parse_index_type(const char *value)
{
	if (std::strcmp(value, "ONLY_RANK_INDEX") == 0)
		index_type_ = ONLY_RANK_INDEX;
	else if (std::strcmp(value, "WITH_SELECT_INDEX") == 0)
		index_type_ = WITH_SELECT_INDEX;
	else if (std::strcmp(value, "DEFAULT") == 0)
		index_type_ = DEFAULT_INDEX_TYPE;
	else
		TAIJU_THROW("failed to parse IndexType: wrong value");
}

void BuilderConfig::parse_node_order(const char *value)
{
	if (std::strcmp(value, "ASCENDING_LABEL_ORDER") == 0)
		node_order_ = ASCENDING_LABEL_ORDER;
	else if (std::strcmp(value, "DESCENDING_LABEL_ORDER") == 0)
		node_order_ = DESCENDING_LABEL_ORDER;
	else if (std::strcmp(value, "TOTAL_WEIGHT_ORDER") == 0)
		node_order_ = TOTAL_WEIGHT_ORDER;
	else if (std::strcmp(value, "MAX_WEIGHT_ORDER") == 0)
		node_order_ = MAX_WEIGHT_ORDER;
	else if (std::strcmp(value, "RANDOM_ORDER") == 0)
		node_order_ = RANDOM_ORDER;
	else if (std::strcmp(value, "LABEL_ORDER") == 0)
		node_order_ = LABEL_ORDER;
	else if (std::strcmp(value, "DEFAULT") == 0)
		node_order_ = DEFAULT_NODE_ORDER;
	else
		TAIJU_THROW("failed to parse NodeOrder: wrong value");
}

const char *BuilderConfig::help()
{
	return "Options:\n"
	"  --taiju-trie-type arg  choose one of the following trie types\n"
	"      PODS_TRIE    Post-Order Difference Sequence\n"
	"      LOB_TRIE     Level-Order Bitmap\n"
	"      LOUDS_TRIE   Level-Order Unary Degree Sequence\n"
	"      PLOUDS_TRIE  Partitioned representation of LOUDS\n"
	"      BP_TRIE      Balanced Parentheses\n"
	"      DFUDS_TRIE   Depth-First Unary Degree Sequence\n"
	"      DEFAULT      PODS_TRIE\n"
	"\n"
	"  --taiju-index-type arg  index type of BitVector\n"
	"      ONLY_RANK_INDEX    use index only for rank()\n"
	"      WITH_SELECT_INDEX  use extra index for accelerating select()\n"
	"      DEFAULT            ONLY_RANK_INDEX\n"
	"\n"
	"  --taiju-node-order arg  arrangement of nodes\n"
	"      ASCENDING_LABEL_ORDER   'a' - 'z'\n"
	"      DESCENDING_LABEL_ORDER  'z' - 'a'\n"
	"      TOTAL_WEIGHT_ORDER      descending order of the total weight\n"
	"      MAX_WEIGHT_ORDER        descending order of the maximum weight\n"
	"      RANDOM_ORDER            shuffle nodes in random\n"
	"      LABEL_ORDER             ASCENDING_LABEL_ORDER\n"
	"      DEFAULT                 LABEL_ORDER\n";
}

}  // namespace taiju
