#ifndef TAIJU_BUILDER_CONFIG_H
#define TAIJU_BUILDER_CONFIG_H

#include "int-types.h"

namespace taiju {

class BuilderConfig
{
public:
	BuilderConfig();
	~BuilderConfig() { clear(); }

	TrieType trie_type() const { return trie_type_; }
	IndexType index_type() const { return index_type_; }
	NodeOrder node_order() const { return node_order_; }

	void set_trie_type(TrieType trie_type) { trie_type_ = trie_type; }
	void set_index_type(IndexType index_type) { index_type_ = index_type; }
	void set_node_order(NodeOrder node_order) { node_order_ = node_order; }

	void parse(int argc, char **argv, int *argc_ptr = NULL);

	void clear();
	void swap(BuilderConfig *target);

	static const char *help();

private:
	TrieType trie_type_;
	IndexType index_type_;
	NodeOrder node_order_;

	// Disallows copies.
	BuilderConfig(const BuilderConfig &);
	BuilderConfig &operator=(const BuilderConfig &);

	void parse_trie_type(const char *value);
	void parse_index_type(const char *value);
	void parse_node_order(const char *value);
};

}  // namespace taiju

#endif  // TAIJU_BUILDER_CONFIG_H
