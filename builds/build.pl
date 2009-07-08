#! /usr/bin/perl -w
use lib 'builds';
use strict;
use Getopt::Long;
use Cwd;

require "buildUtils.pl";
require "buildEnv.pl";

sub buildLibUtils;
sub buildLibOgg;
sub buildLibAlut;
sub buildLibVorbis;
sub buildLibOpenAL;
sub buildLibOggController;
sub usage();
sub checkoutProjects();
sub buildProject;
sub compileProject;
sub deepBuild;

my $deep = 0;
my $doVal = 1;
my $scriptname = "build.pl";
my @projects;

$BuildUtils::target = 'win32';
$BuildUtils::rebuild = 0;
&GetOptions(
   "proj=s",      => \$BuildUtils::proj,
   "config=s",    => \$BuildUtils::config,
   "target=s",    => \$BuildUtils::target,
   "deep!",       => \$deep,
   "val!",        => \$doVal,
   "rebuild",      => \$BuildUtils::rebuild   
);

if($BuildUtils::proj eq "" || $BuildUtils::config eq "")
{
   usage();
}
die "unknown project!" unless -d $BuildUtils::proj; 
exit (main());

sub main
{
   BuildEnv::setupBuildConfiguration();   
   push( @projects, $BuildUtils::proj );      
   
   if( $deep == 0 )   {   
      BuildUtils::doProjectCheckout($BuildUtils::proj);  
      buildProject($BuildUtils::proj);
   }
   else   {
      my @dependencies = BuildUtils::getDependencies($BuildUtils::proj);
      push(@projects, @dependencies);
      @projects = BuildUtils::removeDuplicateDependencies(@projects);
      deepBuild();
   }
   exit;
   
   return 0;
}

sub buildProject
{
   my( $project ) = @_;      
   chdir( "$project" );
   BuildUtils::printProject($project);     
   compileProject($project);   

   if($doVal == 1)
   { 
      BuildUtils::doProjectValidate();
   }   

   buildDist();
   chdir( ".." );
}
sub buildDist
{
   my( $curProj )  = @_;
   my @compileCommands = BuildUtils::getDistroCommands();
   if( $#compileCommands >= 0 )
   {
      BuildUtils::executeCommands(@compileCommands);
   }
   else
   {
      printf "No Distribution Commands to execute\n";
   }
     
}

sub deepBuild
{
   while( $#projects >= 0 )
   { 
      buildProject(pop(@projects));
   }
}
sub compileProject
{
   my( $curProj )  = @_;
   
   my @compileCommands = BuildUtils::getCompileCommands();
   if( $#compileCommands >= 0 )
   {
      BuildUtils::executeCommands(@compileCommands);
   }
   else
   {     
      #  assume one of mine
      chdir( "build/$BuildUtils::target" );
      BuildUtils::doBuild( $curProj, $BuildUtils::config );
      chdir( "../.." );    
   }
}

sub usage()
{
   print "$scriptname -proj project -conf configuration\n";
   exit 0;
}