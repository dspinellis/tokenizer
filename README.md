[![Build Status](https://travis-ci.org/dspinellis/tokenizer.svg?branch=master)](https://travis-ci.org/dspinellis/tokenizer)

# tokenizer

Tokenize source code into integer vectors.

## Build

```
cd src
make
```


## Install

```
cd src
sudo make install
```

## Run

```
tokenizer file.c
tokenizer -l Java -o statement <file.java
```

## Examples of tokenizing "hello world" programs in diverse languages

### C into integers

```
$ curl -s https://raw.githubusercontent.com/leachim6/hello-world/master/c/c.c | tokenizer -l C
35      320     60      2000    46      2001    62      322     2002    40     41       123     2003    40      625     41      59      327     1500    59     125
```

### C into symbols

```
$ curl -s https://raw.githubusercontent.com/leachim6/hello-world/master/c/c.c | tokenizer -l C -t s
# include < ID:2000 . ID:2001 > int ID:2002 ( ) { ID:2003 ( STRING_LITERAL
) ; return 0 ; }
```

### C# into integers

```
$ curl -s https://raw.githubusercontent.com/leachim6/hello-world/master/c/csharp.cs | tokenizer -l "C#"
312     2000    123     360     376     2001    40      41      123     2002   46       2003    46      2004    40      627     41      59      125     125
```

### C# into symbols

```
$ curl -s https://raw.githubusercontent.com/leachim6/hello-world/master/c/csharp.cs | tokenizer -l "C#" -t s
class ID:2000 { static void ID:2001 ( ) { ID:2002 . ID:2003 . ID:2004
( STRING_LITERAL ) ; } }
```

### C# method-only into integers

```
$ curl -s https://raw.githubusercontent.com/leachim6/hello-world/master/c/csharp.cs | tokenizer -l "C#" -o method
123     2002    46      2003    46      2004    40      627     41      59     125
```

### C++ into symbols

```
$ curl -s https://raw.githubusercontent.com/leachim6/hello-world/master/c/c%2B%2B.cpp | tokenizer -l C++ -t s
# include < ID:2000 > LINE_COMMENT using namespace ID:2001 ; int ID:2002
( ) LINE_COMMENT { ID:2003 LSHIFT STRING_LITERAL LSHIFT ID:2004 ;
LINE_COMMENT return 0 ; LINE_COMMENT }
```

### Java into symbols

```
$ curl -s https://raw.githubusercontent.com/leachim6/hello-world/master/j/Java.java | tokenizer -l Java -t s
public class ID:2000 { public static void ID:2001 ( ID:2002 [ ] ID:2003 )
{ ID:2004 . ID:2005 . ID:2006 ( STRING_LITERAL ) ; } }
```

### C++ into code tokens

```
curl -s https://raw.githubusercontent.com/leachim6/hello-world/master/c/c%2B%2B.cpp | tokenizer -l C++ -t c
#
include
<
iostream
>
// ...
using
namespace
std
;
int
main
(
)
// ...
{
cout
<<
"..."
<<
endl
;
// ...
return
0
;
// ...
}
```

## Reference manual
You can read the command's Unix manual page through [this link](https://dspinellis.github.io/manview/?src=https%3A%2F%2Fraw.githubusercontent.com%2Fdspinellis%2Ftokenizer%2Fmaster%2Fsrc%2Ftokenizer.1&name=tokenizer(1)&link=https%3A%2F%2Fgithub.com%2Fdspinellis%2tokenizer).
