#ifndef TAIJU_TRIE_NODE_H
#define TAIJU_TRIE_NODE_H

#include "int-types.h"

namespace taiju {

class TrieNode
{
public:
	class WeightComparer
	{
	public:
		bool operator()(const TrieNode &lhs, const TrieNode &rhs) const
		{
			if (lhs.weight() != rhs.weight())
				return lhs.weight() > rhs.weight();
			return lhs.label() < rhs.label();
		}
	};

	TrieNode() : child_(0), sibling_(0), has_child_(false),
		has_sibling_(false), label_('\0'), is_terminal_(false),
		weight_(0.0) {}

	UInt64 child() const { return child_; }
	UInt64 sibling() const { return sibling_; }
	bool has_child() const { return has_child_; }
	bool has_sibling() const { return has_sibling_; }
	UInt8 label() const { return label_; }
	bool is_terminal() const { return is_terminal_; }
	double weight() const { return weight_; }

	void set_child(UInt64 child) { child_ = child; }
	void set_sibling(UInt64 sibling) { sibling_ = sibling; }
	void set_has_child(bool has_child) { has_child_ = has_child; }
	void set_has_sibling(bool has_sibling) { has_sibling_ = has_sibling; }
	void set_label(UInt8 label) { label_ = label; }
	void set_is_terminal(bool is_terminal) { is_terminal_ = is_terminal; }
	void set_weight(double weight) { weight_ = weight; }

private:
	UInt64 child_;
	UInt64 sibling_;
	bool has_child_;
	bool has_sibling_;
	UInt8 label_;
	bool is_terminal_;
	double weight_;

	// Copyable.
};

}  // namespace taiju

#endif  // TAIJU_TRIE_NODE_H
