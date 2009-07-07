#!/usr/bin/perl
use strict;

package BuildUtils;

my $proj = "";
my $config = "";
my $target = "";

sub printProject
{
   my( $curProject ) = @_;
   print "\n------------------------------------\n-\n";
   print "- Building: $curProject \n";
   print "-\n------------------------------------\n";
}

sub doProjectCheckout
{
   my( $curProject ) = @_;
   print "\n------------------------------------\n";
   print "- Checking Out: $curProject \n";
   print "------------------------------------\n";
   system( "cvs co $curProject");   
}

sub doCleanBuild
{
   my( $curProject, $curConfig ) = @_;
   system( "msbuild $curProject.sln /t:Rebuild /p:Configuration=$curConfig /v:d" );
}
sub doBuild
{
   my( $curProject, $curConfig ) = @_;
   system( "msbuild $curProject.sln /p:Configuration=$curConfig /v:d" );
}
sub doProjectValidate
{
   my( $project ) = @_;
   chdir( "$project/validation" );
   if( -f "$ENV{'CDH_BINS_DIR'}/validate.exe")
   {
      system( "$ENV{'CDH_BINS_DIR'}/validate.exe" );
   }
   chdir( "../.." );
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

sub getCompileCommands
{
   my( $project ) = @_;
   my @compileCommands;
   my $compfile = "$project/build/compilationSteps";
               
   if( -f "$compfile" )
   {
      open(COMPFILE, "<$compfile"); 
      my @lines = <COMPFILE>;
      chomp @lines;
      close COMPFILE;    
      
      push(@compileCommands, @lines);
      @compileCommands = reverse(@compileCommands);
   }   
   return @compileCommands;
}
sub getDistroCommands
{
   my( $project ) = @_;
   my @distroCommands;
   my $distfile = "$project/build/distroCommands";
               
   if( -f "$distfile" )
   {
      open(DISTFILE, "<$distfile"); 
      my @lines = <DISTFILE>;
      chomp @lines;
      close DISTFILE;    
      
      push(@distroCommands, @lines);
   }   
   return @distroCommands;
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