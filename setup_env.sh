echo "Setting up environment for demo..."

export CC="gcc-5"

echo "Building..."

ORIG_DIR=`pwd`

cd mongo && scons -j4 mongod mongo && echo "Mongo built." && cd $ORIG_DIR

cd mongo-c-driver
./autogen.sh --with-libbson=bundled --enable-debug-symbols=full --enable-optimizations=no

make -j4 all && echo "c driver built."

cd $ORIG_DIR

echo "Testing"

# one failure expected
cd mongo && scons -j4 unittests; cd $ORIG_DIR
cd mongo-c-driver && make -j4 test && cd $ORIG_DIR

echo "Ready"
