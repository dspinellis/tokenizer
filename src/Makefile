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

all: tokenizer

HFILES=$(wildcard *.h)
OBJS=CTokenizer.o JavaTokenizer.o TokenizerBase.o SymbolTable.o TokenId.o

UnitTests: UnitTests.o $(OBJS) CToken.h JavaToken.h
	$(CXX) $(LDFLAGS) UnitTests.o $(OBJS) -lcppunit -o $@

test: UnitTests
	./UnitTests

tokenizer: $(OBJS) tokenizer.o
	$(CXX) $(LDFLAGS) tokenizer.o $(OBJS) -o $@

UnitTests.o CTokenizer.o: CToken.h
UnitTests.o JavaTokenizer.o: JavaToken.h

CToken.h: CTokenizer.cpp
	./mktoken.pl C

JavaToken.h: JavaTokenizer.cpp
	./mktoken.pl Java

install: all
	@mkdir -p $(DESTDIR)$(MANPREFIX)
	@mkdir -p $(DESTDIR)$(BINPREFIX)
	install tokenizer $(DESTDIR)$(BINPREFIX)/
	install -m 644 tokenizer.1 $(DESTDIR)$(MANPREFIX)/

clean:
	rm -f *.o *.d *.exe tokenizer UnitTests CToken.h JavaToken.h

# Pull-in dependencies generated with -MD
-include $(OBJS:.o=.d)
