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
sub deepBuild;

my $deep = 0;
my $doVal = 1;
my $rebuild = 0;
my $scriptname = "build.pl";
my @projects;

&GetOptions(
   "proj=s",      => \$BuildUtils::proj,
   "config=s",    => \$BuildUtils::config,
   "target=s",    => \$BuildUtils::target,
   "deep!",       => \$deep,
   "val!",        => \$doVal,
   "rebuild!",      => \$rebuild
);

if($BuildUtils::proj eq "" || $BuildUtils::config eq "")
{
   usage();
}
$BuildUtils::target = 'vc8';
BuildEnv::setupBuildConfiguration();
exit (main());

sub main
{
   push( @projects, $BuildUtils::proj );
   
   if( $deep == 0 )   {   
      BuildUtils::doProjectCheckout($BuildUtils::proj);
      buildProject( $BuildUtils::proj );    
      if( $doVal == 1)
      {
         BuildUtils::doProjectValidate($BuildUtils::proj);
      }
      buildDist( $BuildUtils::proj );
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

sub deepBuild
{
   while( $#projects >= 0 )
   {
      my $project = pop(@projects);
      BuildUtils::printProject($project);      
      buildProject($project);
      if($doVal == 1)
      {
         BuildUtils::doProjectValidate($project);
      }
      buildDist($project);
   }
}
sub buildDist()
{
   my( $curProj )  = @_;
   executeCommands(BuildUtils::getDistroCommands($BuildUtils::proj));
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
sub buildProject()
{
   my( $curProj )  = @_;
   
   my @compileCommands = BuildUtils::getCompileCommands($BuildUtils::proj);
   if( $#compileCommands >= 0 )
   {
      executeCommands(@compileCommands);
   }
   else
   {   
      if( $curProj  eq "lib_ogg" )   {
         buildLibOgg($curProj);   
      }
      elsif( $curProj eq "lib_openAL" ) {
      }
      elsif( $curProj  eq "lib_vorbis" )   {
         buildLibVorbis($curProj);   
      }
      elsif( $curProj  eq "lib_alut" )   {
         buildLibAlut($curProj);   
      }
      elsif( $curProj eq "app_scribbleSHMUP" ){      
         # have to treat this differently still because it also runs its own installer installer
         buildProjWithInstaller($curProj);
      }
      elsif( $curProj eq "graveyardDemo" ) {
         # have to treat this differently still because it also runs its own installer installer
         buildProjWithInstaller($curProj);
      }
      else   {
         #  assume one of mine
         chdir( "$curProj/build/$BuildUtils::target" );
         if($rebuild == 1)
         {
            BuildUtils::doCleanBuild( $curProj, $BuildUtils::config );
         }
         else
         {
            BuildUtils::doBuild( $curProj, $BuildUtils::config );
         }
         chdir( "../../.." );         
      }
   }
}

sub buildLibOgg
{
   my( $curProject ) = @_;
   chdir("$curProject/build");
   system("call buildOgg.bat $BuildUtils::config");
   chdir("../..");
}
sub buildLibAlut
{
   my( $curProject ) = @_;
   chdir("$curProject/build");
   system("call buildAlut.bat $BuildUtils::config");
   chdir("../..");   
}
sub buildLibVorbis
{
   my( $curProject ) = @_;
   chdir("$curProject/build");
   system("call buildVorbis.bat $BuildUtils::config");
   chdir("../..");
}

sub usage()
{
   print "$scriptname -proj project -conf configuration\n";
   exit 0;
}

use File::Path;

sub buildProjWithInstaller
{
   my( $curProj )  = @_;
   my $curdir = getcwd();
   
   chdir("$curProj/build");
   BuildUtils::doProjectBuild($curProj, $BuildUtils::config);
   chdir("../..");      
   
   BuildUtils::printProject( "installer for $curProj" );
   $ENV{'PROJ_DIR'} = "$curdir/$curProj"; #required for installer script
   print "using PROJ_DIR: $ENV{'PROJ_DIR'}\n";
   mkpath( "$curProj/dist/vc8/$BuildUtils::config" );
   
   my $path = "";
   # HAUKAP - lame hack, hardcode nsi names
   if( $curProj eq "app_scribbleSHMUP" ){      
      $path = "$curProj/inst/scribbleSHMUP.nsi";
   }
   elsif( $curProj eq "graveyardDemo" ) {
      $path = "$curProj/inst/CryptKeeper.nsi";
   }
   
   system( "makensis /DCONFIGURATION=$BuildUtils::config /DTARGET=vc8 $path" );
}