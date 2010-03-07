#ifndef TAIJU_BP_TRIE_H
#define TAIJU_BP_TRIE_H

#include "const-balanced-bit-vector.h"
#include "trie-base.h"
#include "trie-method.h"

namespace taiju {

class BpTrie : public TrieBase
{
public:
	BpTrie() : num_keys_(0), bp_(), labels_(), is_terminals_() {}
	~BpTrie() { clear(); }

	TrieType type() const { return BP_TRIE; }
	const char *type_name() const { return "BpTrie"; }

	UInt64 root() const { return 0; }
	bool child(UInt64 id, UInt64 *id_ptr = NULL) const;
	bool sibling(UInt64 id, UInt64 *id_ptr = NULL) const;
	UInt8 label(UInt64 id) const { return labels_[bp_.rank(id) - 1]; }
	bool is_terminal(UInt64 id) const
	{ return is_terminals_[bp_.rank(id) - 1]; }
	UInt64 key_id(UInt64 id) const
	{ return is_terminals_.rank(bp_.rank(id) - 1) - 1; }

	bool empty() const { return num_keys_ == 0; }
	UInt64 num_keys() const { return num_keys_; }
	UInt64 num_nodes() const { return labels_.num_objs(); }
	UInt64 size() const;

	bool find(const void *ptr, UInt64 size, UInt64 *key_id_ptr = NULL) const
	{ return TrieMethod::find(*this, ptr, size, key_id_ptr); }
	bool follow(UInt64 id, const void *ptr, UInt64 size,
		UInt64 *id_ptr = NULL) const
	{ return TrieMethod::follow(*this, id, ptr, size, id_ptr); }
	bool follow(UInt64 id, UInt8 label, UInt64 *id_ptr = NULL) const
	{ return TrieMethod::follow(*this, id, label, id_ptr); }

	void clear();
	void swap(BpTrie *target);
	const void *map(Mapper mapper, bool checks_type = true);
	void read(Reader reader, bool checks_type = true);
	void write(Writer writer) const;

private:
	UInt64 num_keys_;
	ConstBalancedBitVector bp_;
	ConstVector<UInt8> labels_;
	ConstBitVector is_terminals_;

	// Disallows copies.
	BpTrie(const BpTrie &);
	BpTrie &operator=(const BpTrie &);
};

inline bool BpTrie::child(UInt64 id, UInt64 *id_ptr) const
{
	if (!bp_[id + 1])
		return false;

	if (id_ptr != NULL)
		*id_ptr = id + 1;
	return true;
}

inline bool BpTrie::sibling(UInt64 id, UInt64 *id_ptr) const
{
	UInt64 sibling_id = bp_.findclose(id) + 1;
	if (!bp_[sibling_id])
		return false;

	if (id_ptr != NULL)
		*id_ptr = sibling_id;
	return true;
}

}  // namespace taiju

#endif  // TAIJU_BP_TRIE_H
