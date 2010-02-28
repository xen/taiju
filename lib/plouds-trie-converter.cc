#include <taiju/plouds-trie-converter.h>

#include <taiju/queue-file.h>

namespace taiju {

PloudsTrieConverter::PloudsTrieConverter() : num_keys_(0), has_childs_(),
	has_siblings_(), labels_(), is_terminals_() {}

UInt64 PloudsTrieConverter::size() const
{
	return sizeof(TrieType) + sizeof(UInt64) + has_childs_.size()
		+ has_siblings_.size() + labels_.size() + is_terminals_.size();
}

void PloudsTrieConverter::convert(const TrieBase &trie)
{
	PloudsTrieConverter temp;

	temp.open();
	temp.build(trie);

	temp.finish();

	swap(&temp);
}

void PloudsTrieConverter::swap(PloudsTrieConverter *target)
{
	std::swap(num_keys_, target->num_keys_);
	has_childs_.swap(&target->has_childs_);
	has_siblings_.swap(&target->has_siblings_);
	labels_.swap(&target->labels_);
	is_terminals_.swap(&target->is_terminals_);
}

void PloudsTrieConverter::write(Writer writer)
{
	if (!finished())
		TAIJU_THROW("failed to write PloudsTrie: the converter is invalid");

	writer.write(type());
	writer.write(num_keys_);
	has_childs_.write(writer);
	has_siblings_.write(writer);
	labels_.write(writer);
	is_terminals_.write(writer);
}

void PloudsTrieConverter::open()
{
	PloudsTrieConverter temp;

	temp.has_childs_.open(ONLY_RANK_INDEX);
	temp.has_siblings_.open(WITH_SELECT_INDEX);
	temp.labels_.open();
	temp.is_terminals_.open(ONLY_RANK_INDEX);

	swap(&temp);
}

void PloudsTrieConverter::close()
{
	num_keys_ = 0;
	has_childs_.clear();
	has_siblings_.clear();
	labels_.clear();
	is_terminals_.clear();
}

void PloudsTrieConverter::build(const TrieBase &trie)
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
		has_siblings_.append(trie.sibling(id));

		bool has_next = trie.child(id, &id);
		has_childs_.append(has_next);
		while (has_next)
		{
			queue.write(id);
			has_next = trie.sibling(id, &id);
		}
	}
}

void PloudsTrieConverter::finish()
{
	has_childs_.finish();
	has_siblings_.finish();
	labels_.finish();
	is_terminals_.finish();
}

}  // namespace taiju
