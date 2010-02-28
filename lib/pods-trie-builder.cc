#include <taiju/pods-trie-builder.h>

#include <algorithm>

namespace taiju {

PodsTrieBuilder::PodsTrieBuilder() : node_order_(DEFAULT_NODE_ORDER),
	num_keys_(0), child_diffs_(), has_siblings_(), labels_(),
	is_terminals_(), inters_(), finished_(false) {}

UInt64 PodsTrieBuilder::size() const
{
	return sizeof(TrieType) + sizeof(UInt64) + child_diffs_.size()
		+ has_siblings_.size() + labels_.size() + is_terminals_.size();
}

void PodsTrieBuilder::open(NodeOrder node_order)
{
	PodsTrieBuilder temp;

	switch (node_order)
	{
	case ASCENDING_LABEL_ORDER:
	case DESCENDING_LABEL_ORDER:
	case TOTAL_WEIGHT_ORDER:
	case MAX_WEIGHT_ORDER:
	case RANDOM_ORDER:
		temp.node_order_ = node_order;
		break;
	default:
		TAIJU_THROW("failed to open PodsTrieBuilder: invalid NodeOrder");
	}

	temp.child_diffs_.open();
	temp.has_siblings_.open();
	temp.labels_.open();
	temp.is_terminals_.open(ONLY_RANK_INDEX);

	temp.append_inter(TrieNode());

	swap(&temp);
}

void PodsTrieBuilder::close()
{
	node_order_ = DEFAULT_NODE_ORDER;
	num_keys_ = 0;
	child_diffs_.clear();
	has_siblings_.clear();
	labels_.clear();
	is_terminals_.clear();
	inters_.clear();
	finished_ = false;
}

void PodsTrieBuilder::append(const void *ptr, UInt64 size, double weight)
{
	if (finished())
		TAIJU_THROW("failed to append: the builder is finished");

	append_key(static_cast<const UInt8 *>(ptr), size, weight);
}

void PodsTrieBuilder::finish()
{
	finished_ = true;

	flush(0);

	if (!inters_.empty())
	{
		append_node(inters_[0]);
		inters_.clear();
	}

	child_diffs_.finish();
	has_siblings_.finish();
	labels_.finish();
	is_terminals_.finish();
}

void PodsTrieBuilder::swap(PodsTrieBuilder *target)
{
	std::swap(node_order_, target->node_order_);
	std::swap(num_keys_, target->num_keys_);
	child_diffs_.swap(&target->child_diffs_);
	has_siblings_.swap(&target->has_siblings_);
	labels_.swap(&target->labels_);
	is_terminals_.swap(&target->is_terminals_);
	inters_.swap(&target->inters_);
	std::swap(finished_, target->finished_);
}

void PodsTrieBuilder::write(Writer writer)
{
	finish();

	writer.write(type());
	writer.write(num_keys_);
	child_diffs_.write(writer);
	has_siblings_.write(writer);
	labels_.write(writer);
	is_terminals_.write(writer);
}

void PodsTrieBuilder::append_key(const UInt8 *key, UInt64 length,
	double weight)
{
	UInt64 id = 0;
	for ( ; length > 0; ++key, --length)
	{
		if (!inters_[id].has_child())
			break;
		UInt64 child_id = inters_[id].child();

		UInt8 label = inters_[child_id].label();
		if (*key < label)
			TAIJU_THROW("failed to append: wrong key order");
		else if (*key > label)
		{
			flush(child_id);
			inters_[child_id].set_has_sibling(true);
			break;
		}

		switch (node_order_)
		{
		case ASCENDING_LABEL_ORDER:
		case DESCENDING_LABEL_ORDER:
			break;
		case TOTAL_WEIGHT_ORDER:
			inters_[id].set_weight(inters_[id].weight() + weight);
			break;
		case MAX_WEIGHT_ORDER:
			if (weight > inters_[id].weight())
				inters_[id].set_weight(weight);
			break;
		case RANDOM_ORDER:
			break;
		}
		id = child_id;
	}

	for ( ; length > 0; ++key, --length)
	{
		UInt64 child_id = inters_.num_objs();
		inters_[id].set_has_child(true);
		inters_[id].set_child(child_id);

		TrieNode node;
		node.set_label(*key);
		node.set_weight(weight);
		append_inter(node);

		id = child_id;
	}
	inters_[id].set_is_terminal(true);

	++num_keys_;
}

void PodsTrieBuilder::append_inter(const TrieNode &node)
{
	if (!inters_.push(node))
		TAIJU_THROW("failed to append: Vector::push() failed");
}

void PodsTrieBuilder::append_node(const TrieNode &node)
{
	if (node.has_child())
		child_diffs_.append(labels_.num_objs() - node.child());
	else
		child_diffs_.append(0);
	has_siblings_.append(node.has_sibling());
	labels_.append(node.label());
	is_terminals_.append(node.is_terminal());
}

void PodsTrieBuilder::flush(UInt64 root)
{
	while (root + 1 < inters_.num_objs())
	{
		std::size_t end_id = static_cast<std::size_t>(inters_.num_objs());
		std::size_t begin_id = end_id - 1;
		while (inters_[begin_id - 1].has_sibling())
			--begin_id;

		if (end_id - begin_id > 1)
		{
			inters_[end_id - 1].set_has_sibling(true);
			switch (node_order_)
			{
			case ASCENDING_LABEL_ORDER:
				break;
			case DESCENDING_LABEL_ORDER:
				std::reverse(inters_.begin() + begin_id,
					inters_.begin() + end_id);
				break;
			case TOTAL_WEIGHT_ORDER:
			case MAX_WEIGHT_ORDER:
				std::stable_sort(inters_.begin() + begin_id,
					inters_.begin() + end_id, TrieNode::WeightComparer());
				break;
			case RANDOM_ORDER:
				std::random_shuffle(inters_.begin() + begin_id,
					inters_.begin() + end_id);
				break;
			}
			inters_[end_id - 1].set_has_sibling(false);
		}

		for (UInt64 id = end_id; id > begin_id; --id)
			append_node(inters_[id - 1]);

		inters_.resize(begin_id);
		inters_[begin_id - 1].set_child(labels_.num_objs() - 1);
	}
}

}  // namespace taiju
