#!/usr/bin/env perl
#
# Create a map from keywords to their enumeration
#

use strict;
use warnings;
use List::Util qw(shuffle);

my %keywords;
my @keywords;

for my $in_fname (@ARGV) {
	open(my $in, '<', $in_fname) || die "Unable to open $in_fname: $!\n";
	while (<$in>) {
		s/\r//g;
		chop;
		$keywords{$_} = 1;
	}
}
@keywords = keys %keywords;

my $out_fname = "Keyword.h";
open(my $out, '>', $out_fname) || die "Unable to open $out_fname: $!\n";

print $out qq(
#ifndef KEYWORD_H
#define KEYWORD_H

#include <map>
#include <string>

#include "TokenId.h"

/** Classify identifiers into keywords */
class Keyword {
public:
	enum IdentifierType {
		FIRST = TokenId::KEYWORD,
		IDENTIFIER,	// Plain identifier (not a keyword)
);

for my $k (sort @keywords) {
	print $out "\t\tK_$k,\n";
}

print $out "
		LAST,		// Last value (295)
	};
private:
	// Keyword map
	typedef std::map <std::string, enum IdentifierType> KeywordMap;
	typedef std::map <int, std::string> TokenMap;
	KeywordMap km;
	TokenMap tm;
public:
	Keyword() {
		km = {
";

# Shuffle to avoid presenting sorted data to the map
for my $k (shuffle @keywords) {
	print $out qq(\t\t\t{"$k", K_$k },\n);
}

print $out '
		};
		tm = {
';

for my $k (shuffle @keywords) {
	print $out qq(\t\t\t{K_$k, "$k" },\n);
}

print $out qq|
		};
	}

	enum IdentifierType identifier_type(const std::string &s) {
		auto f = km.find(s);
		if (f == km.end())
			return IDENTIFIER;
		else
			return f->second;
	}

	const std::string & to_string(int k) const {
		static const std::string UNKNOWN("???");

		auto t = tm.find(k);
		return t == tm.end() ? UNKNOWN : t->second;
	}
};
#endif /* KEYWORD_H */
|;
