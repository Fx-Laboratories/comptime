#/usr/bin/bash

mkdir build
cd build

cmake ..
sudo cmake --build . --config Release --target install -- -j $(nproc)

cd ..
sudo rm -rf build
