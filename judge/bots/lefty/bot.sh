# Versión Python del script de ejecución.

dir=$(realpath --relative-to=`pwd` "$(dirname "$0")")
pyscript=LeftyBot  

usage() {
  echo "$0 (build|exec)" 
}

# Compilar el bot
build() {
  ulimit -Sv 131072 
  python -m py_compile $dir/*.py
}

# Ejecutar el bot
execute() {
  ulimit -Sv 131072 
  python $dir/$pyscript.pyc
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