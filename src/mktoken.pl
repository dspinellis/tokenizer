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

my %tokens;

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
	TokenMap tm;

public:
	enum TokenNum {
		FIRST = TokenId::OTHER_TOKEN,
|;

while (<$in>) {
	$tokens{$1} = 1 if (/\b${lang}Token\:\:(\w+)/);
}

for my $t (sort keys %tokens) {
	print $out "\t\t$t,\n";
}

print $out "
	};

	${lang}Token() {
		tm = {
";

for my $t (shuffle keys %tokens) {
	print $out qq(\t\t\t{$t, "$t" },\n);
}

print $out qq|
		};
	}

	const std::string & to_string(int k) const {
		static const std::string UNKNOWN("???");

		auto t = tm.find(k);
		return t == tm.end() ? UNKNOWN : t->second;
	}
};
#endif /* ${lang}TOKEN_H */
|;
