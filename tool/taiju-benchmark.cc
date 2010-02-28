#include "time-watch.h"

#include <taiju/line-reader.h>
#include <taiju/trie-builder.h>
#include <taiju/trie.h>
#include <taiju/trie-converter-factory.h>
#include <taiju/trie-factory.h>

#include <algorithm>
#include <cstdio>
#include <cstring>
#include <exception>
#include <fstream>
#include <iostream>
#include <memory>
#include <set>
#include <sstream>
#include <vector>

static taiju::BuilderConfig taiju_config;

void read_keys(std::istream *in, std::vector<std::string> *keys)
{
	std::cerr << "reading keys..." << std::endl;
	taiju::TimeWatch watch;

	taiju::LineReader reader;
	reader.open(in);

	const char *key;
	std::size_t length;
	while (reader.read(&key, &length))
	{
		if (length > 0)
			keys->push_back(key);

		if (keys->size() % 100000 == 0)
		{
			std::cerr << "\rkeys: " << keys->size()
				<< ", time: " << watch.elapsed() << "s  ";
		}
	}
	std::cerr << "\rkeys: " << keys->size()
		<< ", time: " << watch.elapsed() << 's' << std::endl;
}

void sort_keys(std::vector<std::string> *keys)
{
	std::cerr << "sorting keys..." << std::endl;
	taiju::TimeWatch watch;

	std::sort(keys->begin(), keys->end());
	keys->erase(std::unique(keys->begin(), keys->end()), keys->end());

	std::cerr << "keys: " << keys->size()
		<< ", time: " << watch.elapsed() << 's' << std::endl;
}

void randomize_keys(const std::vector<std::string> &keys,
	std::vector<std::string> *rand_keys)
{
	std::cerr << "randomizing keys..." << std::endl;
	taiju::TimeWatch watch;

	std::vector<std::string>(keys).swap(*rand_keys);
	std::random_shuffle(rand_keys->begin(), rand_keys->end());

	std::cerr << "time: " << watch.elapsed() << 's' << std::endl;
}

void build_trie(const std::vector<std::string> &keys, taiju::Trie *trie)
{
	std::cerr << "building a trie..." << std::endl;
	taiju::TimeWatch watch;

	taiju::TrieBuilder builder;
	builder.open(taiju_config);
	for (std::size_t i = 0; i < keys.size(); ++i)
	{
		builder.append(keys[i].c_str(), keys[i].length());
		if (builder.num_keys() % 100000 == 0)
		{
			std::cerr << "\rkeys: " << builder.num_keys()
				<< ", nodes: " << builder.num_nodes()
				<< ", bytes: " << builder.size()
				<< ", time: " << watch.elapsed() << "s  ";
			if (builder.num_keys() % 10000000 == 0)
				std::cerr << '\n';
		}
	}
	builder.finish();
	std::cerr << "\rkeys: " << builder.num_keys()
		<< ", nodes: " << builder.num_nodes()
		<< ", bytes: " << builder.size()
		<< ", time: " << watch.elapsed() << 's' << std::endl;

	std::stringstream stream;
	builder.write(&stream);
	builder.clear();
	trie->read(&stream);
}

std::auto_ptr<taiju::TrieBase> convert_trie(const taiju::Trie &src_trie,
	const std::vector<std::string> &keys)
{
	taiju::TimeWatch watch;

	std::auto_ptr<taiju::TrieConverterBase> converter(
		taiju::TrieConverterFactory::Create(taiju_config));
	std::printf(" %10s", converter->type_name());

	converter->convert(src_trie);

	// Note that the size of BP_TRIE or DFUDS_TRIE will be fixed in
	// the member function write().
	std::stringstream stream;
	converter->write(&stream);
	converter->clear();

	std::auto_ptr<taiju::TrieBase> dest_trie(
		taiju::TrieFactory::read(&stream));

	std::printf(" %7llukb %8.2fus", dest_trie->size() / 1000,
		1e+6 * watch.elapsed() / keys.size());

	return dest_trie;
}

void find_keys(const taiju::TrieBase &trie,
	const std::vector<std::string> &keys)
{
	taiju::TimeWatch watch;

	for (std::size_t i = 0; i < keys.size(); ++i)
	{
//		taiju::UInt64 key_id;
//		if (!trie.find(keys[i].c_str(), keys[i].length(), &key_id))
		if (!trie.find(keys[i].c_str(), keys[i].length()))
		{
			std::cerr << "error: failed to find a key: "
				<< keys[i] << std::endl;
			return;
		}
	}

	std::printf(" %7.2fus", 1e+6 * watch.elapsed() / keys.size());
}

void benchmark_trie(taiju::TrieType trie_type, const taiju::Trie &src_trie,
	const std::vector<std::string> &keys,
	const std::vector<std::string> &rand_keys)
{
	taiju_config.set_trie_type(trie_type);
	std::auto_ptr<taiju::TrieBase> dest_trie(convert_trie(src_trie, keys));

	find_keys(*dest_trie, keys);
	find_keys(*dest_trie, rand_keys);

	std::printf("\n");
}

void benchmark_baseline(	const std::vector<std::string> &keys,
	const std::vector<std::string> &rand_keys)
{
	taiju::TimeWatch watch;

	std::printf(" %10s %9s", "std::set", "");
	std::set<std::string> set(rand_keys.begin(), rand_keys.end());
	std::printf(" %8.2fus", 1e+6 * watch.elapsed() / set.size());

	watch.reset();
	for (std::size_t i = 0; i < keys.size(); ++i)
		set.find(keys[i]);
	std::printf(" %7.2fus", 1e+6 * watch.elapsed() / keys.size());

	watch.reset();
	for (std::size_t i = 0; i < rand_keys.size(); ++i)
		set.find(rand_keys[i]);
	std::printf(" %7.2fus", 1e+6 * watch.elapsed() / rand_keys.size());

	std::printf("\n");
}

void benchmark_tries(const taiju::Trie &trie,
	const std::vector<std::string> &keys,
	const std::vector<std::string> &rand_keys)
{
	std::printf("+----------+---------+----------+-------------------+\n");
	std::printf(" %10s %9s %10s %19s\n",
		"", "", "conversion", "lookup time");
	std::printf(" %10s %9s %10s %9s %9s\n",
		"type", "size", "time", "sorted", "random");
	std::printf("+----------+---------+----------+-------------------+\n");

	benchmark_baseline(keys, rand_keys);
	benchmark_trie(taiju::PODS_TRIE, trie, keys, rand_keys);
	benchmark_trie(taiju::LOB_TRIE, trie, keys, rand_keys);
	benchmark_trie(taiju::LOUDS_TRIE, trie, keys, rand_keys);
	benchmark_trie(taiju::PLOUDS_TRIE, trie, keys, rand_keys);
	benchmark_trie(taiju::BP_TRIE, trie, keys, rand_keys);
	benchmark_trie(taiju::DFUDS_TRIE, trie, keys, rand_keys);

	std::printf("+----------+---------+----------+-------------------+\n");
}

int main(int argc, char *argv[])
{
	try
	{
		taiju_config.parse(argc, argv, &argc);
	}
	catch (...)
	{
		std::cerr << taiju::BuilderConfig::help() << std::endl;
		return 1;
	}

	try
	{
		std::vector<std::string> keys;
		if (argc > 1 && std::strcmp(argv[1], "-") != 0)
		{
			std::ifstream file(argv[1], std::ios::binary);
			if (!file)
				TAIJU_THROW("failed to open input file");
			read_keys(&file, &keys);
		}
		else
			read_keys(&std::cin, &keys);

		sort_keys(&keys);

		std::vector<std::string> rand_keys;
		randomize_keys(keys, &rand_keys);

		taiju::Trie trie;
		build_trie(keys, &trie);

		benchmark_tries(trie, keys, rand_keys);
	}
	catch (const std::exception &ex)
	{
		std::cerr << ex.what() << std::endl;
		return 2;
	}

	return 0;
}
