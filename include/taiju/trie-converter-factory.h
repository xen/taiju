#ifndef TAIJU_TRIE_CONVERTER_FACTORY_H
#define TAIJU_TRIE_CONVERTER_FACTORY_H

#include "trie-converter-base.h"

namespace taiju {

class TrieConverterFactory
{
public:
	static TrieConverterBase *Create(const BuilderConfig &config);
	static TrieConverterBase *Create(TrieType type);

private:
	// Disallows instantiation.
	TrieConverterFactory();
	~TrieConverterFactory();

	// Disallows copies.
	TrieConverterFactory(const TrieConverterFactory &);
	TrieConverterFactory &operator=(const TrieConverterFactory &);
};

}  // namespace taiju

#endif  // TAIJU_TRIE_CONVERTER_FACTORY_H
