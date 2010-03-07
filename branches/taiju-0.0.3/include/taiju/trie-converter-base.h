#ifndef TAIJU_TRIE_CONVERTER_BASE_H
#define TAIJU_TRIE_CONVERTER_BASE_H

#include "builder-config.h"
#include "trie-base.h"

namespace taiju {

class TrieConverterBase
{
public:
	TrieConverterBase() {}
	virtual ~TrieConverterBase() {}

	virtual TrieType type() const = 0;
	virtual const char *type_name() const = 0;

	virtual bool empty() const = 0;
	virtual UInt64 num_keys() const = 0;
	virtual UInt64 num_nodes() const = 0;
	virtual UInt64 size() const = 0;

	virtual bool finished() const = 0;

	virtual void convert(const TrieBase &trie) = 0;

	virtual void clear() = 0;
	virtual void write(Writer writer) = 0;

private:
	// Disallows copies.
	TrieConverterBase(const TrieConverterBase &);
	TrieConverterBase &operator=(const TrieConverterBase &);
};

}  // namespace taiju

#endif  // TAIJU_TRIE_CONVERTER_BASE_H
