#ifndef TAIJU_PLOUDS_TRIE_H
#define TAIJU_PLOUDS_TRIE_H

#include "const-bit-vector.h"
#include "trie-base.h"
#include "trie-method.h"

namespace taiju {

class PloudsTrie : public TrieBase
{
public:
	PloudsTrie();
	~PloudsTrie() { clear(); }

	TrieType type() const { return PLOUDS_TRIE; }
	const char *type_name() const { return "PloudsTrie"; }

	UInt64 root() const { return 0; }
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
	void swap(PloudsTrie *target);
	const void *map(Mapper mapper, bool checks_type = true);
	void read(Reader reader, bool checks_type = true);
	void write(Writer writer) const;

private:
	UInt64 num_keys_;
	ConstBitVector has_childs_;
	ConstBitVector has_siblings_;
	ConstVector<UInt8> labels_;
	ConstBitVector is_terminals_;

	// Disallows copies.
	PloudsTrie(const PloudsTrie &);
	PloudsTrie &operator=(const PloudsTrie &);
};

inline bool PloudsTrie::child(UInt64 id, UInt64 *id_ptr) const
{
	if (!has_childs_[id])
		return false;

	if (id_ptr != NULL)
		*id_ptr = has_siblings_.select(has_childs_.rank(id)) + 1;
	return true;
}

inline bool PloudsTrie::sibling(UInt64 id, UInt64 *id_ptr) const
{
	if (!has_siblings_[id])
		return false;

	if (id_ptr != NULL)
		*id_ptr = id + 1;
	return true;
}

}  // namespace taiju

#endif  // TAIJU_PLOUDS_TRIE_H
