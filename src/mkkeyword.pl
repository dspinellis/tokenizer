#!/usr/bin/env perl
#
# Create a map from keywords to their enumeration
#

use strict;
use warnings;
use List::Util qw(shuffle);

my $lang = $ARGV[0];

my $in_fname = "${lang}-keyword.txt";
my $out_fname = "${lang}Keyword.h";

my @keywords;

open(my $in, '<', $in_fname) || die "Unable to open $in_fname: $!\n";
open(my $out, '>', $out_fname) || die "Unable to open $out_fname: $!\n";

print $out qq(
#ifndef ${lang}KEYWORD_H
#define ${lang}KEYWORD_H

#include <map>
#include <string>

#include "TokenId.h"

/** Classify identifiers into ${lang} keywords */
class ${lang}Keyword {
public:
	enum IdentifierType {
		FIRST = TokenId::KEYWORD,
		IDENTIFIER,	// Plain identifier (not a keyword)
);

while (<$in>) {
	chop;
	push(@keywords, $_);
}

for my $k (sort @keywords) {
	$k = 'KEYWORD_NULL' if ($k eq 'null');
	print $out "\t\t", uc($k), ",\n";
}

print $out "
		LAST,		// Last value (295)
	};
private:
	// Keyword map
	typedef std::map <std::string, enum IdentifierType> KeywordMap;
	KeywordMap km;
public:
	${lang}Keyword() {
		km = {
";

# Shuffle to avoid presenting sorted data to the map
for my $k (shuffle @keywords) {
	my $uck = uc($k);
	$uck = 'KEYWORD_NULL' if ($uck eq 'NULL');
	print $out qq(\t\t\t{"$k", $uck },\n);
}
print $out qq|
		};
	}

	enum IdentifierType identifier_type(const std::string &s) {
		KeywordMap::const_iterator f = km.find(s);
		if (f == km.end())
			return IDENTIFIER;
		else
			return f->second;
	}
};
#endif /* ${lang}KEYWORD_H */
|;
