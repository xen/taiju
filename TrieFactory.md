## はじめに ##

libtaiju では，`taiju/trie-factory.h` で定義されているクラス
`taiju::TrieFactory`を用いることにより，
異なる種類のトライでも同じ方法で復元できるようになっています．


---


## クラスの定義 ##

`taiju::TrieFactory` は状態を持たないため，`static` な関数のみを提供します．
`taiju::TrieFactory` が返すオブジェクトは，
いずれも `delete` を用いて破棄する必要があるため，
`std::auto_ptr` や `boost::scoped_ptr` などのスマートポインタに
格納しておくと安心です．

```
class TrieFactory
{
public:
  static TrieBase *Create(TrieType type);
  static TrieBase *map(Mapper mapper, const void **address_ptr = NULL);
  static TrieBase *read(Reader reader);
};
```


---


### オブジェクトの生成 ###

指定された種類のオブジェクトを生成することができます．

```
static TrieBase *Create(TrieType type);
```

`taiju::TrieType` の詳細については，[IntTypes](IntTypes.md) をご覧ください．

### トライのマッピング ###

指定されたアドレスにトライのデータが配置されているものとして，
ヘッダからトライの種類を判別し，トライのオブジェクトを復元することができます．

```
static TrieBase *map(Mapper mapper, const void **address_ptr = NULL);
```

引数 `mapper` には，任意のポインタを渡すことができます．
`address_ptr` が `NULL` でなければ，`address_ptr` の参照先には，
マップしたデータの終端が格納されます．
複数のトライがメモリ上で連続するように配置されていれば，
`address_ptr` に格納されたアドレスを次の
`taiju::TrieFactory::map()` に渡すという使い方ができます．

### トライの読み込み ###

指定されたストリームからデータを読み込み，
トライのオブジェクトを復元することができます．
読み込んだデータからトライの種類を判断するため，
呼び出し側でトライの種類を指示する必要はありません．

```
static TrieBase *read(Reader reader);
```

引数 `reader` には，`std::istream` のポインタを渡すことができます．