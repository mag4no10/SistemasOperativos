#!/bin/bash

# Macros:
    # Colors
    clear=$(tput sgr0)
    black=$(tput setaf 0)
    red=$(tput setaf 1)
    green=$(tput setaf 2)
    yellow=$(tput setaf 3)
    blue=$(tput setaf 4)
    magenta=$(tput setaf 5)
    cyan=$(tput setaf 6)
    white=$(tput setaf 7)

    bold=$(tput bold)

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
                echo -e "${red}$space\t\t${blue}$count\t${magenta}$total_space\t\t${cyan}$id_dispositivo${clear}"
            else 
                echo -e "${red}$space\t\t${blue}$count\t${magenta}$total_space\t\t${cyan}*\t*${clear}"
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
        --devicefiles )
            ;;
    esac
    shift
done
    
tabla | column -t -N "${bold}${red}Type,Mounted on,Usage,${blue}Repetitions,${magenta}TotalUsage,${cyan}Major ID,${cyan}Minor ID${clear}"
#modificacion

# Fin Programa
exit 0 
