#!/bin/bash

fn_fstypes() {

    echo -e "Primera funcion:\n";

    sudo df --output=fstype,target | grep -v snap | tail -n +2 | sort | uniq | awk '{print $1, $2}';

    echo "";
}

fn_total() {

    echo "Segunda funcion:";

    # Awk mode:
    # sudo df --output=fstype,size --block-size=K | grep tmpfs | awk 'BEGIN {printf "%s\n",$1} \
    # {sum += $2} END {printf "Espacio Total: %d KB\n",sum}'
    
    # Bash mode:
    output=$(sudo df --output=fstype,size | grep tmpfs | tr -s " " | cut  -d " " -f 2) 
    sum=0
    while read -r line
        do
            sum=$((sum+line))
    done <<< $output
    echo -e "\nEspacio Total: $sum KB";
}


echo "Llamando a la primera funcion"
fn_fstypes

echo "Llamando a la segunda funcion"
fn_total
