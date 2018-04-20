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

KEYWORD_HEADERS=$(patsubst %-keyword.txt,%Keyword.h,$(wildcard *-keyword.txt))
TOKEN_HEADERS=$(patsubst %-keyword.txt,%Token.h,$(wildcard *-keyword.txt))
GENERATED_HEADERS=$(KEYWORD_HEADERS) $(TOKEN_HEADERS)

all: $(GENERATED_HEADERS) tokenizer


OBJS=CTokenizer.o JavaTokenizer.o TokenizerBase.o SymbolTable.o TokenId.o \
     NestedClassState.o

UnitTests: UnitTests.o $(OBJS) CToken.h JavaToken.h
	$(CXX) $(LDFLAGS) UnitTests.o $(OBJS) -lcppunit -o $@

test: $(GENERATED_HEADERS) UnitTests
	./UnitTests

tokenizer: $(GENERATED_HEADERS) $(OBJS) tokenizer.o
	$(CXX) $(LDFLAGS) tokenizer.o $(OBJS) -o $@

%Token.h: %Tokenizer.cpp
	./mktoken.pl $(patsubst %Token.h,%,$@)

%Keyword.h: %-keyword.txt
	./mkkeyword.pl $(patsubst %Keyword.h,%,$@)

install: all
	@mkdir -p $(DESTDIR)$(MANPREFIX)
	@mkdir -p $(DESTDIR)$(BINPREFIX)
	install tokenizer $(DESTDIR)$(BINPREFIX)/
	install -m 644 tokenizer.1 $(DESTDIR)$(MANPREFIX)/

clean:
	rm -f *.o *.d *.exe tokenizer UnitTests *Token.h *Keyword.h

# Pull-in dependencies generated with -MD
-include $(OBJS:.o=.d)
