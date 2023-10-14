#!/usr/bin/env bash

EPS=0.0000000000000000001

declare -A matr
row=0
IFS=","
while read line
do
    column=0
    for num in $line
    do  
       matr[$row,$column]=$num
       (( column++ )) 
    done
    (( row++ ))
done < $1

((column--))
declare -a place
declare -a ans


for ((i=0; i<$column; i++))
do
    place[$i]=-1
    ans[$i]=0
done

x=0
y=0
while [[ $x < $row ]] && [[ $y < $column ]]
do
    sel=$x
    for ((k=$x; k<$row; k++)) 
    do
        if  [[ ${matr[$k,$y]#-} > ${matr[$sel,$y]#-} ]]
        then
            sel=$k
        fi
    done
    if [[ $(echo  "scale=21; ${matr[$sel,$y]#-}<$EPS" | bc) == 1 ]]
    then
        ((y++))
	    continue
    fi
    for ((k=$y; k<=$column; k++))
    do
        tmp=${matr[$sel,$k]}
        matr[$sel,$k]=${matr[$x,$k]}
        matr[$x,$k]=$tmp
    done
    place[$y]=$x
    for ((k=0; k<$row; k++))
    do  
	    if [[ $k != $x ]] 
	    then
	        c=$(echo "scale=21; ${matr[$k,$y]} / ${matr[$x,$y]}" | bc)    
		    for ((l=$y; l<=$column; l++))
		    do
		        permut=$(echo "scale=21; ${matr[$x,$l]} * $c" | bc)
			    matr[$k,$l]=$(echo "scale=21; ${matr[$k,$l]} - $permut" | bc)
		    done
        fi
    done
((x++))
((y++))
done

for ((i=0; i<$column; i++))
do
    if [[ place[$i] != -1 ]]
    then
        tmp=${place[$i]}
        ans[$i]=$(echo "scale=21; ${matr[$tmp,$column]} / ${matr[$tmp,$i]}" | bc)
    fi
done

for ((i=0;i<$column;i++)) do
    echo ${ans[$i]}
done
