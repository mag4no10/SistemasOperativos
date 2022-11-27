#!/bin/bash

# Macros:
    # Colors
    CLEAR=$(tput sgr0)
    BLACK=$(tput setaf 0)
    RED=$(tput setaf 1)
    GREEN=$(tput setaf 2)
    YELLOW=$(tput setaf 3)
    BLUE=$(tput setaf 4)
    MAGENTA=$(tput setaf 5)
    CYAN=$(tput setaf 6)
    WHITE=$(tput setaf 7)
    PINK=$(tput setaf 201)
    BOLD=$(tput bold)

    # Variables
    invert=0
    noheader=0
    devicefiles=0
    sopen=0
    sdevice=0
    user_option=0
    counte=0

# Functions

#       Prints column command depending of
#       the parameters 
function column_command() {
    if [ "$devicefiles" == 1 ] || [ "$users" != "" ]; then 
        column -t -N "${BOLD}${RED}Type,Mounted on,Usage,${BLUE}Repetitions,${MAGENTA}TotalUsage,${CYAN}Major ID,${CYAN}Minor ID,${PINK}Opened Files${CLEAR}"    
    else
        column -t -N "${BOLD}${RED}Type,Mounted on,Usage,${BLUE}Repetitions,${MAGENTA}TotalUsage,${CYAN}Major ID,${CYAN}Minor ID${CLEAR}"
    fi
}

#       Print help [-h / --help]
function usage() {
    echo    "Usage: filesysteminfo.sh [-inv] para invertir"
    echo    "       filesysteminfo.sh [--no-header] para quitar la cabecera"
    echo    "       filesysteminfo.sh [-h] [--help] para imprimir ayuda"
    echo    "       filesysteminfo.sh [-sopen] para ordenar por archivos abiertos"
    echo    "       filesysteminfo.sh [-sdevice] para ordenar por numero de dispositivos del mismo tipo"
    echo    "       filesysteminfo.sh [-u] para filtrar por usuarios"
    exit 0
}

#       Prints different error
#       depending of the error code      
function errors() {
    if [ "$error_users" = 1 ]; then
        if [ "$usuario_erroneo" = 0 ]; then
            echo -e "Usuario no especificado, revise el uso del programa escribiendo -h o --help"
        else 
            echo -e "Usuario \"$usuario_erroneo\" no encontrado, revise el uso del programa escribiendo -h o --help"
        fi
    elif [ "$param_error" = 1 ]; then
        echo -e "Error de configuracion de parametros, revise el uso del programa escribiendo -h o --help"
    else
        echo -e "Error en el uso de parametros, para mas ayuda escribir \"./filesysteminfo [-h / --help]\""
    fi
    exit 1
}

#       Prints the chart and evaluates some 
#       arguments
function tabla() {
    output=$(sudo df --all --output=fstype | tail +2 | sort | uniq | tail +3) 
    if [ "$invert" = 1 ]; then
        output=$(sudo df --all --output=fstype | tail +2 | sort | uniq | tail +3 | sort -r)
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
                        open_files=$(sudo df --all -T | grep $line | awk '{print $1}' | xargs -I{} sudo lsof -a -u $i {} 2> /dev/null | wc -l)
                        sum=$(($sum + $open_files))
                    done
                    echo -e "${RED}$space\t\t${BLUE}$count\t${MAGENTA}$total_space\t\t${CYAN}$id_dispositivo\t${PINK}$sum${CLEAR}"
                fi
            else
                if [ "$devicefiles" == 1 ]; then
                    if [ "$id_dispositivo" != "" ]; then
                        open_files=$(sudo df --all -T | grep $line | awk '{print $1}' | xargs -I{} lsof -a {} 2> /dev/null | wc -l)
                        echo -e "${RED}$space\t\t${BLUE}$count\t${MAGENTA}$total_space\t\t${CYAN}$id_dispositivo\t${PINK}$open_files${CLEAR}"
                    fi
                else
                    if [ "$id_dispositivo" != "" ]; then
                        echo -e "${RED}$space\t\t${BLUE}$count\t${MAGENTA}$total_space\t\t${CYAN}$id_dispositivo${CLEAR}"
                    else 
                        echo -e "${RED}$space\t\t${BLUE}$count\t${MAGENTA}$total_space\t\t${CYAN}*\t*${CLEAR}"
                    fi
                fi
            fi
    done <<< $output
}

#       Evaluates parameters and redirects
#       depending of the choose
function param_manager() {
    if [ "$help" = 1 ]; then
        usage
    elif [ $(($sopen + $sdevice)) -gt 1 ] || [ "$sopen" = 1 ] && [ $(($devicefiles + $user_option)) -lt 1 ]; then
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
}

#       Prints the total size of the 
#       current running processes
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
            #users=$(echo -e "$@" | grep -o "\-u.*" | sed -e s/'-\w*$'// -e s/'-\w'// -e s/' '//)
            users=$(echo -e "$@" | grep -o "\-u.*" | sed -e s/'-\w'// -e s/'-\w.*'//)
            if [ "$users" == "" ]; then
                users=0
            fi
            for i in $users; do
                if ! [[ $(getent passwd | tr ':' ' '  | awk '{print $1}' | grep -w $i) ]]; then
                    usuario_erroneo=$i
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