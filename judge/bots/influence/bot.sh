# Versión Python del script de ejecución.

dir=$(realpath --relative-to=`pwd` "$(dirname "$0")")
pyscript=MyBot  

usage() {
  echo "$0 (build|exec)" 
}

# Compilar el bot
build() {
  ulimit -Sv 262144 
  python -m py_compile $dir/*.py
}

# Ejecutar el bot
execute() {
  ulimit -Sv 262144 
  python $dir/$pyscript.pyc
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
  ;;
  *)
    usage
  ;;
  esac
fi