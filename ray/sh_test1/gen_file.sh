a=1

while [ $a -le 100 ]
do
    mkdir file${a} 
    a=$[$a+1]
done
