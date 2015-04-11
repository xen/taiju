## はじめに ##

libtaiju では，6 種類のトライを実装しており，
それぞれに対して別々のクラスを定義しています．
ただし，すべてのクラスが共通のインタフェース
`taiju::TrieBase` を継承しているため，
トライの種類に応じてヘッダやクラスを切り替える必要はありません．
`taiju::TrieBase` は，`taiju/trie-base.h` で定義されています．


---


## クラスの定義 ##

種類が異なればインタフェースも異なるというデザインでは使い勝手が悪くなるので，
`taiju::TrieBase` の仮想関数をオーバーライドする以外，
各トライのクラスに個別のメソッドを極力持たせないようにしています．

```
class TrieBase
{
public:
  TrieBase();
  virtual ~TrieBase();

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
  virtual void write(Writer writer) const;
};
```


---


### アクセサ ###

トライの情報を取得します．

```
virtual UInt64 root() const = 0;
virtual bool child(UInt64 id, UInt64 *id_ptr = NULL) const = 0;
virtual bool sibling(UInt64 id, UInt64 *id_ptr = NULL) const = 0;
virtual UInt8 label(UInt64 id) const = 0;
virtual bool is_terminal(UInt64 id) const = 0;
virtual UInt64 key_id(UInt64 id) const = 0;
```

|**メソッド**|**戻り値**|
|:---------------|:------------|
|`taiju::TrieBase::root()`|ルートの位置を返します．|
|`taiju::TrieBase::child()`|子ノード（first child）が存在するかどうかを返し，存在する場合，その位置を `id_ptr` の参照先に格納します．|
|`taiju::TrieBase::sibling()`|兄弟ノード（next sibling）が存在するかどうかを返し，存在する場合，その位置を `id_ptr` の参照先に格納します．|
|`taiju::TrieBase::label()`|ラベルを返します．|
|`taiju::TrieBase::is_terminal()`|終端ノードかどうかを返します．|
|`taiju::TrieBase::key_id()`|終端ノードであれば，対応するキーの ID を返します．|

### キーを検索する ###

キーがトライに登録されているかどうかを確認できます．

```
virtual bool find(const void *ptr, UInt64 size,
  UInt64 *key_id_ptr = NULL) const = 0;
```

開始アドレス `ptr` と長さ `size` で指定されたキーが
トライに登録されていれば `true` を返します．
このとき，`key_id_ptr` が NULL でなければ，
見つかったキーの ID が `key_id_ptr` の参照先に格納されます．
指定されたキーが登録されていなかった場合は `false` を返します．

### トライを探索する ###

現在位置とラベルを指定することにより，トライ上の経路を辿ることができます．

```
virtual bool follow(UInt64 id, const void *ptr, UInt64 size,
  UInt64 *id_ptr = NULL) const = 0;
virtual bool follow(UInt64 id, UInt8 label,
  UInt64 *id_ptr = NULL) const = 0;
```

`id` には現在位置を指定します．
ルートからの探索をおこなう場合は，
`taiju::TrieBase::root()` の戻り値を渡してください．
ラベル列を指定する場合，開始アドレス `ptr` と長さ `size` を指定します．

探索後の位置は，`id_ptr` の参照先に格納されます．
探索の途中で指定のラベルを持つノードが見つからなければ，
その時点で探索を中止し，`false` を返します．

### トライを復元する ###

保存されているデータから，トライのオブジェクトを復元します．

```
virtual const void *map(Mapper mapper, bool checks_type = true) = 0;
virtual void read(Reader reader, bool checks_type = true) = 0;
```

引数 `mapper` には，任意のポインタを渡すことができます．
引数 `reader` には，`std::istream` のポインタを渡すことができます．

`address_ptr` が `NULL` でなければ，`address_ptr` の参照先には，
マップしたデータの終端が格納されます．
複数のトライがメモリ上で連続するように配置されていれば，
`address_ptr` に格納されたアドレスを次の
`taiju::TrieBase::map()` に渡すという使い方ができます．

`checks_type` が `true` であれば，
先頭にトライの種類が保存されているものとして，
オブジェクトの種類と一致しなければ例外を投げるようになっています．
`checks_type` が `false` であれば，
トライの種類は読み飛ばされたものとして，種類の確認をおこないません．

### トライを出力する ###

トライをストリームに出力します．

```
virtual void write(Writer writer) const;
```

引数 `writer` には，`std::ostream` のポインタを渡すことができます．