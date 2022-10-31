#!/bin/bash

# Macros:
    # Colors
    clear='\033[0m'
    blue='\033[0;38;5;57m'

# Funciones:
usage() {
    echo    "Usage: filesysteminfo.sh [-inv] para invertir"
    echo    "       filesysteminfo.sh [--header] para añadir una cabecera"
    echo -e "       filesysteminfo.sh [--color] [--header] ${blue}para colorizar${clear}"
    echo    "       filesysteminfo.sh [-h] [--help] para imprimir ayuda"
}

filesysteminfo() {
    output=$(sudo df --all --output=fstype | sort | uniq | tail +3) 
    if [ "$invert" = 1 ]; then
        output=$(sudo df --all --output=fstype | sort | uniq | tail +3 | sort -r)
    fi
    while read -r line; do
            count=$(sudo df --all --output=fstype,target,size | grep $line | wc -l)
            space=$(sudo df --all --output=fstype,target,size | grep $line | sort -k3 -r | head -n 1)
            total_space=$(sudo df --all --output=fstype,target,size | grep $line | 
                          awk 'BEGIN {printf "%s",$3} {sum+=$3} END {printf "%d",sum}')
            echo -e "$space\t\t$count\t$total_space"
    done <<< $output
}



# Programa principal:

while [ "$1" != "" ]; do
    case $1 in 
        -h | --help )
            usage
            exit 0
            ;;
        -inv )
            invert=$((1))
            ;;
        --color )
            color=$((1))
            ;;
        --header )
            if [ "$color" = 1 ]; then
                color_output=$(filesysteminfo | column -t -N "Type,Mounted on,Usage,Repetitions,TotalUsage")
                echo -e "${blue}$color_output${clear}"
                exit
            fi
            filesysteminfo | column -t -N "Type,Mounted on,Usage,Repetitions,TotalUsage"
            exit 0
            ;;
    esac
    shift
done
    
filesysteminfo

# Fin Programa
exit 0
