#ifndef TAIJU_BP_TRIE_CONVERTER_H
#define TAIJU_BP_TRIE_CONVERTER_H

#include "balanced-bit-vector-builder.h"
#include "trie-converter-base.h"

namespace taiju {

class BpTrieConverter : public TrieConverterBase
{
public:
	BpTrieConverter();
	~BpTrieConverter() { clear(); }

	TrieType type() const { return BP_TRIE; }
	const char *type_name() const { return "BpTrie"; }

	bool empty() const { return num_keys_ == 0; }
	UInt64 num_keys() const { return num_keys_; }
	UInt64 num_nodes() const { return labels_.num_objs(); }
	UInt64 size() const;

	bool finished() const { return labels_.is_open(); }

	void convert(const TrieBase &trie);

	void clear() { close(); }
	void swap(BpTrieConverter *target);
	void write(Writer writer);

private:
	UInt64 num_keys_;
	BalancedBitVectorBuilder bp_;
	VectorBuilder<UInt8> labels_;
	BitVectorBuilder is_terminals_;

	// Disallows copies.
	BpTrieConverter(const BpTrieConverter &);
	BpTrieConverter &operator=(const BpTrieConverter &);

	void open();
	void close();

	void build(const TrieBase &trie);
	void build(const TrieBase &trie, UInt64 id);
	void finish();
};

}  // namespace taiju

#endif  // TAIJU_BP_TRIE_CONVERTER_H
