#ifndef TAIJU_TRIE_METHOD_H
#define TAIJU_TRIE_METHOD_H

#include "int-types.h"

namespace taiju {

class TrieMethod
{
public:
	template <typename T>
	static bool find(const T &trie, const void *ptr, UInt64 size,
		UInt64 *key_id_ptr = NULL);

	template <typename T>
	static bool follow(const T & trie, UInt64 id, const void *ptr,
		UInt64 size, UInt64 *id_ptr = NULL);

	template <typename T>
	static bool follow(const T &trie, UInt64 id, UInt8 label,
		UInt64 *id_ptr = NULL);

private:
	// Disallows instantiation.
	TrieMethod();
	~TrieMethod();

	// Disallows copies.
	TrieMethod(const TrieMethod &);
	TrieMethod &operator=(const TrieMethod &);
};

template <typename T>
inline bool TrieMethod::find(const T &trie, const void *ptr, UInt64 size,
	UInt64 *key_id_ptr)
{
	UInt64 id;
	if (!trie.follow(trie.root(), ptr, size, &id))
		return false;

	if (!trie.is_terminal(id))
		return false;

	if (key_id_ptr != NULL)
		*key_id_ptr = trie.key_id(id);
	return true;
}

template <typename T>
inline bool TrieMethod::follow(const T & trie, UInt64 id, const void *ptr,
	UInt64 size, UInt64 *id_ptr)
{
	const UInt8 *key = static_cast<const UInt8 *>(ptr);

	for (UInt64 i = 0; i < size; ++i)
	{
		if (!trie.follow(id, key[i], &id))
			return false;
	}

	if (id_ptr != NULL)
		*id_ptr = id;
	return true;
}

template <typename T>
inline bool TrieMethod::follow(const T &trie, UInt64 id, UInt8 label,
	UInt64 *id_ptr)
{
	if (!trie.child(id, &id))
		return false;

	if (trie.label(id) == label)
	{
		if (id_ptr != NULL)
			*id_ptr = id;
		return true;
	}

	while (trie.sibling(id, &id))
	{
		if (trie.label(id) == label)
		{
			if (id_ptr != NULL)
				*id_ptr = id;
			return true;
		}
	}

	return false;
}

}  // namespace taiju

#endif  // TAIJU_TRIE_METHOD_H
