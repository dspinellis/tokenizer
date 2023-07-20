#!/usr/bin/env perl
#
# Create a map from keywords to their enumeration
#

use strict;
use warnings;
use List::Util qw(shuffle);

my %keywords;
my @keywords;

my @languages;
my %keyword_languages;

for my $in_fname (@ARGV) {

	my $language = $in_fname;
	$language =~ s/\-keyword\.txt//;
	push(@languages, $language);

	open(my $in, '<', $in_fname) || die "Unable to open $in_fname: $!\n";
	while (<$in>) {
		s/\r//g;
		chop;
		$keywords{$_} = 1;
		push @{$keyword_languages{$_}}, "li == L_$language";
	}
}
@keywords = keys %keywords;

my $out_fname = "Keyword.h";
open(my $out, '>', $out_fname) || die "Unable to open $out_fname: $!\n";

print $out qq(
// Automatically generated file.  See $0.

#ifndef KEYWORD_H
#define KEYWORD_H

#include <map>
#include <string>

#include "TokenId.h"

/** Classify identifiers into keywords */
class Keyword {
public:
	enum IdentifierType : token_type {
		FIRST = TokenId::KEYWORD,
		IDENTIFIER,	// Plain identifier (not a keyword)
);

for my $k (sort @keywords) {
	print $out "\t\tK_$k,\n";
}

print $out "
		LAST,		// Last value (295)
	};

	enum LanguageId {
";

for my $lang (sort @languages) {
	print $out "\t\tL_$lang,\n";
}

print $out "
	};
private:
	// Keyword map
	typedef std::map <std::string, enum IdentifierType> KeywordMap;
	typedef std::map <token_type, std::string> TokenMap;
	KeywordMap km;
	TokenMap tm;
public:
	// Create a keyword recognizer for the specified language
	Keyword(enum LanguageId li) : km() {
";

# Shuffle to avoid presenting sorted data to the map
for my $k (shuffle @keywords) {
	my $condition = join(" || ", @{$keyword_languages{$k}});
	print $out qq(\t\tif ($condition)\n);
	print $out qq(\t\t\tkm.emplace("$k", K_$k);\n);
}

print $out '
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

	const std::string & to_string(token_type k) const {
		static const std::string UNKNOWN("???");

		auto t = tm.find(k);
		return t == tm.end() ? UNKNOWN : t->second;
	}
};
#endif /* KEYWORD_H */
|;
