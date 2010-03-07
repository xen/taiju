#include <taiju/louds-trie-converter.h>

#include <taiju/queue-file.h>

namespace taiju {

LoudsTrieConverter::LoudsTrieConverter() : num_keys_(0), louds_(),
	labels_(), is_terminals_() {}

UInt64 LoudsTrieConverter::size() const
{
	return sizeof(TrieType) + sizeof(UInt64) + louds_.size()
		+ labels_.size() + is_terminals_.size();
}

void LoudsTrieConverter::convert(const TrieBase &trie)
{
	LoudsTrieConverter temp;

	temp.open();
	temp.build(trie);

	temp.finish();

	swap(&temp);
}

void LoudsTrieConverter::swap(LoudsTrieConverter *target)
{
	std::swap(num_keys_, target->num_keys_);
	louds_.swap(&target->louds_);
	labels_.swap(&target->labels_);
	is_terminals_.swap(&target->is_terminals_);
}

void LoudsTrieConverter::write(Writer writer)
{
	if (!finished())
		TAIJU_THROW("failed to write LoudsTrie: the builder is invalid");

	writer.write(type());
	writer.write(num_keys_);
	louds_.write(writer);
	labels_.write(writer);
	is_terminals_.write(writer);
}

void LoudsTrieConverter::open()
{
	LoudsTrieConverter temp;

	temp.louds_.open(WITH_SELECT_INDEX);
	temp.labels_.open();
	temp.is_terminals_.open(ONLY_RANK_INDEX);

	swap(&temp);
}

void LoudsTrieConverter::close()
{
	num_keys_ = 0;
	louds_.clear();
	labels_.clear();
	is_terminals_.clear();
}

void LoudsTrieConverter::build(const TrieBase &trie)
{
	num_keys_ = trie.num_keys();

	louds_.append(true);
	louds_.append(false);

	QueueFile queue;
	queue.open();
	queue.write(trie.root());

	UInt64 id;
	while (queue.read(&id))
	{
		labels_.append(trie.label(id));
		is_terminals_.append(trie.is_terminal(id));

		if (trie.child(id, &id))
		{
			queue.write(id);
			louds_.append(true);

			while (trie.sibling(id, &id))
			{
				queue.write(id);
				louds_.append(true);
			}
		}
		louds_.append(false);
	}
}

void LoudsTrieConverter::finish()
{
	louds_.finish();
	labels_.finish();
	is_terminals_.finish();
}

}  // namespace taiju
