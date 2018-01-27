# Versión Python del script de ejecución.

dir=$(realpath --relative-to=`pwd` "$(dirname "$0")")
program=main  

usage() {
  echo "$0 (build|exec)" 
}

# Compilar el bot
# TODO : limitaciones kernel mem, etc. ¿--network none?
build() {
  CONTAINER=$(docker create --memory 500M -w /myDir gcc \
              /bin/bash -c "make clean && make") # <= Compilation command
  docker cp $dir/ $CONTAINER:/myDir/
  docker start --attach $CONTAINER    # Also print stdout stderr
  if [ $? -eq 0 ] ; then
    docker cp $CONTAINER:/myDir/. $dir
  fi
  docker rm $CONTAINER
}

# Ejecutar el bot
execute() {
  CONTAINER=$(docker create --rm -a stdin -a stdout -i --memory 300M --network none debian:stretch-slim \
              ./$program) # <= Execution command
  docker cp $dir/$program $CONTAINER:/
  docker start --attach --interactive $CONTAINER
}

if [ $# -ne 1 ]
then
  usage
else
  case "$1" in
  "exec")
    execute
  ;;
  "build")
    build
  ;;
  *)
    usage
  ;;
  esac
fi