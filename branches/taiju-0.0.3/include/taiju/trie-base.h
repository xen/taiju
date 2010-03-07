#ifndef TAIJU_TRIE_BASE_H
#define TAIJU_TRIE_BASE_H

#include "mapper.h"
#include "reader.h"
#include "writer.h"

namespace taiju {

class TrieBase
{
public:
	TrieBase() {}
	virtual ~TrieBase() {}

	virtual TrieType type() const = 0;
	virtual const char *type_name() const = 0;

	virtual UInt64 root() const = 0;
	virtual bool child(UInt64 id, UInt64 *id_ptr = NULL) const = 0;
	virtual bool sibling(UInt64 id, UInt64 *id_ptr = NULL) const = 0;
	virtual UInt8 label(UInt64 id) const = 0;
	virtual bool is_terminal(UInt64 id) const = 0;
	virtual UInt64 key_id(UInt64 id) const = 0;

	virtual bool empty() const = 0;
	virtual UInt64 num_keys() const = 0;
	virtual UInt64 num_nodes() const = 0;
	virtual UInt64 size() const = 0;

	virtual bool find(const void *ptr, UInt64 size,
		UInt64 *key_id_ptr = NULL) const = 0;
	virtual bool follow(UInt64 id, const void *ptr, UInt64 size,
		UInt64 *id_ptr = NULL) const = 0;
	virtual bool follow(UInt64 id, UInt8 label,
		UInt64 *id_ptr = NULL) const = 0;

	virtual void clear() = 0;
	virtual const void *map(Mapper mapper, bool checks_type = true) = 0;
	virtual void read(Reader reader, bool checks_type = true) = 0;
	virtual void write(Writer writer) const = 0;

private:
	// Disallows copies.
	TrieBase(const TrieBase &);
	TrieBase &operator=(const TrieBase &);
};

}  // namespace taiju

#endif  // TAIJU_TRIE_BASE_H
