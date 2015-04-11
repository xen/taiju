## はじめに ##

libtaiju では，大規模なトライを構築するため，
一時ファイルを利用することでメモリ消費を抑えています．
`taiju/temp-file.h` で定義されているクラス `taiju::TempFile` は，
一時ファイルの作成や入出力をサポートします．


---


## クラスの定義 ##

ファイルの操作に関しては，`std::fread()`, `std::fwrite()`,
`std::fseek()`, `std::ftell()` と同等の機能を提供しています．
作成した一時ファイルは自動的に削除するようになっていますが，
Windows 環境では作成したファイルのパスを
`::unlink()` で無効にするという手段が使えないため，
異常終了した場合などは，一時ファイルが残ってしまうことがあります．

```
class TempFile
{
public:
 	TempFile();
	~TempFile();

	bool is_open() const;

	void open(UInt64 buf_size = 0);
	void close();

	void read(void *buf, UInt64 size = 1);
	template <typename T>
	void read(T *buf, UInt64 size = 1);

	void write(const void *buf, UInt64 size);
	template <typename T>
	void write(const T *buf, UInt64 size);

	template <typename T>
	void write(const T &buf);

	// Disallows writing a pointer.
	template <typename T>
	void write(const T *buf);

	void flush();

	UInt64 tell();
	void seek(Int64 offset = 0, int whence = SEEK_SET);

	void clear();
	void swap(TempFile *target);
};
```


---


### 一時ファイルを作成する ###

```
void open(UInt64 buf_size = 0);
```

一時ファイルを作成し，内部バッファのサイズ `buf_size` を設定します．
`buf_size` が `0` のときは，
デフォルト値 `taiju::TEMP_FILE_DEFAULT_BUF_SIZE` を使用します．
可能であればファイルのパスを無効にし，一時ファイルが残ったり，
他のプロセスによって操作されたりすることを防ぎます．

### 一時ファイルを閉じる ###

```
void close();
```

一時ファイルを閉じ，必要であればファイルの削除をおこないます．
エラーによりアプリケーションが終了した場合，
一時ファイルが残ってしまう可能性があります．

### 一時ファイルから読み込む ###

```
void read(void *buf, UInt64 size = 1);
template <typename T>
void read(T *buf, UInt64 size = 1);
```

一時ファイルからデータを読み込みます．
指定されたサイズの読み込みに失敗した場合は例外を投げます．

### 一時ファイルに書き込む ###

```
void write(const void *buf, UInt64 size);
template <typename T>
void write(const T *buf, UInt64 size);

template <typename T>
void write(const T &buf);
```

一時ファイルにデータを書き込みます．
指定されたサイズの書き込みに失敗した場合は例外を投げます．

### 内部バッファをフラッシュする ###

```
void flush();
```

内部バッファに書き込み待ちのデータがあれば，ファイルに反映させます．

### ファイル位置を取得・変更する ###

```
UInt64 tell();
void seek(Int64 offset = 0, int whence = SEEK_SET);
```

`taiju::TempFile::tell()` はファイル位置を返します．
一方，`taiju::TempFile::seek()` はファイル位置を設定します．
`whence` には，`SEEK_SET`, `SEEK_CUR`, `SEEK_END` を指定できます．
それぞれの意味は，`std::fseek()` に指定した場合と同様です．


---


## 一時ファイルの操作方法 ##

一時ファイルを作成する簡単な方法は，`std::tmpfile()` を用いることです．
しかし，Windows Vista や 7 においては，
ドライブのルートディレクトリに一時ファイルを作成するという
`std::tmpfile()` の動作が管理者権限を要求するため，別の方法が必要となります．

### Windows の場合 ###

`taiju::TempFile::open()` の実装は以下のようになっています．

  1. `::_tempnam()` を用いて一時ファイルのパスを生成します．
  1. `std::fopen()` あるいは `::fopen_s()` を用いて一時ファイルを作成します．
  1. `std::setvbuf()` を用いて内部バッファのサイズを設定します．

`taiju::TempFile::close()` の実装は以下のようになっています．

  1. `std::fclose()` を用いて一時ファイルを閉じます．
  1. `std::remove()` を用いて一時ファイルを削除します．

### Windows 以外の場合 ###

`taiju::TempFile::open()` の実装は以下のようになっています．

  1. `std::tempfile()` を用いて一時ファイルを作成します．
  1. `std::setvbuf()` を用いて内部バッファのサイズを設定します．

`taiju::TempFile::close()` の実装は以下のようになっています．

  1. `std::fclose()` を用いて一時ファイルを閉じます．

`std::tempfile()` は作成したファイルのパスを `::unlink()` で無効にするため，
明示的にファイルを削除する必要がありません．