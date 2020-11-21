#!/bin/bash

# make sure script is running from MRE/utility directory
cd "$(dirname "$0")"

# main directory
cd ..

rm -rf dist
mkdir dist

# dist directory
cd dist

cmake ..
make

# remove everything except output
# and hide the output (1> /dev/null 2> /dev/null)
find . -not -name '*mre-exe*' -exec rm -rv {} \; 1> /dev/null 2> /dev/null

echo "Build complete!"