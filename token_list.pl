#!/usr/bin/perl
#
# List the tokens read
#


while (<>) {
	for my $n (split) {
		if ($n < 256) {
			print chr($n);
		}
	}
}
