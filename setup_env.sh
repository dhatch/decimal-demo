echo "Setting up environment for demo..."
echo "Building..."

ORIG_DIR=`pwd`

cd mongo;
scons -j4 mongod mongo || cd $ORIG_DIR && exit $?
echo "Mongo built." && cd $ORIG_DIR

cd mongo-c-driver
export CC='gcc-5'

if [! -f configure]; then
./autogen.sh --with-libbson=bundled --enable-debug-symbols=full --enable-optimizations=no
./src/libbson/autogen.sh --enable-debug-symbols=full --enable-optimizations=no
fi

make -j4 all || cd $ORIG_DIR && exit $?
echo "c driver built."

cd $ORIG_DIR

echo "Testing"

# one failure expected
cd mongo && scons -j4 unittests && ./buildscripts/resmoke.py --suites=unittests --continueOnFailure; cd $ORIG_DIR

mkdir -p test/tmp/data
./mongo/mongod --dbpath test/tmp/data 2>&1 >/dev/null &
cd mongo-c-driver && CC='gcc-5' make -j4 test && cd $ORIG_DIR
rm -r test/tmp/data
kill %1

echo "Building demo..."

cd demo && make && cd $ORIG_DIR

echo "Ready"
