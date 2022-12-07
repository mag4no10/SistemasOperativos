#!/bin/bash

binary=./bin/mini_terminal

touch ./tmp/example.txt && echo "1 2 3, Juan Periquito y Andres" > ./tmp/example.txt
echo "example.txt: "
cat ./tmp/example.txt
touch ./tmp/example2.txt && echo "" > ./tmp/example2.txt
echo "example2.txt: "
cat ./tmp/example2.txt

$binary -a ./tmp/example.txt ./tmp/example2.txt

echo "------------"
echo "example.txt: "
cat ./tmp/example.txt
echo "example2.txt: "
cat ./tmp/example2.txt

echo "------------"
cd ./tmp/
if [[ $(cmp example.txt example2.txt) == "" ]]; then
    echo "cmp matches"   
fi
du example.txt example2.txt

rm example.txt example2.txt
echo "------------"
echo "TEST FINALIZADO CON 0 ERRORES"


exit 0