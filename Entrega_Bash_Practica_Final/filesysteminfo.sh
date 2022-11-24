#!/bin/bash

# Macros:
    # Colors
    Clear=$(tput sgr0)
    Black=$(tput setaf 0)
    Red=$(tput setaf 1)
    Green=$(tput setaf 2)
    Yellow=$(tput setaf 3)
    Blue=$(tput setaf 4)
    Magenta=$(tput setaf 5)
    Cyan=$(tput setaf 6)
    White=$(tput setaf 7)
    Pink=$(tput setaf 201)
    Bold=$(tput bold)

    # Variables
    invert=0
    noheader=0
    devicefiles=0
    sopen=0
    sdevice=0
    user_option=0

# Funciones:
function column_command() {
    if [ "$devicefiles" == 1 ] || [ "$users" != "" ]; then 
        column -t -N "${Bold}${Red}Type,Mounted on,Usage,${Blue}Repetitions,${Magenta}TotalUsage,${Cyan}Major ID,${Cyan}Minor ID,${Pink}Opened Files${Clear}"    
    else
        column -t -N "${Bold}${Red}Type,Mounted on,Usage,${Blue}Repetitions,${Magenta}TotalUsage,${Cyan}Major ID,${Cyan}Minor ID${Clear}"
    fi
}

function usage() {
    echo    "Usage: filesysteminfo.sh [-inv] para invertir"
    echo    "       filesysteminfo.sh [--no-header] para quitar la cabecera"
    echo    "       filesysteminfo.sh [-h] [--help] para imprimir ayuda"
    exit 0
}

function errors() {
    if [ "$error_users" = 1 ]; then
        echo -e "Usuario/s no encontrado, revise el uso del programa escribiendo -h o --help"
    elif [ "$param_error" = 1 ]; then
        echo -e "Error de configuracion de parametros, revise el uso del programa escribiendo -h o --help"
    else
        echo -e "Error en el uso de parametros, para mas ayuda escribir \"./filesysteminfo [-h / --help]\""
    fi
    exit 1
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
                    sum=0
                    for i in $users; do
                        if [ "$line" == "btrfs" ]; then
                            open_files=$(sudo df --all -T | sudo lsof -u $i +D / 2> /dev/null | wc -l)
                        else
                            open_files=$(sudo df --all -T | grep $line | awk '{print $7}' | xargs -I{} sudo lsof -u $i +D {} 2> /dev/null | wc -l)
                        fi
                        sum=$(($sum + $open_files))
                    done
                    echo -e "${Red}$space\t\t${Blue}$count\t${Magenta}$total_space\t\t${Cyan}$id_dispositivo\t${Pink}$sum${Clear}"
                fi
            else 
                if [ "$devicefiles" == 1 ]; then
                    if [ "$id_dispositivo" != "" ]; then
                        open_files=$(sudo df --all -T | grep $line | awk '{print $7}' | xargs -I{} lsof +D {} 2> /dev/null | wc -l)
                        echo -e "${Red}$space\t\t${Blue}$count\t${Magenta}$total_space\t\t${Cyan}$id_dispositivo\t${Pink}$open_files${Clear}"
                    fi
                else
                    if [ "$id_dispositivo" != "" ]; then
                        echo -e "${Red}$space\t\t${Blue}$count\t${Magenta}$total_space\t\t${Cyan}$id_dispositivo${Clear}"
                    else 
                        echo -e "${Red}$space\t\t${Blue}$count\t${Magenta}$total_space\t\t${Cyan}*\t*${Clear}"
                    fi
                fi
            fi
    done <<< $output
}

function param_manager() {
    if [ "$help" = 1 ]; then
        usage
    elif [ $(($invert + $sopen + $sdevice)) -gt 1 ] || [ "$sopen" = 1 ] && [ $(($devicefiles + $user_option)) -lt 1 ]; then
        param_error=1
        errors
    elif [ "$sdevice" = 1 ]; then
        if [ "$noheader" = 1 ]; then
            tabla | sort -k4 | column_command | tail +2
        else
            tabla | sort -k4 | column_command
        fi
    elif [ "$sopen" = 1 ]; then
        if [ "$noheader" = 1 ]; then
            tabla | sort -k8 | column_command | tail +2
        else
            tabla | sort -k8 | column_command
        fi
    elif [ "$noheader" = 1 ]; then
        tabla | column_command | tail +2
    else
        tabla | column_command
    fi
    exit 0
}

function modificacion() {
    ps -A -o size --no-headers | awk 'BEGIN {printf "%s",$1} {sum+=$1} END {printf "%d\n",sum}'
}

# Programa principal:
while [ "$1" != "" ]; do
    case $1 in 
        -h | --help )
            help=1
            ;;
        -inv )
            invert=1
            ;;
        -noheader )
            noheader=1
            ;;
        -devicefiles )
            devicefiles=1
            ;;
        -sopen )
            sopen=1
            ;;
        -sdevice )
            sdevice=1
            ;;  
        -u )
            devicefiles=1; user_option=1
            users=$(echo -e "$@" | grep -o "\-u.*" | sed -e s/'-\w*$'// -e s/'-\w'// -e s/' '//)
            if [ "$users" == "" ]; then
                users=0
            fi
            for i in $users; do
                if ! [[ $(getent passwd | tr ':' ' '  | awk '{print $1}' | grep -w $i) ]]; then
                    error_users=1
                    errors
                fi
            done
            ;;
        -* )
            errors
            exit 1
            ;;
        * )
            if ! [ "$user_option" = 1 ] && ! [ "$1" == -* ]; then
                errors
            fi
            ;;
    esac
    shift
done

param_manager

# Fin Programa
exit 0 