#include <taiju/dfuds-trie.h>

namespace taiju {

UInt64 DfudsTrie::size() const
{
	return sizeof(TrieType) + sizeof(UInt64) + dfuds_.size()
		+ labels_.size() + is_terminals_.size();
}

void DfudsTrie::clear()
{
	num_keys_ = 0;
	dfuds_.clear();
	labels_.clear();
	is_terminals_.clear();
}

void DfudsTrie::swap(DfudsTrie *target)
{
	std::swap(num_keys_, target->num_keys_);
	dfuds_.swap(&target->dfuds_);
	labels_.swap(&target->labels_);
	is_terminals_.swap(&target->is_terminals_);
}

const void *DfudsTrie::map(Mapper mapper, bool checks_type)
{
	DfudsTrie temp;

	if (checks_type)
	{
		if (*mapper.map<TrieType>() != type())
			TAIJU_THROW("failed to map DfudsTrie: wrong TrieType");
	}

	temp.num_keys_ = *mapper.map<UInt64>();
	mapper = temp.dfuds_.map(mapper);
	mapper = temp.labels_.map(mapper);
	mapper = temp.is_terminals_.map(mapper);

	swap(&temp);

	return mapper.address();
}

void DfudsTrie::read(Reader reader, bool checks_type)
{
	DfudsTrie temp;

	if (checks_type)
	{
		TrieType trie_type;
		reader.read(&trie_type);
		if (trie_type != type())
			TAIJU_THROW("failed to read DfudsTrie: wrong TrieType");
	}

	reader.read(&temp.num_keys_);
	temp.dfuds_.read(reader);
	temp.labels_.read(reader);
	temp.is_terminals_.read(reader);

	swap(&temp);
}

void DfudsTrie::write(Writer writer) const
{
	writer.write(num_keys_);
	dfuds_.write(writer);
	labels_.write(writer);
	is_terminals_.write(writer);
}

}  // namespace taiju
