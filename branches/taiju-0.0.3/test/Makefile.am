AM_CXXFLAGS = -Wall -Weffc++ -DTAIJU_DEBUG -I../include

TESTS = \
	test-line-reader \
	test-int-types \
	test-queue \
	test-vector \
	test-vector-builder \
	test-bit-vector \
	test-balanced-bit-vector \
	test-biased-int-vector \
	test-trie

noinst_PROGRAMS = $(TESTS)

test_line_reader_SOURCES = test-line-reader.cc
test_line_reader_LDADD = ../lib/libtaiju.a

test_int_types_SOURCES = test-int-types.cc
test_int_types_LDADD = ../lib/libtaiju.a

test_queue_SOURCES = test-queue.cc
test_queue_LDADD = ../lib/libtaiju.a

test_vector_SOURCES = test-vector.cc
test_vector_LDADD = ../lib/libtaiju.a

test_vector_builder_SOURCES = test-vector-builder.cc
test_vector_builder_LDADD = ../lib/libtaiju.a

test_bit_vector_SOURCES = test-bit-vector.cc
test_bit_vector_LDADD = ../lib/libtaiju.a

test_balanced_bit_vector_SOURCES = test-balanced-bit-vector.cc
test_balanced_bit_vector_LDADD = ../lib/libtaiju.a

test_biased_int_vector_SOURCES = test-biased-int-vector.cc
test_biased_int_vector_LDADD = ../lib/libtaiju.a

test_trie_SOURCES = test-trie.cc
test_trie_LDADD = ../lib/libtaiju.a
