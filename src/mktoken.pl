#!/usr/bin/env perl
#
# Find token uses in the specified language tokenizer files
# and generate a global tokens enumeration file
#

use strict;
use warnings;
use List::Util qw(shuffle);

my %token_symbol;

for my $in_fname (@ARGV) {
	open(my $in, '<', $in_fname) || die "Unable to open $in_fname: $!\n";
	while (<$in>) {
		s/\r//g;
		chop;
		$token_symbol{$1} = $2 if (/\bToken\:\:(\w+);\s+\/\/\s+(.*)/);
	}
}

my $out_fname = "Token.h";
open(my $out, '>', $out_fname) || die "Unable to open $out_fname: $!\n";

print $out qq|
/*
 * Auto-generated file
 */

#ifndef TOKEN_H
#define TOKEN_H

#include "TokenId.h"

class Token {
private:
	typedef std::map <int, std::string> TokenMap;
	TokenMap token_name;
	TokenMap token_symbol;

public:
	enum TokenNum {
		FIRST = TokenId::OTHER_TOKEN,
|;

for my $t (sort keys %token_symbol) {
	print $out "\t\t$t,\n";
}

print $out "
	};

	Token() {
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
#endif /* TOKEN_H */
|;
