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
    pink=$(tput setaf 201)
    bold=$(tput bold)

    # Variables

# Funciones:
function column_command() {
    if [ "$devicefiles" == 1 ] || [ "$users" != "" ]; then 
        column -t -N "${bold}${red}Type,Mounted on,Usage,${blue}Repetitions,${magenta}TotalUsage,${cyan}Major ID,${cyan}Minor ID,${pink}Opened Files${clear}"    
    else
        column -t -N "${bold}${red}Type,Mounted on,Usage,${blue}Repetitions,${magenta}TotalUsage,${cyan}Major ID,${cyan}Minor ID${clear}"
    fi
}

function usage() {
    echo    "Usage: filesysteminfo.sh [-inv] para invertir"
    echo    "       filesysteminfo.sh [--no-header] para quitar la cabecera"
    echo    "       filesysteminfo.sh [-h] [--help] para imprimir ayuda"
}

function errors() {
    echo -e "Error en el uso de parametros, para mas ayuda escribir \"./filesysteminfo [-h / --help]\""
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
            id_dispositivo=$(sudo df --all -T | grep $line | sort -k3 -r | head -n 1 | 
                            awk '{print $1}' | xargs -I{} ls -l {} 2> /dev/null |
                            awk '{print $5 $6}' | tr ',' ' ')
            permission=$(sudo df --all -T | grep $line | sort -k3 -r | head -n 1 | 
                        awk '{print $1}' | xargs -I{} ls -l {} 2> /dev/null |
                        awk '{print $1}')
            if [ "$users" != "" ]; then
                if [ "$id_dispositivo" != "" ]; then
                    sum=$((0))
                    for i in $users; do
                        if [[ $(getent passwd | grep $i) ]]; then
                            open_files=$(sudo df --all -T | grep $line | awk '{print $7}' | xargs -I{} sudo lsof -u $i +D {} 2> /dev/null | wc -l)
                            sum=$(($sum + $open_files))
                        else 
                            open_files=$((0))
                        fi
                    done
                    echo -e "${red}$space\t\t${blue}$count\t${magenta}$total_space\t\t${cyan}$id_dispositivo\t${pink}$open_files${clear}"
                fi
            else 
                if [ "$devicefiles" == 1 ]; then
                    if [ "$id_dispositivo" != "" ]; then
                        open_files=$(sudo df --all -T | grep $line | awk '{print $7}' | xargs -I{} lsof +D {} 2> /dev/null | wc -l)
                        echo -e "${red}$space\t\t${blue}$count\t${magenta}$total_space\t\t${cyan}$id_dispositivo\t${pink}$open_files${clear}"
                    fi
                else
                    if [ "$id_dispositivo" != "" ]; then
                        echo -e "${red}$space\t\t${blue}$count\t${magenta}$total_space\t\t${cyan}$id_dispositivo${clear}"
                    else 
                        echo -e "${red}$space\t\t${blue}$count\t${magenta}$total_space\t\t${cyan}*\t*${clear}"
                    fi
                fi
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
            tabla | column_command | tail +2
            exit 0
            ;;
        -devicefiles )
            devicefiles=$((1))
            ;;
        -u )
            users=$(echo -e "$@" | grep -o "\-u.*" | sed -e s/'-\w*$'// -e s/'-\w'// -e s/' '//)
            ;;
        -sopen )
            sopen=$((1))
            ;;
        -sdevice )
            sdevice=$((1))
            ;;    
#        * )
#            errors
#            exit 1
#            ;;
    esac
    shift
done

if [ "$sdevice" == 1 ]; then
    tabla | sort -k4
else
    tabla | column_command
fi

# Fin Programa
exit 0 