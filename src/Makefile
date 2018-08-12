PREFIX ?= /usr/local
BINPREFIX ?= "$(PREFIX)/bin"
MANPREFIX ?= "$(PREFIX)/share/man/man1"

# All warnings, treat warnings as errors, generate dependencies in .d files
# offer C++11 features
CXXFLAGS=-Wall -Werror -MD -std=c++11

ifdef DEBUG
LDFLAGS=-g
CXXFLAGS+=-g -O0
else
CXXFLAGS+=-O2
endif

KEYWORD_FILES=$(wildcard *-keyword.txt)
TOKENIZER_FILES=$(patsubst %-keyword.txt,%Tokenizer.cpp,$(wildcard *-keyword.txt))
GENERATED_HEADERS=Keyword.h Token.h
TEST_FILES=$(wildcard *Test.h)

all: $(GENERATED_HEADERS) tokenizer


OBJS=CTokenizer.o CppTokenizer.o JavaTokenizer.o CSharpTokenizer.o \
     PythonTokenizer.o TokenizerBase.o SymbolTable.o TokenId.o \
     NestedClassState.o PHPTokenizer.o

UnitTests: UnitTests.o $(OBJS) Token.h
	$(CXX) $(LDFLAGS) UnitTests.o $(OBJS) -lcppunit -o $@

UnitTests.o: $(TEST_FILES)

test: $(GENERATED_HEADERS) UnitTests
	./UnitTests

tokenizer: $(GENERATED_HEADERS) $(OBJS) tokenizer.o
	$(CXX) $(LDFLAGS) tokenizer.o $(OBJS) -o $@

Token.h: mktoken.pl $(TOKENIZER_FILES)
	./mktoken.pl $(TOKENIZER_FILES)

Keyword.h: mkkeyword.pl $(KEYWORD_FILES)
	./mkkeyword.pl $(KEYWORD_FILES)

install: all
	@mkdir -p $(DESTDIR)$(MANPREFIX)
	@mkdir -p $(DESTDIR)$(BINPREFIX)
	install tokenizer $(DESTDIR)$(BINPREFIX)/
	install -m 644 tokenizer.1 $(DESTDIR)$(MANPREFIX)/

clean:
	rm -f *.o *.d *.exe tokenizer UnitTests Token.h Keyword.h

# Pull-in dependencies generated with -MD
-include $(OBJS:.o=.d)
