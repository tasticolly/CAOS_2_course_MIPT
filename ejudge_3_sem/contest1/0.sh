#!/usr/bin/env bash
fail="---"
#MANPATH=$(manpath)
find_man_file() {
    IFS=":"
    for dir in $MANPATH
    do
        filepath_zip="$dir/man3/$1.3.gz"
        filepath_normal="$dir/man3/$1.3"
        if [ -f "$filepath_zip" ]; then 
            extract_libname "$filepath_zip" && return 0
        elif [ -f "$filepath_normal" ]; then    
            extract_libname "$filepath_normal" && return 0
        fi
    done
    echo $fail
}


extract_libname() {
    lib=$(zgrep "#include <.*>" $1 | sed  's/.*include <//' | sed "s/>.*//" | head -1)
    echo "$lib"
}


while read function
do
    find_man_file "$function"
done
