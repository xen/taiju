#include "time-watch.h"

#include <taiju/builder-config.h>
#include <taiju/file-map.h>
#include <taiju/line-reader.h>
#include <taiju/trie-factory.h>

#include <cstring>
#include <exception>
#include <fstream>
#include <iostream>
#include <memory>
#include <string>

void trie_find(const taiju::TrieBase &trie, std::istream *in)
{
	taiju::TimeWatch watch;

	taiju::LineReader reader;
	reader.open(in);

	taiju::UInt64 num_keys = 0;
	taiju::UInt64 num_found = 0;

	const char *key;
	std::size_t length;
	while (reader.read(&key, &length))
	{
		if (trie.find(key, length))
			++num_found;

		if (++num_keys % 100000 == 0)
		{
			std::cerr << "\rkeys: " << num_keys << ", found: " << num_found
				<< ", not found: " << (num_keys - num_found);
		}
	}
	std::cerr << "\rkeys: " << num_keys << ", found: " << num_found
		<< ", not found: " << (num_keys - num_found) << std::endl;

	std::cerr << "time (s): " << watch.elapsed() << std::endl;
}

int main(int argc, char *argv[])
{
	taiju::BuilderConfig config;
	try
	{
		config.parse(argc, argv, &argc);
	}
	catch (...)
	{
		std::cerr << taiju::BuilderConfig::help() << std::endl;
		return 1;
	}

	try
	{
		std::auto_ptr<taiju::TrieBase> trie;
		taiju::FileMap file_map;
		if (argc > 1 && std::strcmp(argv[1], "-") != 0)
		{
			std::cerr << "trie: " << argv[1] << std::endl;
			file_map.open(argv[1]);
			trie.reset(taiju::TrieFactory::map(file_map.address()));
		}
		else
			trie.reset(taiju::TrieFactory::read(&std::cin));

		if (argc > 2 && std::strcmp(argv[2], "-") != 0)
		{
			std::cerr << "query: " << argv[2] << std::endl;
			std::ifstream file(argv[2], std::ios::binary);
			if (!file)
				TAIJU_THROW("failed to open query file");
			trie_find(*trie, &file);
		}
		else
			trie_find(*trie, &std::cin);
	}
	catch (const std::exception &ex)
	{
		std::cerr << ex.what() << std::endl;
		return 2;
	}

	return 0;
}
