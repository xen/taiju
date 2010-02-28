AM_CXXFLAGS = -Wall -Weffc++ -I../include

bin_PROGRAMS = \
	taiju-build \
	taiju-convert \
	taiju-find \
	taiju-test \
	taiju-benchmark

taiju_build_SOURCES = taiju-build.cc
taiju_build_LDADD = ../lib/libtaiju.a

taiju_convert_SOURCES = taiju-convert.cc
taiju_convert_LDADD = ../lib/libtaiju.a

taiju_find_SOURCES = taiju-find.cc
taiju_find_LDADD = ../lib/libtaiju.a

taiju_test_SOURCES = taiju-test.cc
taiju_test_LDADD = ../lib/libtaiju.a

taiju_benchmark_SOURCES = taiju-benchmark.cc
taiju_benchmark_LDADD = ../lib/libtaiju.a

dist_noinst_HEADERS = time-watch.h
