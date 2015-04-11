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