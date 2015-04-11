## はじめに ##

改行を区切りとする大規模なキー集合を高速に読み込むためのクラス
`taiju::LineReader` が `taiju/line-reader.h` で定義されています．
内部バッファを使用するため，他のアクセス方法と併用することはできません．


---


## クラスの定義 ##

`std::getline()` と比べて，高速だけど融通の利かない行単位入力を提供します．

```
class LineReader
{
public:
  LineReader();
  ~LineReader();

  void open(std::istream *stream, std::size_t buf_size = 0);
  void close();

  bool read(const char **line_ptr = NULL, std::size_t *length_ptr = NULL);

  void clear();
  void swap(LineReader *target);
};
```


---


### ストリームと関連付ける ###

```
void open(std::istream *stream, std::size_t buf_size = 0);
```

ストリームとの関連付けをおこない，
`taiju::LineReader::read()` による行単位の入力ができるようにします．
指定されたサイズ `buf_size` より短い行の読み込みが可能になります．
`buf_size` が `0` のときは，
デフォルト値 `taiju::LINE_READER_DEFAULT_BUF_SIZE` を使用します．

### ストリームとの関連付けを解除する ###

```
void close();
```

ストリームとの関連付けを解除し，
内部バッファに割り当てられているメモリを解放します．

### 行を読み込む ###

```
bool read(const char **line_ptr = NULL, std::size_t *length_ptr = NULL);
```

関連付けられているストリームから一行を読み込み，
有効なアドレスを `line_ptr` の参照先に格納し，
行の長さを `length_ptr` の参照先に格納します．
行の読み込みに成功すれば `true` を返し，ストリームの終端に到達した場合など，
行の読み込みに失敗すれば `false` を返します．

`taiju::LineReader::read()` は，
内部バッファに格納されている文字列の開始位置を返します．
`taiju::LineReader::read()` を呼び出した時点で，内部バッファの使い回しにより，
前回の呼び出しで返されたアドレスにはゴミが格納されているものと考えてください．

`taiju::LineReader::open()` において確保されたバッファに
収まらない行を見つけると例外を投げます．