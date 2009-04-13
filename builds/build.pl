#! /usr/bin/perl -w
use lib 'builds';
use strict;
use Getopt::Long;
use Cwd;

require "buildUtils.pl";

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
my $scriptname = "build.pl";
my @projects;

&GetOptions(
   "proj=s",      => \$BuildUtils::proj,
   "config=s",    => \$BuildUtils::config,
   "deep",      => \$deep
);


if($BuildUtils::proj eq "" || $BuildUtils::config eq "")
{
   usage();
}
exit (main());

sub main
{
   push( @projects, $BuildUtils::proj );
   
   if( $deep == 0 )   {   
      BuildUtils::doProjectCheckout($BuildUtils::proj);
      buildProject( $BuildUtils::proj );
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
   }
}

sub buildProject()
{
   my( $curProj )  = @_;
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
      chdir( "$curProj/build/vc8" );
      BuildUtils::doProjectBuild( $curProj, $BuildUtils::config );
      chdir( "../../.." );
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