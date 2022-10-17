#!/bin/bash

# sysinfo - Un script que informa del estado del sistema

##### Constantes

TITLE="Información del sistema para $HOSTNAME"

RIGHT_NOW=$(date +"%x %r%Z")
TIME_STAMP="Actualizada el $RIGHT_NOW por $USER"


##### Estilos


TEXT_BOLD=$(tput bold)

TEXT_GREEN=$(tput setaf 2)

TEXT_RESET=$(tput sgr0)


##### Funciones


system_info() {
    # Función de stub temporal
    echo "${TEXT_BOLD}Función system_info:${TEXT_RESET}"
    echo -n -e "\tOS: "; uname -r
    echo -n -e "\tArchitecture: "; uname -m
    echo ""
}


show_uptime() {
    # Función de stub temporal
    echo "${TEXT_BOLD}Función show_uptime:${TEXT_RESET}"
    echo -n -e "\t"; uptime -p
    echo ""
}


drive_space() {
    # Función de stub temporal
    echo "${TEXT_BOLD}Función drive_space:${TEXT_RESET}"
    echo -n -e "\tEspacio disponible en disco: "
    sudo df -h / | tail +2 | awk '{print $4}'
    echo ""
}


home_space() {
    # Función de stub temporal
    echo "${TEXT_BOLD}Función home_space${TEXT_RESET}"
    echo -n -e "\tEspacio ocupado por /home: "
    sudo df -h ~ | tail +2 |awk '{print $3}'
    echo ""
}


##### Programa principal


cat << _EOF_

$TEXT_BOLD$TITLE$TEXT_RESET


$TEXT_GREEN$TIME_STAMP$TEXT_RESET

_EOF_

system_info
show_uptime
drive_space
home_space