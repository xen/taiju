#include <taiju/builder-config.h>
#include <taiju/file-map.h>
#include <taiju/trie-factory.h>

#include <cstring>
#include <exception>
#include <fstream>
#include <iostream>
#include <memory>
#include <string>

void trie_find(const taiju::TrieBase &trie, std::istream *in)
{
	std::string key;
	while (std::getline(*in, key))
	{
		taiju::UInt64 key_id;
		if (trie.find(key.c_str(), key.length(), &key_id))
			std::cout << key << ": found: " << key_id << '\n';
		else
			std::cout << key << ": not found\n";
	}
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
