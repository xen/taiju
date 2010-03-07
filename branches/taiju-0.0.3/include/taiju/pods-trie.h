#ifndef TAIJU_PODS_TRIE_H
#define TAIJU_PODS_TRIE_H

#include "const-biased-int-vector.h"
#include "trie-base.h"
#include "trie-method.h"

namespace taiju {

class PodsTrie : public TrieBase
{
public:
	PodsTrie();
	~PodsTrie() { clear(); }

	TrieType type() const { return PODS_TRIE; }
	const char *type_name() const { return "PodsTrie"; }

	UInt64 root() const { return num_nodes() - 1; }
	bool child(UInt64 id, UInt64 *id_ptr = NULL) const;
	bool sibling(UInt64 id, UInt64 *id_ptr = NULL) const;
	UInt8 label(UInt64 id) const { return labels_[id]; }
	bool is_terminal(UInt64 id) const { return is_terminals_[id]; }
	UInt64 key_id(UInt64 id) const { return is_terminals_.rank(id) - 1; }

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
	void swap(PodsTrie *target);
	const void *map(Mapper mapper, bool checks_type = true);
	void read(Reader reader, bool checks_type = true);
	void write(Writer writer) const;

private:
	UInt64 num_keys_;
	ConstBiasedIntVector child_diffs_;
	ConstVector<bool> has_siblings_;
	ConstVector<UInt8> labels_;
	ConstBitVector is_terminals_;

	// Disallows copies.
	PodsTrie(const PodsTrie &);
	PodsTrie &operator=(const PodsTrie &);
};

inline bool PodsTrie::child(UInt64 id, UInt64 *id_ptr) const
{
	if (child_diffs_.is_zero(id))
		return false;

	if (id_ptr != NULL)
		*id_ptr = id - child_diffs_.as_non_zero(id);
	return true;
}

inline bool PodsTrie::sibling(UInt64 id, UInt64 *id_ptr) const
{
	if (!has_siblings_[id])
		return false;

	if (id_ptr != NULL)
		*id_ptr = id - 1;
	return true;
}

}  // namespace taiju

#endif  // TAIJU_PODS_TRIE_H
