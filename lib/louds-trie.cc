#include <taiju/louds-trie.h>

namespace taiju {

UInt64 LoudsTrie::size() const
{
	return sizeof(TrieType) + sizeof(UInt64) + louds_.size()
		+ labels_.size() + is_terminals_.size();
}

void LoudsTrie::clear()
{
	num_keys_ = 0;
	louds_.clear();
	labels_.clear();
	is_terminals_.clear();
}

void LoudsTrie::swap(LoudsTrie *target)
{
	std::swap(num_keys_, target->num_keys_);
	louds_.swap(&target->louds_);
	labels_.swap(&target->labels_);
	is_terminals_.swap(&target->is_terminals_);
}

const void *LoudsTrie::map(Mapper mapper, bool checks_type)
{
	LoudsTrie temp;

	if (checks_type)
	{
		if (*mapper.map<TrieType>() != type())
			TAIJU_THROW("failed to map LoudsTrie: wrong TrieType");
	}

	temp.num_keys_ = *mapper.map<UInt64>();
	mapper = temp.louds_.map(mapper);
	mapper = temp.labels_.map(mapper);
	mapper = temp.is_terminals_.map(mapper);

	swap(&temp);

	return mapper.address();
}

void LoudsTrie::read(Reader reader, bool checks_type)
{
	LoudsTrie temp;

	if (checks_type)
	{
		TrieType trie_type;
		reader.read(&trie_type);
		if (trie_type != type())
			TAIJU_THROW("failed to read LoudsTrie: wrong TrieType");
	}

	reader.read(&temp.num_keys_);
	temp.louds_.read(reader);
	temp.labels_.read(reader);
	temp.is_terminals_.read(reader);

	swap(&temp);
}

void LoudsTrie::write(Writer writer) const
{
	writer.write(num_keys_);
	louds_.write(writer);
	labels_.write(writer);
	is_terminals_.write(writer);
}

}  // namespace taiju
