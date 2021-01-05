rem generate_the_project
@echo off

set premake_exe="ext\bin\premake\premake5.exe"
set config_file="premake5.lua"
set environment="vs2019"

set solution_name=native_world.sln

del %solution_name%
pushd "../"
call %premake_exe% %environment%
popd

@echo on
rem The project is built
PAUSE