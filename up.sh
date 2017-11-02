#!/bin/bash

set -e

#Software necesario
software=( docker-compose docker basename pwd xargs )

for i in "${software[@]}"; do
    if ! hash $i 2>/dev/null; then
            echo -e "missing $i"
            exit 1
    fi
done


while getopts ":r" opt; do
  case $opt in
    r)
      echo "Se va a borar todo el contenido persistente de la plataforma. Pulsa cualquier tecla para continuar"
      read a
      docker-compose config  --services | docker-compose rm -vf
      docker volume ls -q | grep -i $(basename $(pwd)) | xargs docker volume rm
      exit 0
      ;;
    \?)
      echo "Invalid option: -$OPTARG" >&2
      exit 1
      ;;
  esac
done

docker-compose up --build -d
