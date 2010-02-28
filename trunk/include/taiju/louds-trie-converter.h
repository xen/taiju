#ifndef TAIJU_LOUDS_TRIE_CONVERTER_H
#define TAIJU_LOUDS_TRIE_CONVERTER_H

#include "bit-vector-builder.h"
#include "trie-converter-base.h"

namespace taiju {

class LoudsTrieConverter : public TrieConverterBase
{
public:
	LoudsTrieConverter();
	~LoudsTrieConverter() { clear(); }

	TrieType type() const { return LOUDS_TRIE; }
	const char *type_name() const { return "LoudsTrie"; }

	bool empty() const { return num_keys_ == 0; }
	UInt64 num_keys() const { return num_keys_; }
	UInt64 num_nodes() const { return labels_.num_objs(); }
	UInt64 size() const;

	bool finished() const { return labels_.is_open(); }

	void convert(const TrieBase &trie);

	void clear() { close(); }
	void swap(LoudsTrieConverter *target);
	void write(Writer writer);

private:
	UInt64 num_keys_;
	BitVectorBuilder louds_;
	VectorBuilder<UInt8> labels_;
	BitVectorBuilder is_terminals_;

	// Disallows copies.
	LoudsTrieConverter(const LoudsTrieConverter &);
	LoudsTrieConverter &operator=(const LoudsTrieConverter &);

	void open();
	void close();

	void build(const TrieBase &trie);
	void finish();
};

}  // namespace taiju

#endif  // TAIJU_LOUDS_TRIE_CONVERTER_H
