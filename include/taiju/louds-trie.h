#ifndef TAIJU_LOUDS_TRIE_H
#define TAIJU_LOUDS_TRIE_H

#include "const-bit-vector.h"
#include "trie-base.h"
#include "trie-method.h"

namespace taiju {

class LoudsTrie : public TrieBase
{
public:
	LoudsTrie() : num_keys_(0), louds_(), labels_(), is_terminals_() {}
	~LoudsTrie() { clear(); }

	TrieType type() const { return LOUDS_TRIE; }
	const char *type_name() const { return "LoudsTrie"; }

	UInt64 root() const { return 0; }
	bool child(UInt64 id, UInt64 *id_ptr = NULL) const;
	bool sibling(UInt64 id, UInt64 *id_ptr = NULL) const;
	UInt8 label(UInt64 id) const { return labels_[louds_.rank(id) - 1]; }
	bool is_terminal(UInt64 id) const
	{ return is_terminals_[louds_.rank(id) - 1]; }
	UInt64 key_id(UInt64 id) const
	{ return is_terminals_.rank(louds_.rank(id) - 1) - 1; }

	bool empty() const { return num_keys_ == 0; }
	UInt64 num_keys() const { return num_keys_; }
	UInt64 num_nodes() const { return labels_.num_objs(); }
	UInt64 size() const;

	bool find(const void *ptr, UInt64 size, UInt64 *key_id_ptr = NULL) const
	{ return TrieMethod::find(*this, ptr, size, key_id_ptr); }
	bool follow(UInt64 id, const void *ptr, UInt64 size,
		UInt64 *id_ptr = NULL) const
	{ return TrieMethod::follow(*this, id, ptr, size, id_ptr); }
	bool follow(UInt64 id, UInt8 label, UInt64 *id_ptr = NULL) const;

	void clear();
	void swap(LoudsTrie *target);
	const void *map(Mapper mapper, bool checks_type = true);
	void read(Reader reader, bool checks_type = true);
	void write(Writer writer) const;

private:
	UInt64 num_keys_;
	ConstBitVector louds_;
	ConstVector<UInt8> labels_;
	ConstBitVector is_terminals_;

	// Disallows copies.
	LoudsTrie(const LoudsTrie &);
	LoudsTrie &operator=(const LoudsTrie &);
};

inline bool LoudsTrie::child(UInt64 id, UInt64 *id_ptr) const
{
	UInt64 child_id = louds_.select(louds_.rank(id)) + 1;
	if (!louds_[child_id])
		return false;

	if (id_ptr != NULL)
		*id_ptr = child_id;
	return true;
}

inline bool LoudsTrie::sibling(UInt64 id, UInt64 *id_ptr) const
{
	if (!louds_[id + 1])
		return false;

	if (id_ptr != NULL)
		*id_ptr = id + 1;
	return true;
}

inline bool LoudsTrie::follow(UInt64 id, UInt8 label, UInt64 *id_ptr) const
{
	if (!child(id, &id))
		return false;

	UInt64 node_id = louds_.rank(id) - 1;
	do
	{
		if (labels_[node_id] == label)
		{
			if (id_ptr != NULL)
				*id_ptr = id;
			return true;
		}
		++node_id;
	} while (sibling(id, &id));

	return false;
}

}  // namespace taiju

#endif  // TAIJU_LOUDS_TRIE_H
