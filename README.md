[![Build Status](https://travis-ci.org/dspinellis/tokenizer.svg?branch=master)](https://travis-ci.org/dspinellis/tokenizer)
[![DOI](https://zenodo.org/badge/DOI/10.5281/zenodo.2558419.svg)](https://doi.org/10.5281/zenodo.2558419)


# tokenizer

Tokenize source code into integer vectors, symbols, or discrete tokens.

The following languages are currently supported.
* C
* C#
* C++
* Java
* PHP
* Python

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

## Contributing
To support a new language proceed as follows.
* Open an issue with the language name and a pointer to its lexical structure
defintion.
* Add a comment indicating that you're working on it.
* List the language's keywords in a file name *language*`-keyword.txt`.
Keep alphabetic order. If the language supports a C-like preprocessor
add those keywords as well.
* Copy the source code files of an existing language that most resembles
the new language to create the new language files:
*language*`Tokenizer.cpp`, *language*`Tokenizer.h`, *language*`TokenizerTest.h`.
* In the copied files rename all instances
(uppercase, lowercase, CamelCase) of the existing language name to the
new language name.
* Create a list of the new language's operators and punctuators, and
methodically go through the *language*`Tokenizer.cpp` `switch` statements
to ensure that these are correctly handled.
When code is missing or different, base the new code on an existing pattern.
* Add code to handle the language's comments.
* Adjust, if needed, the handling of constants and literals.
Note that for the sake of simplicity and efficiency,
the tokenizer can assume that its input is correct.
* To implement features that aren't handled in the language whose
tokenizer implementation you copied, look at the implementation of other
language tokenizers that have these features.
* If you need to reuse a method from another language, move it to
`TokenizerBase`.
* Add the object file *language*`Tokenizer.o` to the `OBJ` list of file
names in the `Makefile`.
* Add unit tests for any new or modified features you implemented.
* Update the file`UnitTests.cpp` to include the unit test header file,
and call `addTest` with the unit test suite.
* Update the method `process_file` in `tokenizer.cpp` to call the
tokenizer you implemented and the language's name to the list
of supported languages.
* Ensure the language is correctly tokenized, both by running the
tokenizer and by running the unit tests with `make test`.
* Update the manual page `tokenizer.1` and this `README.md` file.
