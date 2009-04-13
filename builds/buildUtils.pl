#!/usr/bin/perl
use strict;

package BuildUtils;

my $proj = "";
my $config = "";

sub printProject
{
   my( $curProject ) = @_;
   print "\n------------------------------------\n";
   print "- Building: $curProject \n";
   print "------------------------------------\n";
}

sub doProjectCheckout
{
   my( $curProject ) = @_;
   print "\n------------------------------------\n";
   print "- Checking Out: $curProject \n";
   print "------------------------------------\n";
   system( "svn co file:///svnsrc/$curProject");   
}

sub doProjectBuild
{
   my( $curProject, $curConfig ) = @_;
   system( "msbuild $curProject.sln /t:Rebuild /p:Configuration=$curConfig /v:d" );
}

sub stringExistsInArray
{
   my( $element, @array ) = @_;
   foreach (@array)
   {
      if( $_ eq $element )
      {return 1;}
   }
   return 0;
}
sub removeDuplicateDependencies
{
   my( @dependencies ) = @_;
   my @retarray;
   while( $#dependencies >= 0 )
   {
      my $dep = pop(@dependencies);
      my $exists = stringExistsInArray($dep, @retarray);
      if( $exists == 0 )
      {
         unshift(@retarray, $dep);
      }
   }   
   return @retarray;
}

sub getDependencies
{
   my( $project ) = @_;
   my @dependencies;
   my $depfile = "$project/build/dependencies";
            
   doProjectCheckout($project);
   
   if( -f "$depfile" )
   {
      open(DEPFILE, "<$depfile"); 
      my @lines = <DEPFILE>;
      chomp @lines;
      close DEPFILE;    
      
      push(@dependencies, @lines);
      foreach(@lines) {
         push(@dependencies, 
              getDependencies($_));
      }  
   }   
   return @dependencies;
}
1;