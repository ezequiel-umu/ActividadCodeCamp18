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
  CONTAINER=$(docker create --rm -a stdin -a stdout -i --memory 300M openjdk:alpine java -cp $dir $java_class)
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