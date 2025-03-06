#!/bin/bash
mkdir test
cp -r code test
cat test/code/14.c

cd test/code
n=0
while [ $n -le 15 ]
do
	gcc -c  $n.c
	n=$[$n+1]
done
gcc *.o -o ../hello
cd ..

./hello 2> err.txt

mv err.txt ..
#pwd
cd ..
chmod 655 err.txt

n1=1
n2=1

if [ $# -eq 1 ]; then
	n1=$1
fi

if [ $# -eq 2 ]; then
	n1=$1
	n2=$2
fi
n3=$[$n1+$n2]
tmp=${n3}p
#echo $tmp
sed -n ${tmp} err.txt >&2
