#include <taiju/trie.h>
#include <taiju/trie-builder.h>
#include <taiju/trie-factory.h>
#include <taiju/trie-converter-factory.h>

#include <cassert>
#include <cstdlib>
#include <ctime>
#include <memory>
#include <set>
#include <sstream>
#include <string>
#include <vector>

void GenerateValidKeys(std::size_t num_keys,
	std::set<std::string> *valid_keys)
{
	std::vector<char> key;
	while (valid_keys->size() < num_keys)
	{
		key.resize(1 + (std::rand() % 6));
		for (std::size_t i = 0; i < key.size(); ++i)
			key[i] = 'A' + (std::rand() % 26);
		valid_keys->insert(std::string(&key[0], key.size()));
	}
}

void GenerateInvalidKeys(std::size_t num_keys,
	const std::set<std::string> &valid_keys,
	std::set<std::string> *invalid_keys)
{
	std::vector<char> key;
	while (invalid_keys->size() < num_keys)
	{
		key.resize(1 + (std::rand() % 8));
		for (std::size_t i = 0; i < key.size(); ++i)
			key[i] = 'A' + (std::rand() % 26);

		std::string generated_key(&key[0], key.size());
		if (valid_keys.find(generated_key) == valid_keys.end())
			invalid_keys->insert(std::string(&key[0], key.size()));
	}
}

void TestTrieContents(const taiju::TrieBase &trie,
	const std::set<std::string> &valid_keys,
	const std::set<std::string> &invalid_keys)
{
	std::vector<bool> key_id_flags(valid_keys.size(), false);
	for (std::set<std::string>::const_iterator it = valid_keys.begin();
		it != valid_keys.end(); ++it)
	{
		taiju::UInt64 key_id;
		assert(trie.find(it->data(), it->size(), &key_id));
		key_id_flags[static_cast<std::size_t>(key_id)] = true;
	}
	for (std::size_t i = 0; i < key_id_flags.size(); ++i)
		assert(key_id_flags[i]);

	for (std::set<std::string>::const_iterator it = invalid_keys.begin();
		it != invalid_keys.end(); ++it)
		assert(!trie.find(it->data(), it->size()));
}

void TestFurtherDerivedTrie(taiju::TrieType trie_type,
	const taiju::TrieBase &trie,
	const std::set<std::string> &valid_keys,
	const std::set<std::string> &invalid_keys)
{
	std::auto_ptr<taiju::TrieConverterBase> converter(
		taiju::TrieConverterFactory::Create(trie_type));
	converter->convert(trie);
	assert(converter->type() == trie_type);
	assert(converter->num_keys() == trie.num_keys());
	assert(converter->num_nodes() == trie.num_nodes());

	std::stringstream stream;
	converter->write(&stream);

	std::string str = stream.str();
	assert(str.size() == converter->size());

	std::auto_ptr<taiju::TrieBase> derived_trie(
		taiju::TrieFactory::map(str.data()));
	assert(derived_trie->type() == converter->type());
	assert(derived_trie->num_keys() == converter->num_keys());
	assert(derived_trie->num_nodes() == converter->num_nodes());
	assert(derived_trie->size() == converter->size());

	TestTrieContents(*derived_trie, valid_keys, invalid_keys);
}

void TestDerivedTrie(taiju::TrieType trie_type, const taiju::TrieBase &trie,
	const std::set<std::string> &valid_keys,
	const std::set<std::string> &invalid_keys)
{
	std::auto_ptr<taiju::TrieConverterBase> converter(
		taiju::TrieConverterFactory::Create(trie_type));
	converter->convert(trie);
	assert(converter->type() == trie_type);
	assert(converter->num_keys() == trie.num_keys());
	assert(converter->num_nodes() == trie.num_nodes());

	std::stringstream stream;
	converter->write(&stream);

	std::string str = stream.str();
	assert(str.size() == converter->size());

	std::auto_ptr<taiju::TrieBase> derived_trie(
		taiju::TrieFactory::map(str.data()));
	assert(derived_trie->type() == converter->type());
	assert(derived_trie->num_keys() == converter->num_keys());
	assert(derived_trie->num_nodes() == converter->num_nodes());
	assert(derived_trie->size() == converter->size());

	TestTrieContents(*derived_trie, valid_keys, invalid_keys);

	derived_trie.reset(taiju::TrieFactory::read(&stream));
	assert(derived_trie->type() == converter->type());
	assert(derived_trie->num_keys() == converter->num_keys());
	assert(derived_trie->num_nodes() == converter->num_nodes());
	assert(derived_trie->size() == converter->size());

	TestTrieContents(*derived_trie, valid_keys, invalid_keys);

	TestFurtherDerivedTrie(taiju::PODS_TRIE,
		*derived_trie, valid_keys, invalid_keys);
	TestFurtherDerivedTrie(taiju::LOB_TRIE,
		*derived_trie, valid_keys, invalid_keys);
	TestFurtherDerivedTrie(taiju::LOUDS_TRIE,
		*derived_trie, valid_keys, invalid_keys);
	TestFurtherDerivedTrie(taiju::PLOUDS_TRIE,
		*derived_trie, valid_keys, invalid_keys);
	TestFurtherDerivedTrie(taiju::BP_TRIE,
		*derived_trie, valid_keys, invalid_keys);
	TestFurtherDerivedTrie(taiju::DFUDS_TRIE,
		*derived_trie, valid_keys, invalid_keys);
}

void TestDerivedTries(const taiju::TrieBase &trie,
	const std::set<std::string> &valid_keys,
	const std::set<std::string> &invalid_keys)
{
	std::cerr << " PODS: ";
	TestDerivedTrie(taiju::PODS_TRIE, trie, valid_keys, invalid_keys);

	std::cerr << "ok, LOB: ";
	TestDerivedTrie(taiju::LOB_TRIE, trie, valid_keys, invalid_keys);

	std::cerr << "ok, LOUDS: ";
	TestDerivedTrie(taiju::LOUDS_TRIE, trie, valid_keys, invalid_keys);

	std::cerr << "ok, PLOUDS: ";
	TestDerivedTrie(taiju::PLOUDS_TRIE, trie, valid_keys, invalid_keys);

	std::cerr << "ok, BP: ";
	TestDerivedTrie(taiju::BP_TRIE, trie, valid_keys, invalid_keys);

	std::cerr << "ok, DFUDS: ";
	TestDerivedTrie(taiju::DFUDS_TRIE, trie, valid_keys, invalid_keys);

	std::cerr << "ok" << std::endl;
}

void TestTrie(taiju::NodeOrder node_order,
	const std::set<std::string> &valid_keys,
	const std::set<std::string> &invalid_keys)
{
	taiju::TrieBuilder builder;
	builder.open(node_order);
	for (std::set<std::string>::const_iterator it = valid_keys.begin();
		it != valid_keys.end(); ++it)
		builder.append(it->data(), it->size(), 1.0 * std::rand() / RAND_MAX);
	builder.finish();
	assert(builder.num_keys() == valid_keys.size());

	std::stringstream stream;
	builder.write(&stream);

	std::string str = stream.str();
	assert(str.size() == builder.size());

	taiju::Trie trie;

	trie.map(str.data());
	assert(trie.num_keys() == builder.num_keys());
	assert(trie.num_nodes() == builder.num_nodes());
	assert(trie.size() == builder.size());

	TestTrieContents(trie, valid_keys, invalid_keys);

	trie.read(&stream);
	assert(trie.num_keys() == builder.num_keys());
	assert(trie.num_nodes() == builder.num_nodes());
	assert(trie.size() == builder.size());

	TestTrieContents(trie, valid_keys, invalid_keys);

	std::cerr << "ok" << std::endl;

	TestDerivedTries(trie, valid_keys, invalid_keys);
}

int main()
{
	std::srand(static_cast<unsigned int>(std::time(NULL)));

	static const std::size_t NUM_VALID_KEYS = 1 << 15;
	static const std::size_t NUM_INVALID_KEYS = 1 << 16;

	std::set<std::string> valid_keys;
	GenerateValidKeys(NUM_VALID_KEYS, &valid_keys);

	std::set<std::string> invalid_keys;
	GenerateInvalidKeys(NUM_INVALID_KEYS, valid_keys, &invalid_keys);

	std::cerr << "TrieBuilder with ASCENDING_LABEL_ORDER: ";
	TestTrie(taiju::ASCENDING_LABEL_ORDER, valid_keys, invalid_keys);

	std::cerr << "TrieBuilder with DESCENDING_LABEL_ORDER: ";
	TestTrie(taiju::DESCENDING_LABEL_ORDER, valid_keys, invalid_keys);

	std::cerr << "TrieBuilder with TOTAL_WEIGHT_ORDER: ";
	TestTrie(taiju::TOTAL_WEIGHT_ORDER, valid_keys, invalid_keys);

	std::cerr << "TrieBuilder with MAX_WEIGHT_ORDER: ";
	TestTrie(taiju::MAX_WEIGHT_ORDER, valid_keys, invalid_keys);

	return 0;
}
