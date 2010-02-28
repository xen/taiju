#include <taiju/bp-trie-converter.h>

namespace taiju {

BpTrieConverter::BpTrieConverter() : num_keys_(0), bp_(),
	labels_(), is_terminals_() {}

UInt64 BpTrieConverter::size() const
{
	return sizeof(TrieType) + sizeof(UInt64) + bp_.size()
		+ labels_.size() + is_terminals_.size();
}

void BpTrieConverter::convert(const TrieBase &trie)
{
	BpTrieConverter temp;

	temp.open();
	temp.build(trie);

	swap(&temp);
}

void BpTrieConverter::swap(BpTrieConverter *target)
{
	std::swap(num_keys_, target->num_keys_);
	bp_.swap(&target->bp_);
	labels_.swap(&target->labels_);
	is_terminals_.swap(&target->is_terminals_);
}

void BpTrieConverter::write(Writer writer)
{
	finish();

	writer.write(type());
	writer.write(num_keys_);
	bp_.write(writer);
	labels_.write(writer);
	is_terminals_.write(writer);
}

void BpTrieConverter::open()
{
	BpTrieConverter temp;

	temp.bp_.open(WITH_SELECT_INDEX);
	temp.labels_.open();
	temp.is_terminals_.open(ONLY_RANK_INDEX);

	swap(&temp);
}

void BpTrieConverter::close()
{
	num_keys_ = 0;
	bp_.clear();
	labels_.clear();
	is_terminals_.clear();
}

void BpTrieConverter::build(const TrieBase &trie)
{
	num_keys_ = trie.num_keys();
	build(trie, trie.root());
	bp_.append(false);
}

void BpTrieConverter::build(const TrieBase &trie, UInt64 id)
{
	labels_.append(trie.label(id));
	is_terminals_.append(trie.is_terminal(id));

	bp_.append(true);
	if (trie.child(id, &id))
	{
		build(trie, id);
		while (trie.sibling(id, &id))
			build(trie, id);
	}
	bp_.append(false);
}

void BpTrieConverter::finish()
{
	bp_.finish();
	labels_.finish();
	is_terminals_.finish();
}

}  // namespace taiju
