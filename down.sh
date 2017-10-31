#!/bin/bash

set -e

#Software necesario
software=( docker-compose docker)

for i in "${software[@]}"; do
    if ! hash $i 2>/dev/null; then
            echo -e "missing $i"
            exit 1
    fi
done


docker-compose down
