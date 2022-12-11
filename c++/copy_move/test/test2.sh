#!/bin/bash

binary=./bin/mini_terminal

dd if=/dev/urandom of=testfile1 bs=1M count=20 iflag=fullblock 2> /dev/null
mv testfile1 ./tmp/
dd if=/dev/urandom of=testfile2 bs=1M count=20 iflag=fullblock 2> /dev/null
mv testfile2 ./tmp/

$binary -a ./tmp/testfile1 ./tmp/testfile2

cd ./tmp/
if [[ $(cmp testfile1 testfile2) == "" ]]; then
    echo "cmp matches"   
fi
du testfile1 testfile2

rm testfile1 testfile2
echo "------------"
echo "TEST FINALIZADO CON 0 ERRORES"


exit 0