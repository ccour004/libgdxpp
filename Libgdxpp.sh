
echo Executing Pre Build commands ...
mkdir build
cd build
cmake ..
ln -s ../assets assets
echo Done
make
