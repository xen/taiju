#include <taiju/pods-trie.h>

namespace taiju {

PodsTrie::PodsTrie() : num_keys_(0), child_diffs_(), has_siblings_(),
	labels_(), is_terminals_() {}

UInt64 PodsTrie::size() const
{
	return sizeof(TrieType) + sizeof(UInt64) + child_diffs_.size()
		+ has_siblings_.size() + labels_.size() + is_terminals_.size();
}

void PodsTrie::clear()
{
	num_keys_ = 0;
	child_diffs_.clear();
	has_siblings_.clear();
	labels_.clear();
	is_terminals_.clear();
}

void PodsTrie::swap(PodsTrie *target)
{
	std::swap(num_keys_, target->num_keys_);
	child_diffs_.swap(&target->child_diffs_);
	has_siblings_.swap(&target->has_siblings_);
	labels_.swap(&target->labels_);
	is_terminals_.swap(&target->is_terminals_);
}

const void *PodsTrie::map(Mapper mapper, bool checks_type)
{
	PodsTrie temp;

	if (checks_type)
	{
		if (*mapper.map<TrieType>() != type())
			TAIJU_THROW("failed to map PodsTrie: wrong TrieType");
	}

	temp.num_keys_ = *mapper.map<UInt64>();
	mapper = temp.child_diffs_.map(mapper);
	mapper = temp.has_siblings_.map(mapper);
	mapper = temp.labels_.map(mapper);
	mapper = temp.is_terminals_.map(mapper);

	swap(&temp);

	return mapper.address();
}

void PodsTrie::read(Reader reader, bool checks_type)
{
	PodsTrie temp;

	if (checks_type)
	{
		TrieType trie_type;
		reader.read(&trie_type);
		if (trie_type != type())
			TAIJU_THROW("failed to read PodsTrie: wrong TrieType");
	}

	reader.read(&temp.num_keys_);
	temp.child_diffs_.read(reader);
	temp.has_siblings_.read(reader);
	temp.labels_.read(reader);
	temp.is_terminals_.read(reader);

	swap(&temp);
}

void PodsTrie::write(Writer writer) const
{
	writer.write(num_keys_);
	child_diffs_.write(writer);
	has_siblings_.write(writer);
	labels_.write(writer);
	is_terminals_.write(writer);
}

}  // namespace taiju
