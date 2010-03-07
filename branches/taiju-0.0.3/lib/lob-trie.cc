#include <taiju/lob-trie.h>

namespace taiju {

UInt64 LobTrie::size() const
{
	return sizeof(TrieType) + sizeof(UInt64) + lob_.size()
		+ labels_.size() + is_terminals_.size();
}

void LobTrie::clear()
{
	num_keys_ = 0;
	lob_.clear();
	labels_.clear();
	is_terminals_.clear();
}

void LobTrie::swap(LobTrie *target)
{
	std::swap(num_keys_, target->num_keys_);
	lob_.swap(&target->lob_);
	labels_.swap(&target->labels_);
	is_terminals_.swap(&target->is_terminals_);
}

const void *LobTrie::map(Mapper mapper, bool checks_type)
{
	LobTrie temp;

	if (checks_type)
	{
		if (*mapper.map<TrieType>() != type())
			TAIJU_THROW("failed to map LobTrie: wrong TrieType");
	}

	temp.num_keys_ = *mapper.map<UInt64>();
	mapper = temp.lob_.map(mapper);
	mapper = temp.labels_.map(mapper);
	mapper = temp.is_terminals_.map(mapper);

	swap(&temp);

	return mapper.address();
}

void LobTrie::read(Reader reader, bool checks_type)
{
	LobTrie temp;

	if (checks_type)
	{
		TrieType trie_type;
		reader.read(&trie_type);
		if (trie_type != type())
			TAIJU_THROW("failed to read LobTrie: wrong TrieType");
	}

	reader.read(&temp.num_keys_);
	temp.lob_.read(reader);
	temp.labels_.read(reader);
	temp.is_terminals_.read(reader);

	swap(&temp);
}

void LobTrie::write(Writer writer) const
{
	writer.write(num_keys_);
	lob_.write(writer);
	labels_.write(writer);
	is_terminals_.write(writer);
}

}  // namespace taiju
