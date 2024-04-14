for SOL in `ls sols/ -l | grep "^...x" | awk '{print $9}'`
do
  echo $SOL
  let k=`ls tests/*in | wc -l`; 
  for i in `seq -w 1 $k`; 
  do 
    echo -n "test $i --> "; 
    tt=`(time cat tests/$i.in | ./sols/$SOL > WW) 2>&1 >/dev/null | grep 'real' | awk -F"m" '{print $2}' | sed -e 's/s//'`; 
    echo -n "time=$tt rez="; ./checker/check tests/$i.in WW tests/$i.out;  
  done
done
if [ -f WW ] 
  then 
    rm WW
  else
    echo "Нет решений!"
fi
