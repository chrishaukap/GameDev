#!/usr/bin/perl
use strict;

package BuildEnv;

require "buildUtils.pl";

sub setupBuildConfiguration
{
   setProjectName( $BuildUtils::proj );
   setConfiguration( $BuildUtils::config );
   setTarget($BuildUtils::target);
   setProjBuildDirectory( "$BuildUtils::proj/build/$BuildUtils::target" );
   setProjBinDirectory( "$BuildUtils::proj/bin/$BuildUtils::target/$BuildUtils::config" );
   setProjLibDirectory( "$BuildUtils::proj/lib/$BuildUtils::target/$BuildUtils::config" );
   setProjDistDirectory( "$BuildUtils::proj/dist/$BuildUtils::target/$BuildUtils::config" );
   setBinsDirectory( "bin/$BuildUtils::target/$BuildUtils::config" );
}
#set build directory
sub setProjectName
{
   my( $projName ) = @_;
   $ENV{'CDH_PROJECT_NAME'} = $projName;
}

#set bin directory
sub setBinsDirectory
{
   my( $dir ) = @_;
   $ENV{'CDH_BINS_DIR'} = $dir;
}

#set build directory
sub setProjBuildDirectory
{
   my( $dir ) = @_;
   $ENV{'CDH_PROJBUILD_DIR'} = $dir;
}
#set bin directory
sub setProjBinDirectory
{
   my( $dir ) = @_;
   $ENV{'CDH_PROJBIN_DIR'} = $dir;
}
#set lib directory
sub setProjLibDirectory
{
   my( $dir ) = @_;
   $ENV{'CDH_PROJLIB_DIR'} = $dir;
}
#set dist directory
sub setProjDistDirectory
{
   my( $dir ) = @_;
   $ENV{'CDH_PROJDIST_DIR'} = $dir;
}

#set build configuration
sub setConfiguration
{
   my( $config ) = @_;
   $ENV{'CDH_CONFIGURATION'} = $config;
}
#set windows for now
sub setTarget
{
   my( $target ) = @_;
   $ENV{'CDH_TARGET'} = $target;
}
1;