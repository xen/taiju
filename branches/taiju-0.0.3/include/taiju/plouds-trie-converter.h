#ifndef TAIJU_PLOUDS_TRIE_CONVERTER_H
#define TAIJU_PLOUDS_TRIE_CONVERTER_H

#include "bit-vector-builder.h"
#include "trie-converter-base.h"

namespace taiju {

class PloudsTrieConverter : public TrieConverterBase
{
public:
	PloudsTrieConverter();
	~PloudsTrieConverter() { clear(); }

	TrieType type() const { return PLOUDS_TRIE; }
	const char *type_name() const { return "PloudsTrie"; }

	bool empty() const { return num_keys_ == 0; }
	UInt64 num_keys() const { return num_keys_; }
	UInt64 num_nodes() const { return labels_.num_objs(); }
	UInt64 size() const;

	bool finished() const { return labels_.is_open(); }

	void convert(const TrieBase &trie);

	void clear() { close(); }
	void swap(PloudsTrieConverter *target);
	void write(Writer writer);

private:
	UInt64 num_keys_;
	BitVectorBuilder has_childs_;
	BitVectorBuilder has_siblings_;
	VectorBuilder<UInt8> labels_;
	BitVectorBuilder is_terminals_;

	// Disallows copies.
	PloudsTrieConverter(const PloudsTrieConverter &);
	PloudsTrieConverter &operator=(const PloudsTrieConverter &);

	void open();
	void close();

	void build(const TrieBase &trie);
	void finish();
};

}  // namespace taiju

#endif  // TAIJU_PLOUDS_TRIE_CONVERTER_H
