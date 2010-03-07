#ifndef TAIJU_LOB_TRIE_CONVERTER_H
#define TAIJU_LOB_TRIE_CONVERTER_H

#include "bit-vector-builder.h"
#include "trie-converter-base.h"

namespace taiju {

class LobTrieConverter : public TrieConverterBase
{
public:
	LobTrieConverter() : num_keys_(0), lob_(), labels_(), is_terminals_() {}
	~LobTrieConverter() { clear(); }

	TrieType type() const { return LOB_TRIE; }
	const char *type_name() const { return "LobTrie"; }

	bool empty() const { return num_keys_ == 0; }
	UInt64 num_keys() const { return num_keys_; }
	UInt64 num_nodes() const { return labels_.num_objs(); }
	UInt64 size() const;

	bool finished() const { return labels_.is_open(); }

	void convert(const TrieBase &trie);

	void clear() { close(); }
	void swap(LobTrieConverter *target);
	void write(Writer writer);

private:
	UInt64 num_keys_;
	BitVectorBuilder lob_;
	VectorBuilder<UInt8> labels_;
	BitVectorBuilder is_terminals_;

	// Disallows copies.
	LobTrieConverter(const LobTrieConverter &);
	LobTrieConverter &operator=(const LobTrieConverter &);

	void open();
	void close();

	void build(const TrieBase &trie);
	void finish();
};

}  // namespace taiju

#endif  // TAIJU_LOB_TRIE_CONVERTER_H
