#include "time-watch.h"

#include <taiju/trie-factory.h>
#include <taiju/trie-converter-factory.h>

#include <cstring>
#include <exception>
#include <fstream>
#include <memory>
#include <iostream>

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
		if (argc > 1 && std::strcmp(argv[1], "-") != 0)
		{
			std::cerr << "input: " << argv[1] << std::endl;
			std::ifstream file(argv[1], std::ios::binary);
			if (!file)
				TAIJU_THROW("failed to open input file");
			trie.reset(taiju::TrieFactory::read(&file));
		}
		else
			trie.reset(taiju::TrieFactory::read(&std::cin));

		std::auto_ptr<taiju::TrieConverterBase> converter(
			taiju::TrieConverterFactory::Create(config));

		taiju::TimeWatch watch;
		std::cerr << "before conversion: keys: " << trie->num_keys()
			<< ", nodes: " << trie->num_nodes()
			<< ", bytes: " << trie->size() << std::endl;

		converter->convert(*trie);

		std::cerr << "after conversion: keys: " << converter->num_keys()
			<< ", nodes: " << converter->num_nodes()
			<< ", bytes: " << converter->size() << std::endl;

		trie.reset();

		if (argc > 2 && std::strcmp(argv[2], "-") != 0)
		{
			std::cerr << "output: " << argv[2] << std::endl;
			std::ofstream file(argv[2], std::ios::binary);
			if (!file)
				TAIJU_THROW("failed to open output file");
			converter->write(&file);
		}
		else
			converter->write(&std::cout);

		std::cerr << "after output: keys: " << converter->num_keys()
			<< ", nodes: " << converter->num_nodes()
			<< ", bytes: " << converter->size() << std::endl;

		std::cerr << "time (s): " << watch.elapsed() << std::endl;
	}
	catch (const std::exception &ex)
	{
		std::cerr << ex.what() << std::endl;
		return 2;
	}

	return 0;
}
