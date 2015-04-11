## はじめに ##

このドキュメントでは，libtaiju を構成するモジュールを，
トライを操作する主要なモジュールと補助的なモジュールに分けて，
簡単に説明します．


---


## 主要なモジュール ##

### トライの構築 ###

libtaiju では複数のトライをサポートしていますが，
キー集合からの構築が可能なのは PODS トライのみです．
そして，キー集合から PODS トライを構築するためのクラスが
`taiju::PodsTrieBuilder` です．
また，`taiju::PodsTrieBuilder` の別名として，
`taiju::TrieBuilder` を定義しています．
PODS トライから別の種類のトライを構築するには，次に説明するインタフェース
`taiju::TrieConverterBase` を継承するクラスを用います．

  * [PodsTrieBuilder](PodsTrieBuilder.md)
    * キー集合からトライを構築するためのクラスです．

### インタフェース ###

異なる種類のトライでも同じ方法で扱えるように，
トライの探索・検索用インタフェース `taiju::TrieBase` と，
構築用インタフェース `taiju::TrieConverterBase` を提供しています．
インスタンスの生成については，次に説明するファクトリクラスが便利です．

  * [TrieBase](TrieBase.md)
    * トライの探索・検索用インタフェースです．
  * [TrieConverterBase](TrieConverterBase.md)
    * トライの構築用インタフェースです．
    * 既存のトライから異なる種類のトライを構築する用途で使います．

### ファクトリ ###

トライの種類に応じてクラスを明示的に使い分けるのは苦労するので，
引数に応じたクラスを生成し，
インタフェースのポインタを返すモジュールを提供しています．
`taiju::TrieFactory` は，トライをマップしたり
ファイルから読み込んだりする場合に有用で，
ヘッダからトライの種類を自動的に判別するようになっています．
`taiju::TrieConverterFactory` は，
単純にインスタンスを生成するだけになっています．

  * [TrieFactory](TrieFactory.md)
    * トライのマップや読み込みに便利なクラスです．
  * [TrieConverterFactory](TrieConverterFactory.md)
    * トライの構築用インタフェースを継承するクラスのインスタンスを生成します．
    * 構築するトライの種類を容易に切り替えることができます．


---


## 補助的なモジュール ##

### 整数型と定数 ###

libtaiju では，サイズによる整数の使い分けが多いため，
符号あり整数 `Int8`, `Int64` や符号なし整数 `UInt8`, `UInt64` という
別名を定義して用いるようにしています．
また，整数の別名を定義しているヘッダ `taiju/int-types.h` において，
トライのパラメータ，あるいは libtaiju のパラメータとなる定数も管理しています．

  * [IntTypes](IntTypes.md)
    * サイズを明示した整数型を定義しています．
    * モジュールや libtaiju 全体のパラメータとなる定数を定義しています．

### 例外 ###

libtaiju のモジュールは，何かに失敗したとき，
基本的に例外を投げるようになっています．
その例外 `taiju::Exception` は，`std::exception` を継承しているので，
メンバ関数 `what()` を用いてエラーメッセージを取り出すことができます．

  * [Exception](Exception.md)
    * 例外のクラスを定義しています．
    * 行番号やファイル名をエラーメッセージに付加した例外を投げるためのマクロを定義しています．

### コマンドライン引数 ###

トライの性能を左右する主要なパラメータとして，
トライの種類 `taiju::TrieType` と
ノードの配置順序 `taiju::NodeOrder` があります．
そして，これらの切り替えを容易にするために，
コマンドライン引数からオプション `--taiju-trie-type` と `--taiju-node-order`
他を切り出すクラス `taiju::BuilderConfig` を提供しています．

  * [BuilderConfig](BuilderConfig.md)
    * libtaiju に対するコマンドラインオプションを解釈するためのクラスです．

### I/O 補助 ###

大規模なデータの入力を補助したり，
環境による実装の切り替えを隠蔽したりするクラスを提供しています．
`taiju::LineReader` は行単位の入力を補助するためのクラスで，
内部バッファを用いることにより，
`std::getline()` などより高速な入力を実現しています．
`taiju::FileMap` は Memory-mapped I/O の実装を隠蔽するためのクラスで，
環境により Windows API と `::mmap()` を使い分けるようになっています．
`taiju::TempFile` は一時ファイルの実装を隠蔽するためのクラスで，
環境により `std::fopen()` や `std::tmpfile()` を使い分けるようになっています．

  * [LineReader](LineReader.md)
    * ストリームからの行単位の入力を補助するためのクラスです．
  * [FileMap](FileMap.md)
    * Memory-mapped I/O の利用を補助するためのクラスです．
  * [TempFile](TempFile.md)
    * 一時ファイルの操作を補助するためのクラスです．