#include <taiju/bp-trie.h>

namespace taiju {

UInt64 BpTrie::size() const
{
	return sizeof(TrieType) + sizeof(UInt64) + bp_.size()
		+ labels_.size() + is_terminals_.size();
}

void BpTrie::clear()
{
	num_keys_ = 0;
	bp_.clear();
	labels_.clear();
	is_terminals_.clear();
}

void BpTrie::swap(BpTrie *target)
{
	std::swap(num_keys_, target->num_keys_);
	bp_.swap(&target->bp_);
	labels_.swap(&target->labels_);
	is_terminals_.swap(&target->is_terminals_);
}

const void *BpTrie::map(Mapper mapper, bool checks_type)
{
	BpTrie temp;

	if (checks_type)
	{
		if (*mapper.map<TrieType>() != type())
			TAIJU_THROW("failed to map BpTrie: wrong TrieType");
	}

	temp.num_keys_ = *mapper.map<UInt64>();
	mapper = temp.bp_.map(mapper);
	mapper = temp.labels_.map(mapper);
	mapper = temp.is_terminals_.map(mapper);

	swap(&temp);

	return mapper.address();
}

void BpTrie::read(Reader reader, bool checks_type)
{
	BpTrie temp;

	if (checks_type)
	{
		TrieType trie_type;
		reader.read(&trie_type);
		if (trie_type != type())
			TAIJU_THROW("failed to read BpTrie: wrong TrieType");
	}

	reader.read(&temp.num_keys_);
	temp.bp_.read(reader);
	temp.labels_.read(reader);
	temp.is_terminals_.read(reader);

	swap(&temp);
}

void BpTrie::write(Writer writer) const
{
	writer.write(num_keys_);
	bp_.write(writer);
	labels_.write(writer);
	is_terminals_.write(writer);
}

}  // namespace taiju
