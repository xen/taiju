#ifndef TAIJU_PODS_TRIE_CONVERTER_H
#define TAIJU_PODS_TRIE_CONVERTER_H

#include "biased-int-vector-builder.h"
#include "trie-converter-base.h"

namespace taiju {

class PodsTrieConverter : public TrieConverterBase
{
public:
	PodsTrieConverter();
	~PodsTrieConverter() { clear(); }

	TrieType type() const { return PODS_TRIE; }
	const char *type_name() const { return "PodsTrie"; }

	bool empty() const { return num_keys_ == 0; }
	UInt64 num_keys() const { return num_keys_; }
	UInt64 num_nodes() const { return labels_.num_objs(); }
	UInt64 size() const;

	bool finished() const { return labels_.is_open(); }

	void convert(const TrieBase &trie);

	void clear() { close(); }
	void swap(PodsTrieConverter *target);
	void write(Writer writer);

private:
	UInt64 num_keys_;
	BiasedIntVectorBuilder child_diffs_;
	VectorBuilder<bool> has_siblings_;
	VectorBuilder<UInt8> labels_;
	BitVectorBuilder is_terminals_;

	// Disallows copies.
	PodsTrieConverter(const PodsTrieConverter &);
	PodsTrieConverter &operator=(const PodsTrieConverter &);

	void open();
	void close();

	void build(const TrieBase &trie);
	UInt64 build(const TrieBase &trie, UInt64 id);
	void finish();
};

}  // namespace taiju

#endif  // TAIJU_PODS_TRIE_CONVERTER_H
