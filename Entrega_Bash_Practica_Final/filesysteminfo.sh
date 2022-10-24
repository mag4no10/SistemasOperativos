#!/bin/bash

# Funciones:
usage() {
    echo "Usage: filesysteinfo.sh [-inv para invertir]"
}

filesysteminfo() {
    output=$(sudo df --all --output=fstype | sort | uniq | tail +3 )
    if [ "$invert" = 1 ]; then
        output=$(sudo df --all --output=fstype | sort | uniq | tail +3 | sort -r)
    fi
    while read -r line; do
            count=$(sudo df --all --output=target,fstype,size -h | grep $line | wc -l)
            space=$(sudo df --all --output=target,fstype,size -h | grep $line | sort -k2 -r | head -n 1)
            echo -e "$count \t $space"
    done <<< $output
}



# Programa principal:

    while [ "$1" != "" ]; do
        case $1 in 
            -h | --help )
                usage
                exit
                ;;
            -inv )
                invert=$((1))
                ;;

        esac
        shift
    done
    
    filesysteminfo

# Fin Programa
exit 0
