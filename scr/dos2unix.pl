#!/usr/bin/perl

for(@ARGV){
  print STDERR "doing $_\n";
  open(STDIN,"<$_") or die "open:$_:$!";
  open(STDOUT,">$_.new") or die "open:$_:$!";
  link("$_","$_.old") or die "link:$_,$_.old:$!";
  {
    local ($_);
    while(<STDIN>){
      s/\s+$//; 
      print "$_\n";
    };
  };
  rename("$_.new","$_") or die "rename:$_.new,$_:$!";
};
