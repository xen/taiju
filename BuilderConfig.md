## はじめに ##

libtaiju を用いるコマンドにおいて，
トライの種類やノードの配置順序を容易に切り替えられるように，
--taiju- で始まるコマンドラインオプションを解釈するためのクラス
`taiju::BuilderConfig` を `taiju/builder-config.h` で定義しています．


---


## クラスの定義 ##

オブジェクトを作成した時点では，すべてデフォルト値です．
`taiju::BuilderConfig::parse()` に `argc`, `argv` を渡せば，
--taiju- で始まるオプションのみを取り出すことができます．

```
class BuilderConfig
{
public:
  BuilderConfig();
  ~BuilderConfig();

  TrieType trie_type() const;
  IndexType index_type() const;
  NodeOrder node_order() const;

  void set_trie_type(TrieType trie_type);
  void set_index_type(IndexType index_type);
  void set_node_order(NodeOrder node_order);

  void parse(int argc, char **argv, int *argc_ptr = NULL);

  void clear();
  void swap(BuilderConfig *target);

  static const char *help();
```


---


### アクセサ ###

`taiju::BuilderConfig` で設定できる内容は，トライの種類 `taiju::TrieType`，
ビットベクターの索引種別 `taiju::IndexType`，
ノードの配置順序 `taiju::IndexType` で，
それぞれにアクセスするための関数が定義されています．

```
TrieType trie_type() const;
IndexType index_type() const;
NodeOrder node_order() const;

void set_trie_type(TrieType trie_type);
void set_index_type(IndexType index_type);
void set_node_order(NodeOrder node_order);
```

### コマンド引数の解釈 ###

コマンド引数を受け取り，--taiju- で始まるオプションのみを切り出します．
該当するオプションは `argv` から取り除かれるため，
残りの引数を用いる場合，引数の数を `argc_ptr` で受け取る必要があります．
--taiju- で始まる不正なオプションがあれば例外を投げます．

```
void parse(int argc, char **argv, int *argc_ptr = NULL);
```

指定可能なオプションは以下の通りです．
設定の詳細については，[IntTypes](IntTypes.md) を参照してください．

  * Options:
    * --taiju-trie-type arg --- choose one of the following trie types
      * `PODS_TRIE`: Post-Order Difference Sequence
      * `LOB_TRIE`: Level-Order Bitmap
      * `LOUDS_TRIE`: Level-Order Unary Degree Sequence
      * `PLOUDS_TRIE`: Partitioned representation of LOUDS
      * `BP_TRIE`: Balanced Parentheses
      * `DFUDS_TRIE`: Depth-First Unary Degree Sequence
      * `DEFAULT`: `PODS_TRIE`
    * --taiju-index-type arg --- index type of BitVector
      * `ONLY_RANK_INDEX`: use index only for rank()
      * `WITH_SELECT_INDEX`: use extra index for accelerating select()
      * `DEFAULT`: `ONLY_RANK_INDEX`
    * --taiju-node-order arg --- arrangement of nodes
      * `ASCENDING_LABEL_ORDER`: 'a' - 'z'
      * `DESCENDING_LABEL_ORDER`: 'z' - 'a'
      * `TOTAL_WEIGHT_ORDER`: descending order of the total weight
      * `MAX_WEIGHT_ORDER`: descending order of the maximum weight
      * `RANDOM_ORDER`: shuffle nodes in random
      * `LABEL_ORDER`: `ASCENDING_LABEL_ORDER`
      * `DEFAULT`: `LABEL_ORDER`

### 使い方の表示 ###

使い方として表示できる文字列を返します．

```
static const char *help();
```

`taiju::BuilderConfig::help()` の返す文字列を
`std::cout` や `std::cerr` に渡せば，
指定できるオプションの一覧が表示されます．