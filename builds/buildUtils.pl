#!/usr/bin/perl
use strict;

package BuildUtils;

my $proj = "";
my $config = "";
my $target = "";
my $rebuild = 0;

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
   print "\n------------------------------------\n-\n";
   print "- Checking Out: $curProject \n";
   print "-\n------------------------------------\n";
   system( "cvs co $curProject");   
}

sub executeCommands
{
   my (@listOfCommands) = @_;
   while( $#listOfCommands >= 0 )
   {
      my $command = pop(@listOfCommands);
      print "EXECUTING COMMAND: $command\n";      
      if( substr($command, 0, 3) eq "cd " )
      {
         chdir( substr($command, 3) );
      }
      else
      {
         system( "$command" ); 
      }
   }
}

sub doBuild
{
   my( $curProject, $curConfig ) = @_;
   print "DOBUILD: msbuild $curProject.sln $ENV{'CDH_REBUILD'} /p:Configuration=$curConfig /v:d\n\n";
   system( "msbuild $curProject.sln $ENV{'CDH_REBUILD'} /p:Configuration=$curConfig /v:d" );
}
sub doProjectValidate
{
   if( -d "validation" )
   {
      chdir( "validation" );
      if( -f "$ENV{'CDH_BINS_DIR'}/validate.exe")
      {
         system( "$ENV{'CDH_BINS_DIR'}/validate.exe" );
      }
      chdir( ".." );
   }
   else
   {
      print "No validation tests to run...\n";
   }
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
   my @compileCommands;
   my $compfile = "build/compilationSteps";
               
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
   my @distroCommands;
   my $distfile = "build/distroCommands";
   if( -f "$distfile" )
   {
      open(DISTFILE, "<$distfile"); 
      my @lines = <DISTFILE>;
      chomp @lines;
      close DISTFILE;    
      
      push(@distroCommands, @lines);
      @distroCommands = reverse(@distroCommands);
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