# Versión Java del script de ejecución.

dir=$(realpath --relative-to=`pwd` "$(dirname "$0")")
java_class=HunterBot  

usage() {
  echo "$0 (build|exec)" 
}

# Compilar el bot
build() {
  ulimit -Sv unlimited  
  cd $dir/
  javac $java_class.java
}

# Ejecutar el bot
execute() {
  ulimit -Sv unlimited
  java -cp $dir $java_class -Xmx256m
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