#include <taiju/dfuds-trie-converter.h>

namespace taiju {

DfudsTrieConverter::DfudsTrieConverter() : num_keys_(0), dfuds_(),
	labels_(), is_terminals_() {}

UInt64 DfudsTrieConverter::size() const
{
	return sizeof(TrieType) + sizeof(UInt64) + dfuds_.size()
		+ labels_.size() + is_terminals_.size();
}

void DfudsTrieConverter::convert(const TrieBase &trie)
{
	DfudsTrieConverter temp;

	temp.open();
	temp.build(trie);

	swap(&temp);
}

void DfudsTrieConverter::swap(DfudsTrieConverter *target)
{
	std::swap(num_keys_, target->num_keys_);
	dfuds_.swap(&target->dfuds_);
	labels_.swap(&target->labels_);
	is_terminals_.swap(&target->is_terminals_);
}

void DfudsTrieConverter::write(Writer writer)
{
	finish();

	writer.write(type());
	writer.write(num_keys_);
	dfuds_.write(writer);
	labels_.write(writer);
	is_terminals_.write(writer);
}

void DfudsTrieConverter::open()
{
	DfudsTrieConverter temp;

	temp.dfuds_.open(WITH_SELECT_INDEX);
	temp.labels_.open();
	temp.is_terminals_.open(ONLY_RANK_INDEX);

	swap(&temp);
}

void DfudsTrieConverter::close()
{
	num_keys_ = 0;
	dfuds_.clear();
	labels_.clear();
	is_terminals_.clear();
}

void DfudsTrieConverter::build(const TrieBase &trie)
{
	num_keys_ = trie.num_keys();

	dfuds_.append(false);

	dfuds_.append(true);
	dfuds_.append(false);

	labels_.append(trie.label(trie.root()));
	is_terminals_.append(trie.is_terminal(trie.root()));

	UInt64 stack[256];
	build(trie, trie.root(), stack);

	dfuds_.append(false);
}

void DfudsTrieConverter::build(const TrieBase &trie, UInt64 id, UInt64 *stack)
{
	UInt64 child_id;
	if (!trie.child(id, &child_id))
	{
		dfuds_.append(false);
		return;
	}

	UInt64 num_children = 0;
	stack[num_children++] = child_id;
	while (trie.sibling(child_id, &child_id))
		stack[num_children++] = child_id;

	for (UInt64 i = num_children; i > 0; --i)
	{
		dfuds_.append(true);
		labels_.append(trie.label(stack[i - 1]));
		is_terminals_.append(trie.is_terminal(stack[i - 1]));
	}
	dfuds_.append(false);

	id = stack[0];
	do
	{
		build(trie, id, stack);
	} while (trie.sibling(id, &id));
}

void DfudsTrieConverter::finish()
{
	dfuds_.finish();
	labels_.finish();
	is_terminals_.finish();
}

}  // namespace taiju
