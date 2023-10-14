#!/usr/bin/env bash

EPS=0.01

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
	echo  "${matr[$sel,$y]#-}"
	if [[ $(echo  "${matr[$sel,$y]#-}<$EPS" | bc -l) == 1 ]]
	then
	    ((y++))
		continue
	fi
	echo $sel
	for ((k=$y; k<=$column; k++))
	do
	    tmp=${matr[$sel,$k]}
	    matr[$sel,$k]=${matr[$x,$k]}
	    matr[$x,$k]=$tmp
    done
    
    
    for ((kk=0;kk<row;kk++))
    do
        for ((ll=0;ll<=column;ll++))
        do
        echo ${matr[$kk,$ll]}
        done
        
    done
    echo -------
    
    
    place[$y]=$x
    
	for ((k=0; k<$row; k++))
	do  
		if [[ $k != $x ]] 
		then
		    echo  ${matr[$k,$y]} / ${matr[$x,$y]} | bc -l
		    c=$(echo "${matr[$k,$y]} / ${matr[$x,$y]}" | bc -l)
		    
			for ((l=$y; l<=$column; l++))
			do
			    permut=$(echo "${matr[$x,$l]} * $c" | bc -l)
				matr[$k,$l]=$(echo "${matr[$k,$l]} - $permut" | bc -l)
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
		    ans[$i]=$(echo "${matr[$tmp,$column]} / ${matr[$tmp,$i]}" | bc -l)

	    fi
done

for ((i=0;i<$column;i++)) do
    echo ${ans[$i]}
done














