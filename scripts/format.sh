#! /bin/sh
cd "`dirname $0`/.."
find src -type f -name '*.[hc]pp' | xargs clang-format -verbose -i
