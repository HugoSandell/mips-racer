#!/bin/bash
response="1"

echo -n "Enter number of cores (Leave blank for default) > "
read response

make install TTYDEV=/dev/ttyS2 -j"$response"
