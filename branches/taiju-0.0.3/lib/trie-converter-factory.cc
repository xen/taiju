#include <taiju/trie-converter-factory.h>

#include <taiju/pods-trie-converter.h>
#include <taiju/lob-trie-converter.h>
#include <taiju/louds-trie-converter.h>
#include <taiju/plouds-trie-converter.h>
#include <taiju/bp-trie-converter.h>
#include <taiju/dfuds-trie-converter.h>

namespace taiju {

TrieConverterBase *TrieConverterFactory::Create(const BuilderConfig &config)
{
	return Create(config.trie_type());
}

TrieConverterBase *TrieConverterFactory::Create(TrieType type)
{
	try
	{
		switch (type)
		{
		case PODS_TRIE:
			return new PodsTrieConverter;
		case LOB_TRIE:
			return new LobTrieConverter;
		case LOUDS_TRIE:
			return new LoudsTrieConverter;
		case PLOUDS_TRIE:
			return new PloudsTrieConverter;
		case BP_TRIE:
			return new BpTrieConverter;
		case DFUDS_TRIE:
			return new DfudsTrieConverter;
		default:
			TAIJU_THROW("failed to create converter: invalid TrieType");
		}
	}
	catch (const std::bad_alloc &)
	{
		TAIJU_THROW("failed to create converter: std::bad_alloc");
	}
}

}  // namespace taiju
