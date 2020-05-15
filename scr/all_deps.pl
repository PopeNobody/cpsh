#!/usr/bin/perl


use strict;
use warnings;
use autodie qw(:all);

use Data::Dumper;

die "usage: $0 <ofn> <deps>\n" if @ARGV<2;

my $ofn = shift;
if( -e $ofn ) {
  die "$ofn exists";
};
open(STDOUT,">$ofn") or die "open:$ofn:$!";
sub suck_file($) {
  local @ARGV = shift;
  <ARGV>;
};
my %seen;
for(@ARGV){
  for(join("",suck_file($_)))
  {
    s{\\\n}{ }g;
    s{^\S+}{};
    ++$seen{$_} for( split );
  };
};
for( sort keys %seen ) {
  print "$_\n";
};
exit(0);

