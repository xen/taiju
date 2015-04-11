## はじめに ##

libtaiju では，エラーが発生したとき，`taiju/exception.h` で定義されている
例外 `taiju::Exception` を投げるようになってます．
`taiju::Exception` は `std::exception` を継承しているため，
`const std::exception &` として受け取ることが可能で，
例外が発生した位置やメッセージは `taiju::Exception::what()` で確認できます．


---


## 例外クラス ##

```
class Exception : public std::exception
{
public:
  explicit Exception(const char *msg = NULL);
  Exception(const Exception &rhs) throw();
  virtual ~Exception() throw();

  virtual const char *what() const throw();
};
```


---


## マクロ ##

```
#define TAIJU_THROW(msg) ...
```

`TAIJU_THROW()` は例外を投げるためのマクロで，
ファイル名 `__FILE__`，行番号 `__LINE__` の後に
`msg` をつなげた文字列を生成し，
その文字列を持たせた `taiju::Exception` を例外として投げます．

文字列を連結するため，`msg` は文字列定数のみを受け付けます．

```
#ifdef TAIJU_DEBUG
#define TAIJU_ASSERT(cond) ...
#else
#define TAIJU_ASSERT(cond)
#endif
```

`TAIJU_ASSERT()` は `TAIJU_DEBUG` が定義されている場合にのみ
有効なマクロで，条件 `cond` が `false` であれば例外を投げます．
`assert()` の代わりとして利用しています．