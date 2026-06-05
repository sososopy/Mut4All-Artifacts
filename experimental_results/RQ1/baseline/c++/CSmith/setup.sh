tar -xf csmith-csmith-2.3.0.tar.gz
mv csmith-csmith-2.3.0 csmith

cd csmith
mkdir build
cmake -DCMAKE_INSTALL_PREFIX="build" .
make && make install
