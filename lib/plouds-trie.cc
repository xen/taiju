#include <taiju/plouds-trie.h>

namespace taiju {

PloudsTrie::PloudsTrie() : num_keys_(0), has_childs_(),
	has_siblings_(), labels_(), is_terminals_() {}

UInt64 PloudsTrie::size() const
{
	return sizeof(TrieType) + sizeof(UInt64) + has_childs_.size()
		+ has_siblings_.size() + labels_.size() + is_terminals_.size();
}

void PloudsTrie::clear()
{
	num_keys_ = 0;
	has_childs_.clear();
	has_siblings_.clear();
	labels_.clear();
	is_terminals_.clear();
}

void PloudsTrie::swap(PloudsTrie *target)
{
	std::swap(num_keys_, target->num_keys_);
	has_childs_.swap(&target->has_childs_);
	has_siblings_.swap(&target->has_siblings_);
	labels_.swap(&target->labels_);
	is_terminals_.swap(&target->is_terminals_);
}

const void *PloudsTrie::map(Mapper mapper, bool checks_type)
{
	PloudsTrie temp;

	if (checks_type)
	{
		if (*mapper.map<TrieType>() != type())
			TAIJU_THROW("failed to map PloudsTrie: wrong TrieType");
	}

	temp.num_keys_ = *mapper.map<UInt64>();
	mapper = temp.has_childs_.map(mapper);
	mapper = temp.has_siblings_.map(mapper);
	mapper = temp.labels_.map(mapper);
	mapper = temp.is_terminals_.map(mapper);

	swap(&temp);

	return mapper.address();
}

void PloudsTrie::read(Reader reader, bool checks_type)
{
	PloudsTrie temp;

	if (checks_type)
	{
		TrieType trie_type;
		reader.read(&trie_type);
		if (trie_type != type())
			TAIJU_THROW("failed to read PloudsTrie: wrong TrieType");
	}

	reader.read(&temp.num_keys_);
	temp.has_childs_.read(reader);
	temp.has_siblings_.read(reader);
	temp.labels_.read(reader);
	temp.is_terminals_.read(reader);

	swap(&temp);
}

void PloudsTrie::write(Writer writer) const
{
	writer.write(num_keys_);
	has_childs_.write(writer);
	has_siblings_.write(writer);
	labels_.write(writer);
	is_terminals_.write(writer);
}

}  // namespace taiju
