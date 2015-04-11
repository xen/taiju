## はじめに ##

libtaiju で使用する整数の型や定数などは，
`taiju/int-types.h` で定義されています．


---


## 整数 ##

32-bit 環境と 64-bit 環境のいずれにおいても 64-bit 整数を用いるため，
`long` ではなく `long long` を用いています．
`long long` が 128-bit 整数と解釈される時代が来ると問題になりますが，
しばらく先になりそうなので大丈夫だと思います．
他に，`taiju::Int16` および `taiju::UInt16` は
使っていないのでほとんど影響はありませんが，
環境によっては `int` を 32-bit 整数と仮定していることが
問題になるかもしれません．

### 符号あり整数 ###

```
typedef signed char Int8;
typedef signed short Int16;
typedef signed int Int32;
typedef signed long long Int64;
```

### 符号なし整数 ###

```
typedef unsigned char UInt8;
typedef unsigned short UInt16;
typedef unsigned int UInt32;
typedef unsigned long long UInt64;
```


---


## 定数 ##

libtaiju を利用するときに使う可能性がある定数は，
`taiju::TrieType`, `taiju::IndexType`, `taiju::NodeOrder` の 3 つです．
ただし，`taiju::IndexType` はビットベクターを単体で
用いるとき以外には使いません．

### トライの種類 ###

`taiju::TrieType` はトライの種類を表す型です．
`taiju::TrieConverterFactory::Create()` の引数として使うことで，
指定した種類のトライを構築できます．

```
enum TrieType
{
  PODS_TRIE,
  LOB_TRIE,
  LOUDS_TRIE,
  PLOUDS_TRIE,
  BP_TRIE,
  DFUDS_TRIE,
  DEFAULT_TRIE_TYPE = PODS_TRIE
};
```

各トライの概要はプロジェクト taiju のホームページにて確認できます．

  * プロジェクト taiju のホームページ
    * http://code.google.com/p/taiju/

### ビットベクターの索引種別 ###

`taiju::BitVectorBuilder::open()` に
`taiju::WITH_SELECT_INDEX` を渡すことにより，#summary メモリマップドファイル．

## はじめに ##

libtaiju では，大規模なトライをメモリ上に展開することなく
利用できるように，ファイルに対するメモリマップド I/O
を補助するクラス `taiju::FileMap` を `taiju/file-map.h` で定義しています．
ただし，ファイルのフォーマットがメモリマップド I/O
に最適化されていないため，性能は大幅に低下します．


---


## クラスの定義 ##

libtaiju におけるメモリマップド I/O の利用目的は，
トライを保存したファイルをメモリ上にマップすることです．
また，ファイルの一部をマップするような機能も必要ないと考え，
ファイル全体をメモリ上にマップするという，
とても単純な機能のみを提供するようにしました．

```
class FileMap
{
public:
  FileMap();
  ~FileMap();

  void open(const char *file_name);
  void close();

  template <typename T>
  const T *pointer() const;
  const void *address() const;
  std::size_t size() const;

  void clear();
  void swap(FileMap *target);
};
```


---


### ファイルを開く ###

```
void open(const char *file_name);
```

指定されたファイルを開き，ファイル全体をメモリ上にマップします．
ファイルを開くのに失敗したり，マップに失敗したりした場合，
例外を投げるようになっています．

### ファイルを閉じる ###

```
void close();
```

ファイルをマップしている場合，そのマップを解除し，ファイルを閉じます．
ファイルをマップしていない場合は，何も起きません．

### アドレスの取得 ###

```
template <typename T>
const T *pointer() const;
const void *address() const;
```

`taiju::FileMap::address()` はマップされた領域のアドレスを返します．
`taiju::FileMap::pointer()` はテンプレート引数により指定された型の
ポインタとしてアドレスを返します．

### ファイルサイズの取得 ###

```
std::size_t size() const;
```

ファイルのサイズをバイト単位で返します．


---


## メモリマップド I/O の方法 ##

`taiju::FileMap` の実装は，Windows とそれ以外で大きく異なります．
具体的には，Windows API を用いるか，`::mmap()` を用いるかという違いです．

### Windows の場合 ###

`taiju::FileMap::open()` の実装は以下のようになっています．

  1. `::_stat64` を用いてファイルのサイズを調べます．
  1. `::CreateFile()` を用いてファイルを開きます．
  1. `::CreateFileMapping()` と `::MapViewOfFile()` を用いてファイル全体をメモリ上にマップします．

`taiju::FileMap::close()` の実装は以下のようになっています．

  1. `::UnmapViewOfFile()` と `::CloseHandle()` を用いてファイルのマップを解除します．
  1. `::CloseHandle()` を用いてファイルを閉じます．

### Windows 以外の場合 ###

`taiju::FileMap::open()` の実装は以下のようになっています．

  1. `::stat()` を用いてファイルのサイズを調べます．
  1. `::open()` を用いてファイルを開きます．
  1. `::mmap()` を用いてファイル全体をメモリ上にマップします．

`taiju::FileMap::close()` の実装は以下のようになっています．

  1. `::munmap()` を用いてファイルのマップを解除します．
  1. `::close()` を用いてファイルを閉じます．
`taiju::ConstBitVector::select()` を高速化できます．

```
enum IndexType
{
  ONLY_RANK_INDEX,
  WITH_SELECT_INDEX,
  DEFAULT_INDEX_TYPE = ONLY_RANK_INDEX
};
```

  * `taiju::ONLY_RANK_INDEX`
    * `rank()` 用の索引のみを付加します．
    * `select()` も利用できますが，時間がかかります．
  * `taiju::WITH_SELECT_INDEX`
    * `rank()` 用の索引に加えて `select()` 用の索引を付加します．

### ノードの配置順序 ###

トライを構築するとき，ノードの配置順序を変更します．
初期設定ではラベル昇順にノードを配置しますが，
出現頻度が高い順にノードを配置することができれば，
検索時間を大幅に短縮することが可能です．

トライの構築において，登録する文字列には個別に重みを指定できます．
デフォルトではすべての文字列に 1.0 を重みとして与えるため，
`taiju::TOTAL_WEIGHT_ORDER` を選択することにより，各文字列の出現頻度が同じと
仮定した場合における，出現頻度が高い順の配置順序になります．

```
enum NodeOrder
{
  ASCENDING_LABEL_ORDER,
  DESCENDING_LABEL_ORDER,
  TOTAL_WEIGHT_ORDER,
  MAX_WEIGHT_ORDER,
  RANDOM_ORDER,
  LABEL_ORDER = ASCENDING_LABEL_ORDER,
  DEFAULT_NODE_ORDER = LABEL_ORDER
};
```

  * `taiju::ASCENDING_LABEL_ORDER`
    * **ラベル昇順**にノードを配置します．
  * `taiju::DESCENDING_LABEL_ORDER`
    * **ラベル降順**にノードを配置します．
  * `taiju::TOTAL_WEIGHT_ORDER`
    * 各ノードから下方に辿って復元可能な文字列に付与されている**重みの合計が降順**になるようにノードを配置します．
  * `taiju::MAX_WEIGHT_ORDER`
    * 各ノードから下方に辿って復元可能な文字列に付与されている**重みの最大値が降順**になるようにノードを配置します．
  * `taiju::RANDOM_ORDER`
    * **ランダム順**にノードを配置します．
    * 並べ替えには，`std::random_shuffle()` を使用します．乱数生成用の関数オブジェクトは指定しないため，必要であれば `std::srand()` による初期化をおこなってください．