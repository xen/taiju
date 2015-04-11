## はじめに ##

libtaiju では，引数の変更のみでトライの種類を切り替えられるように，
トライの変換を補助するクラス `taiju::TrieConverterFactory` を
`taiju/trie-converter-factory.h` で定義しています．


---


## クラスの定義 ##

`taiju::TrieConverterFactory` は状態を持たないため，
`static` な関数のみを提供します．

```
class TrieConverterFactory
{
public:
  static TrieConverterBase *Create(const BuilderConfig &config);
  static TrieConverterBase *Create(TrieType type);
};
```


---


### トライ変換用オブジェクトの生成 ###

トライの種類を簡単に切り替えできるようにするため，
引数に応じたクラスのオブジェクトを生成する関数を提供しています．

```
static TrieConverterBase *Create(const BuilderConfig &config);
static TrieConverterBase *Create(TrieType type);
```

`taiju::TrieConverterFactory::Create()` が返すオブジェクトは，
`delete` を用いて破棄する必要があります．
`std::auto_ptr` や `boost::scoped_ptr` などのスマートポインタに
格納しておくと安心です．