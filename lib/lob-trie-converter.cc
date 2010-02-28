#include <taiju/lob-trie-converter.h>

#include <taiju/queue-file.h>

namespace taiju {

UInt64 LobTrieConverter::size() const
{
	return sizeof(TrieType) + sizeof(UInt64) + lob_.size()
		+ labels_.size() + is_terminals_.size();
}

void LobTrieConverter::convert(const TrieBase &trie)
{
	LobTrieConverter temp;

	temp.open();
	temp.build(trie);

	temp.finish();

	swap(&temp);
}

void LobTrieConverter::swap(LobTrieConverter *target)
{
	std::swap(num_keys_, target->num_keys_);
	lob_.swap(&target->lob_);
	labels_.swap(&target->labels_);
	is_terminals_.swap(&target->is_terminals_);
}

void LobTrieConverter::write(Writer writer)
{
	if (!finished())
		TAIJU_THROW("failed to write LobTrie: the builder is invalid");

	writer.write(type());
	writer.write(num_keys_);
	lob_.write(writer);
	labels_.write(writer);
	is_terminals_.write(writer);
}

void LobTrieConverter::open()
{
	LobTrieConverter temp;

	temp.lob_.open(ONLY_RANK_INDEX);
	temp.labels_.open();
	temp.is_terminals_.open(ONLY_RANK_INDEX);

	swap(&temp);
}

void LobTrieConverter::close()
{
	num_keys_ = 0;
	lob_.clear();
	labels_.clear();
	is_terminals_.clear();
}

void LobTrieConverter::build(const TrieBase &trie)
{
	num_keys_ = trie.num_keys();

	QueueFile queue;
	queue.open();
	queue.write(trie.root());

	UInt64 id;
	while (queue.read(&id))
	{
		labels_.append(trie.label(id));
		is_terminals_.append(trie.is_terminal(id));

		UInt64 child_id;
		bool has_child = trie.child(id, &child_id);
		lob_.append(has_child);
		if (has_child)
			queue.write(child_id);

		UInt64 sibling_id;
		bool has_sibling = trie.sibling(id, &sibling_id);
		lob_.append(has_sibling);
		if (has_sibling)
			queue.write(sibling_id);
	}
}

void LobTrieConverter::finish()
{
	lob_.finish();
	labels_.finish();
	is_terminals_.finish();
}

}  // namespace taiju
