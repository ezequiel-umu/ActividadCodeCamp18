# Versión Python del script de ejecución.

dir=$(realpath --relative-to=`pwd` "$(dirname "$0")")
program=main  

usage() {
  echo "$0 (build|exec)" 
}

# Compilar el bot
build() {
  ulimit 262144
  make clean
  make
}

# Ejecutar el bot
execute() {
  ulimit 262144
  ./$program
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