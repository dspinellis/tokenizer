#!/usr/bin/env perl
#
# Find token uses in the specified language tokenizer file
# and generate the corresponding tokens enumeration file
#

use strict;
use warnings;
use List::Util qw(shuffle);

my $lang = $ARGV[0];

my $in_fname = "${lang}Tokenizer.cpp";
my $out_fname = "${lang}Token.h";

my %token_symbol;

open(my $in, '<', $in_fname) || die "Unable to open $in_fname: $!\n";
open(my $out, '>', $out_fname) || die "Unable to open $out_fname: $!\n";

print $out qq|
/*
 * Auto-generated file
 */

#ifndef ${lang}TOKEN_H
#define ${lang}TOKEN_H

#include "TokenId.h"

class ${lang}Token {
private:
	typedef std::map <int, std::string> TokenMap;
	TokenMap token_name;
	TokenMap token_symbol;

public:
	enum TokenNum {
		FIRST = TokenId::OTHER_TOKEN,
|;

while (<$in>) {
	chop;
	$token_symbol{$1} = $2 if (/\b${lang}Token\:\:(\w+); \/\/ (.*)/);
}

for my $t (sort keys %token_symbol) {
	print $out "\t\t$t,\n";
}

print $out "
	};

	${lang}Token() {
		token_name = {
";

for my $t (shuffle keys %token_symbol) {
	print $out qq(\t\t\t{$t, "$t" },\n);
}

print $out "
		};

		token_symbol = {
";

for my $t (shuffle keys %token_symbol) {
	print $out qq(\t\t\t{$t, "$token_symbol{$t}" },\n);
}


print $out qq|
		};
	}

	const std::string & to_string(int k) const {
		static const std::string UNKNOWN("???");

		auto t = token_name.find(k);
		return t == token_name.end() ? UNKNOWN : t->second;
	}

	const std::string & to_symbol(int k) const {
		static const std::string UNKNOWN("???");

		auto t = token_symbol.find(k);
		return t == token_symbol.end() ? UNKNOWN : t->second;
	}
};
#endif /* ${lang}TOKEN_H */
|;
