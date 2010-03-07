#include <taiju/pods-trie-converter.h>

namespace taiju {

PodsTrieConverter::PodsTrieConverter() : num_keys_(0), child_diffs_(),
	has_siblings_(), labels_(), is_terminals_() {}

UInt64 PodsTrieConverter::size() const
{
	return sizeof(TrieType) + sizeof(UInt64) + child_diffs_.size()
		+ has_siblings_.size() + labels_.size() + is_terminals_.size();
}

void PodsTrieConverter::convert(const TrieBase &trie)
{
	PodsTrieConverter temp;

	temp.open();

	temp.build(trie);
	temp.finish();

	swap(&temp);
}

void PodsTrieConverter::swap(PodsTrieConverter *target)
{
	std::swap(num_keys_, target->num_keys_);
	child_diffs_.swap(&target->child_diffs_);
	has_siblings_.swap(&target->has_siblings_);
	labels_.swap(&target->labels_);
	is_terminals_.swap(&target->is_terminals_);
}

void PodsTrieConverter::write(Writer writer)
{
	if (!finished())
		TAIJU_THROW("failed to write PodsTrie: the builder is invalid");

	writer.write(type());
	writer.write(num_keys_);
	child_diffs_.write(writer);
	has_siblings_.write(writer);
	labels_.write(writer);
	is_terminals_.write(writer);
}

void PodsTrieConverter::open()
{
	PodsTrieConverter temp;

	temp.child_diffs_.open();
	temp.has_siblings_.open();
	temp.labels_.open();
	temp.is_terminals_.open(ONLY_RANK_INDEX);

	swap(&temp);
}

void PodsTrieConverter::close()
{
	num_keys_ = 0;
	child_diffs_.clear();
	has_siblings_.clear();
	labels_.clear();
	is_terminals_.clear();
}

void PodsTrieConverter::build(const TrieBase &trie)
{
	num_keys_ = trie.num_keys();
	build(trie, trie.root());
}

UInt64 PodsTrieConverter::build(const TrieBase &trie, UInt64 id)
{
	UInt64 child_id;
	bool has_child = trie.child(id, &child_id);
	if (has_child)
		child_id = build(trie, child_id);

	UInt64 sibling_id;
	bool has_sibling = trie.sibling(id, &sibling_id);
	if (has_sibling)
		build(trie, sibling_id);

	child_diffs_.append(has_child ? (num_nodes() - child_id) : 0);
	has_siblings_.append(has_sibling);
	labels_.append(trie.label(id));
	is_terminals_.append(trie.is_terminal(id));

	return num_nodes() - 1;
}

void PodsTrieConverter::finish()
{
	child_diffs_.finish();
	has_siblings_.finish();
	labels_.finish();
	is_terminals_.finish();
}

}  // namespace taiju
