#! /bin/bash
set -e

sep() {
    echo "----------------------------------------------------------------"
}

sep
echo "version info"
g++ --version
cmake --version
cat /etc/os-release

sep
cmake \
    -DEZXML_BUILD_TESTS=on \
    -DEZXML_BUILD_EXAMPLES=on \
    .

sep
make -j 6

sep
echo "running tests"
./ezxml_test
echo "success"

echo ""
echo "done running script - success"
exit 0
