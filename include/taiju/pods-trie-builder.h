#ifndef TAIJU_PODS_TRIE_BUILDER_H
#define TAIJU_PODS_TRIE_BUILDER_H

#include "biased-int-vector-builder.h"
#include "builder-config.h"
#include "trie-node.h"

namespace taiju {

class PodsTrieBuilder
{
public:
	PodsTrieBuilder();
	~PodsTrieBuilder() { clear(); }

	TrieType type() const { return PODS_TRIE; }
	const char *type_name() const { return "PodsTrie"; }

	bool empty() const { return num_keys_ == 0; }
	UInt64 num_keys() const { return num_keys_; }
	UInt64 num_nodes() const { return labels_.num_objs() + inters_.size(); }
	UInt64 size() const;

	bool is_open() const { return labels_.is_open(); }
	bool finished() const { return finished_; }

	void open(const BuilderConfig &config) { open(config.node_order()); }
	void open(NodeOrder node_order = DEFAULT_NODE_ORDER);
	void close();

	void append(const void *ptr, UInt64 size, double weight = 1.0);
	void finish();

	void clear() { close(); }
	void swap(PodsTrieBuilder *target);
	void write(Writer writer);

private:
	NodeOrder node_order_;
	UInt64 num_keys_;
	BiasedIntVectorBuilder child_diffs_;
	VectorBuilder<bool> has_siblings_;
	VectorBuilder<UInt8> labels_;
	BitVectorBuilder is_terminals_;
	Vector<TrieNode> inters_;
	bool finished_;

	// Disallows copies.
	PodsTrieBuilder(const PodsTrieBuilder &);
	PodsTrieBuilder &operator=(const PodsTrieBuilder &);

	void append_key(const UInt8 *key, UInt64 length, double weight);
	void append_inter(const TrieNode &node);
	void append_node(const TrieNode &node);

	void flush(UInt64 root);
};

}  // namespace taiju

#endif  // TAIJU_PODS_TRIE_BUILDER_H
