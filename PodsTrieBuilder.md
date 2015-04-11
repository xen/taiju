## はじめに ##

libtaiju では 6 種類のトライをサポートしていますが，
キー集合から直接構築できるのは PODS トライのみです．
他のトライについては，キー集合から PODS トライを構築した後，
PODS トライを変換するという手順が必要になります．

`taiju/pods-trie-builder.h` で定義されているクラス
`taiju::PodsTrieBuilder` は，
キー集合から PODS トライを構築するためのクラスです．
また，`taiju/trie-builder.h` では，`taiju::PodsTrieBuilder` の別名として
`taiju::TrieBuilder` を定義しています．


---


## クラスの定義 ##

`taiju::PodsTrieBuilder` では，構築中のトライを一時ファイルに保存するため，
メモリの容量を上回るサイズのトライを構築することができます．
トライを構築する手順は，`taiju::PodsTrieBuilder::open()`
で一時ファイルを作成し，登録したい文字列を辞書式昇順に一つずつ
`taiju::PodsTrieBuilder::append()` で追加した後，
`taiju::PodsTrieBuilder::finish()` を呼び出すというものです．
構築したトライは，`taiju::PodsTrieBuilder::write()`
を用いてファイルなどに出力する必要があります．

```
class PodsTrieBuilder
{
public:
  PodsTrieBuilder();
  ~PodsTrieBuilder();

  TrieType type() const;
  const char *type_name() const;

  bool empty() const;
  UInt64 num_keys() const;
  UInt64 num_nodes() const;
  UInt64 size() const;

  bool is_open() const;
  bool finished() const;

  void open(const BuilderConfig &config);
  void open(NodeOrder node_order = DEFAULT_NODE_ORDER);
  void close();

  void append(const void *ptr, UInt64 size, double weight = 1.0);
  void finish();

  void clear();
  void swap(PodsTrieBuilder *target);
  void write(Writer writer);
};
```


---


### 一時ファイルを開く ###

`taiju::PodsTrieBuilder` では，トライを構築する前段階として，
トライの部品を保存しておくための一時ファイルを作成します．
トライの検索時間に強く影響するパラメータであるノードの配置順序は，
この段階で指定する必要があります．

```
void open(const BuilderConfig &config);
void open(NodeOrder node_order = DEFAULT_NODE_ORDER);
```

`taiju::NodeOrder` の詳細については，[IntTypes](IntTypes.md) をご覧ください．

### 一時ファイルを閉じる ###

一時ファイルを閉じて，オブジェクトを初期化します．

```
void close();
```

### キーを追加する ###

構築中のトライに新しいキーを追加します．

```
void append(const void *ptr, UInt64 size, double weight = 1.0);
```

libtaiju では `'\0'` を終端文字として扱わないため，
キーの開始アドレス `ptr `とともに，キーの長さ `size` を指定する必要があります．
キーの重み `weight` は，`taiju::PodsTrieBuilder::open()` で指定した
`taiju::NodeOrder` が `taiju::TOTAL_WEIGHT_ORDER` もしくは
`taiju::MAX_WEIGHT_ORDER` の場合にのみ有効です．

### キーの追加を終了する ###

さらなるキーの追加を禁止して，構築中のトライを完成させます．

```
void finish();
```

`taiju::PodsTrieBuilder::write()` はトライを出力する前に
`taiju::PodsTrieBuilder::finish()` を呼び出します．
そのため，`taiju::PodsTrieBuilder::finish()`
を明示的に呼び出す必要がありません．
明示的に `taiju::PodsTrieBuilder::finish()` を呼び出すことの利点は，
何かのエラーが起きたとき，原因の切り分けに役立つかもしれないという程度です．

### トライを出力する ###

構築したトライをストリームに出力します．
トライが完成していなければ，強制的に完成状態へと移行させます．

```
void write(Writer writer);
```

一時ファイルに保存されているデータを読み込んで連結し，
指定されたストリームに出力します．
引数 `writer` には，`std::ostream` のポインタを渡すことができます．