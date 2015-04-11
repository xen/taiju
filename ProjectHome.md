## Outline ##

The libtaiju is a C++ library for succinct representations of trie --- Trie is a tree structure of storing strings and succinct tries are suitable for a large set of strings.

## Features ##

The following list shows main features of the libtaiju.

  * The libtaiju is optimized for 64-bit environment.
    * It works even on 32-bit environment, but its performance is worse than on 64-bit environment.
  * The libtaiju supports very large tries.
    * The upper limit of the number of trie nodes is 2<sup>38</sup> or 2<sup>40</sup>.
  * The libtaiju uses temporary files for building a large trie.
    * If `_Win32` or `_Win64` is defined (on Windows), temporary files are created by a combination of `::_tempnam()` and `std::fopen()`.
      * If `_MSC_VER` is also defined (on Visual Studio), `::fopen_s()` is used instead of `std::fopen()`.
    * In other cases (such as on Linux), `std::tmpfile()` is used.

## Representations & References ##

The following is a list of trie representations provided by the libtaiju.

  * PODS: Post-Order Difference Sequence
    * This represention is designed for the libtaiju.
  * LOB: Level-Order Bitmap
    * Guy Jacobson. Space-efficient static trees and graphs. _In Proceedings of FOCS 1989_, pp. 549–554, 1989.
      * http://www.cs.cmu.edu/afs/cs.cmu.edu/project/aladdin/wwwlocal/compression/00063533.pdf
  * LOUDS: Level-Order Unary Degree Sequence
    * Guy Jacobson. Space-efficient static trees and graphs. _In Proceedings of FOCS 1989_, pp. 549–554, 1989.
      * http://www.cs.cmu.edu/afs/cs.cmu.edu/project/aladdin/wwwlocal/compression/00063533.pdf
  * PLOUDS: Partitioned representation of LOUDS
    * O’Neil Delpratt, Naila Rahman, and Rajeev Raman. Engineering the LOUDS succinct tree representation. _In Proceedings of WEA 2006_, pp. 134–145, 2006.
    * O’Neil Delpratt. Space efficient in-memory representation of XML documents. 2008.
      * https://lra.le.ac.uk/bitstream/2381/4805/1/2008DelprattodPhD.pdf
  * BP: Balanced Parentheses
    * Ian J. Munro and Venkatesh Raman. Succinct representation of balanced parentheses and static trees. _SIAM Journal on Computing_, Vol. 31, No. 3, pp. 762–776, 2001.
  * DFUDS: Depth-First Unary Degree Sequence
    * David Benoit, Erik D. Demaine, J. Ian Munro, Rajeev Raman, Venkatesh Raman, and S. Srinivasa Rao. Representing trees of higher degree. _Algorithmica_, Vol. 43, No. 4, pp. 275–292, 2005.

## Name ##

Taiju (大樹) means big tree in Japanese.