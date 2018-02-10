# Versión C++ del script de ejecución.

dir=$(realpath --relative-to=`pwd` "$(dirname "$0")")
program=main  

usage() {
  echo "$0 (build|exec container_name|finish container_name)" 
}

# Compilar el bot
# TODO : limitaciones kernel mem, etc. ¿--network none?
build() {
  echo $dir
  CONTAINER=$(docker create --memory 500M -w /myDir gcc \
              /bin/bash -c "make clean && make") # <= Compilation command
  docker cp $dir/. $CONTAINER:/myDir/
  docker start --attach $CONTAINER    # Also print stdout stderr
  if [ $? -eq 0 ] ; then
    docker cp $CONTAINER:/myDir/main $dir
  fi
  docker rm $CONTAINER
}

# Ejecutar el bot
execute() {
  CONTAINER=$(docker create -a stdin -a stdout --name "$1" -i --memory 300M --network none debian:stretch-slim \
              ./$program) # <= Execution command
  docker cp $dir/$program $CONTAINER:/
  docker start --attach --interactive $CONTAINER
}

# Finalizar y traer el debug
finish() {
  docker cp $1:/debug.log $dir > /dev/null
  docker rm -f "$1" > /dev/null
}

if [ $# -lt 1 ]
then
  usage
else
  case "$1" in
  "exec")
    if [ $# -lt 2 ]
    then
      usage
    else
      execute $2
    fi
  ;;
  "build")
    build
  ;;
  "finish") 
        if [ $# -lt 2 ]
    then
      usage
    else
      finish $2
    fi
  ;;
  *)
    usage
  ;;
  esac
fi