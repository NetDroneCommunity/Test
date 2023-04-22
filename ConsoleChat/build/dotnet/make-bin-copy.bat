@echo off

pushd %1

if not exist { "bin\%3" } (
    mkdir "bin\%3"
)

echo copy /y %4.* bin\%3\
copy /y "temp\%2\net6.0\%4.*" "bin\%3\"

echo copy /y NetDrone.* bin\%3\
copy /y "temp\%2\net6.0\NetDrone.*" "bin\%3\"

popd
