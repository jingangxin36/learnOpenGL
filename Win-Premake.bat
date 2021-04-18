@echo off

pushd Graphics
call ..\premake\premake5.exe vs2017
popd
PAUSE