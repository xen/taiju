#include <taiju/trie-factory.h>

#include <taiju/pods-trie.h>
#include <taiju/lob-trie.h>
#include <taiju/louds-trie.h>
#include <taiju/plouds-trie.h>
#include <taiju/bp-trie.h>
#include <taiju/dfuds-trie.h>

#include <memory>

namespace taiju {

TrieBase *TrieFactory::Create(TrieType type)
{
	try
	{
		switch (type)
		{
		case PODS_TRIE:
			return new PodsTrie;
		case LOB_TRIE:
			return new LobTrie;
		case LOUDS_TRIE:
			return new LoudsTrie;
		case PLOUDS_TRIE:
			return new PloudsTrie;
		case BP_TRIE:
			return new BpTrie;
		case DFUDS_TRIE:
			return new DfudsTrie;
		default:
			TAIJU_THROW("failed to create trie: invalid TrieType");
		}
	}
	catch (const std::bad_alloc &)
	{
		TAIJU_THROW("failed to create trie: std::bad_alloc");
	}
}

TrieBase *TrieFactory::map(Mapper mapper, const void **address_ptr)
{
	TrieType type = *mapper.map<TrieType>();
	std::auto_ptr<TrieBase> trie(Create(type));

	const void *address = trie->map(mapper, false);
	if (address_ptr != NULL)
		*address_ptr = address;
	return trie.release();
}

TrieBase *TrieFactory::read(Reader reader)
{
	TrieType type;
	reader.read(&type);
	std::auto_ptr<TrieBase> trie(Create(type));

	trie->read(reader, false);
	return trie.release();
}

}  // namespace taiju
