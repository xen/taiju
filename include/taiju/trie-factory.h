#ifndef TAIJU_TRIE_FACTORY_H
#define TAIJU_TRIE_FACTORY_H

#include "trie-base.h"

namespace taiju {

class TrieFactory
{
public:
	static TrieBase *Create(TrieType type);
	static TrieBase *map(Mapper mapper, const void **address_ptr = NULL);
	static TrieBase *read(Reader reader);

private:
	// Disallows instantiation.
	TrieFactory();
	~TrieFactory();

	// Disallows copies.
	TrieFactory(const TrieFactory &);
	TrieFactory &operator=(const TrieFactory &);
};

}  // namespace taiju

#endif  // TAIJU_TRIE_FACTORY_H
