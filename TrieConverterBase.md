## はじめに ##

libtaiju では，6 種類のトライに対して，インタフェース
`taiju::TrieConverterBase` を継承する変換用のクラスが個別に用意されています．
`taiju::TrieConverterBase` は `taiju/trie-converter-base.h` で定義されています．


---


## クラスの定義 ##

`taiju::TrieConverterBase` はトライを変換するインタフェースです．
既存のトライから異なる種類のトライを構築する手順は，
`taiju::TrieConverterFactory` を用いてトライの種類に応じた
変換用オブジェクトを生成し，
`taiju::TrieConverterBase::convert()` に既存のトライを渡すだけです．
構築したトライは，`taiju::TrieConverterBase::write()`
を用いてファイルなどに出力する必要があります．

```
class TrieConverterBase
{
public:
  TrieConverterBase();
  virtual ~TrieConverterBase();

  virtual TrieType type() const = 0;
  virtual const char *type_name() const = 0;

  virtual bool empty() const = 0;
  virtual UInt64 num_keys() const = 0;
  virtual UInt64 num_nodes() const = 0;
  virtual UInt64 size() const = 0;

  virtual bool finished() const = 0;

  virtual void convert(const TrieBase &trie) = 0;

  virtual void clear() = 0;
  virtual void write(Writer writer) = 0;
};
```


---


### トライを構築する ###

指定されたトライから，異なる種類のトライを構築します．
同じ種類のトライを構築することも可能ですが，
`taiju::PODS_TRIE` と `taiju::TOTAL_WEIGHT_ORDER`，
もしくは `taiju::PODS_TRIE` と `taiju::MAX_WEIGHT_ORDER`
の組み合わせ以外では意味がありません．

```
virtual void convert(const TrieBase &trie) = 0;
```

最初に，新しいトライを保存するための一時ファイルを作成します．
それから，指定されたトライを探索しつつ，新しいトライを構築します．
途中でエラーが起きた場合は例外を投げます．

### トライを出力する ###

構築したトライをストリームに出力します．

```
virtual void write(Writer writer) = 0;
```

一時ファイルに保存されているデータを読み込んで連結し，
指定されたストリームに出力します．
引数 `writer` には，`std::ostream` のポインタを渡すことができます．