#!/bin/bash

# Macros:
    # Colors
    clear='\033[0m'
    blue='\033[0;38;5;57m'

# Funciones:
function usage() {
    echo    "Usage: filesysteminfo.sh [-inv] para invertir"
    echo    "       filesysteminfo.sh [--no-header] para quitar la cabecera"
    echo    "       filesysteminfo.sh [-h] [--help] para imprimir ayuda"
}

function tabla() {
    output=$(sudo df --all --output=fstype | sort | uniq | tail +3) 
    if [ "$invert" = 1 ]; then
        output=$(sudo df --all --output=fstype | sort | uniq | tail +3 | sort -r)
    fi
    while read -r line; do
            count=$(sudo df --all --output=fstype,target,size | grep $line | wc -l)
            space=$(sudo df --all -T | grep $line | sort -k4 -r | head -n 1 | awk '{print $2, " ", $7, " ", $4}')
            total_space=$(sudo df --all -T | grep $line | 
                          awk 'BEGIN {printf "%s",$4} {sum+=$4} END {printf "%d",sum}')
            ##id_dispositivo=$(sudo df --all -T | grep $line | sort -k3 -r | head -n 1 | 
            ##               awk '{print $1}' | xargs -I{} sudo stat --format "%t %T" {} 2> /dev/null) 
            ##if [ "$id_dispositivo" != "" ]; then
            ##    echo -e "$space\t\t$count\t$total_space\t\t$id_dispositivo"
            ##else
            ##    echo -e "$space\t\t$count\t$total_space\t\t*\t*"
            ##fi
            id_dispositivo=$(sudo df --all -T | grep $line | sort -k3 -r | head -n 1 | 
                            awk '{print $1}' | xargs -I{} ls -l {} 2> /dev/null |
                            awk '{print $5 $6}' | tr ',' ' ')
            if [ "$id_dispositivo" != "" ]; then
                echo -e "$space\t\t$count\t$total_space\t\t$id_dispositivo"
            else 
                echo -e "$space\t\t$count\t$total_space\t\t*\t*"
            fi
    done <<< $output
}

function modificacion() {
    ps -A -o size --no-headers | awk 'BEGIN {printf "%s",$1} {sum+=$1} END {printf "%d\n",sum}'
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
        --no-header )
            tabla
            exit 0
            ;;
    esac
    shift
done
    
tabla | column -t -N "Type,Mounted on,Usage,Repetitions,TotalUsage,Major ID,Minor ID"
#modificacion

# Fin Programa
exit 0 
