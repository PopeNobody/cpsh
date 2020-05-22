#!/usr/bin/perl


use strict;
use warnings;
use autodie qw(:all);

use Data::Dumper;

die "usage: $0 <ofn>\n" if @ARGV!=1;

my $ofn = shift;
if( -e $ofn ) {
  rename $ofn,"$ofn.old";
};
open(STDOUT,">$ofn") or die "open:$ofn:$!";
sub suck_file($) {
  local @ARGV = shift;
  <ARGV>;
};
open(STDIN,"find -type f \\( -name '*.d' -o -name '*.dd' \\) |") or die;
my %seen;
while(<STDIN>)
{
  my ($dir,$fnm) = m{(.*)/+(.*)};
  chomp;
  for(join("",suck_file($_)))
  {
    s{\\\n}{ }g;
    s{^\S+}{};
    s{\S+$}{};
    for( split ) {
      $_="$dir/$_" unless m{^/};
      ++$seen{$_} 
    };
  };
};
for( sort keys %seen ) {
  print "$_\n";
};
exit(0);
 
