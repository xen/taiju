#include "time-watch.h"

#include <taiju/line-reader.h>
#include <taiju/trie-builder.h>

#include <cstring>
#include <exception>
#include <fstream>
#include <iostream>

void build_trie(const taiju::BuilderConfig &config,
	std::istream *in, taiju::TrieBuilder *builder)
{
	taiju::TimeWatch watch;

	builder->open(config);

	taiju::LineReader reader;
	reader.open(in);

	const char *key;
	std::size_t length;
	while (reader.read(&key, &length))
	{
		builder->append(key, length);
		if (builder->num_keys() % 100000 == 0)
		{
			std::cerr << "\rkeys: " << builder->num_keys()
				<< ", nodes: " << builder->num_nodes()
				<< ", bytes: " << builder->size();
			if (builder->num_keys() % 10000000 == 0)
				std::cerr << '\n';
		}
	}
	builder->finish();
	std::cerr << "\rkeys: " << builder->num_keys()
		<< ", nodes: " << builder->num_nodes()
		<< ", bytes: " << builder->size() << '\n';

	std::cerr << "time (s): " << watch.elapsed() << std::endl;
}

void write_trie(const taiju::BuilderConfig &config,
	taiju::TrieBuilder *builder, std::ostream *out)
{
	builder->write(out);
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
		taiju::TrieBuilder builder;
		if (argc > 1 && std::strcmp(argv[1], "-") != 0)
		{
			std::cerr << "input: " << argv[1] << std::endl;
			std::ifstream file(argv[1], std::ios::binary);
			if (!file)
				TAIJU_THROW("failed to open input file");
			build_trie(config, &file, &builder);
		}
		else
			build_trie(config, &std::cin, &builder);

		if (argc > 2 && std::strcmp(argv[2], "-") != 0)
		{
			std::cerr << "output: " << argv[2] << std::endl;
			std::ofstream file(argv[2], std::ios::binary);
			if (!file)
				TAIJU_THROW("failed to open output file");
			write_trie(config, &builder, &file);
		}
		else
			write_trie(config, &builder, &std::cout);
	}
	catch (const std::exception &ex)
	{
		std::cerr << ex.what() << std::endl;
		return 2;
	}

	return 0;
}
