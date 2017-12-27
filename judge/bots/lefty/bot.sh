# Versión Python del script de ejecución.

dir=$(realpath --relative-to=`pwd` "$(dirname "$0")")
pyscript=LeftyBot  

usage() {
  echo "$0 (build|exec)" 
}

# Compilar el bot
# TODO : limitaciones kernel mem, etc. ¿--network none?
build() {
  CONTAINER=$(docker create --memory 500M -w /myDir python:2.7-alpine3.7 \
              python -m py_compile *.py) # <= Compilation command
  docker cp $dir/ $CONTAINER:/myDir/
  docker start --attach $CONTAINER    # Also print stdout stderr
  if [ $? -eq 0 ] ; then
    docker cp $CONTAINER:/myDir/. $dir
  fi
  docker rm $CONTAINER
}

# Ejecutar el bot
execute() {
  CONTAINER=$(docker create --rm -a stdin -a stdout -i --memory 300M --network none python:2.7-alpine3.7 \
              python $pyscript.pyc) # <= Execution command
  docker cp $dir/ $CONTAINER:/
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