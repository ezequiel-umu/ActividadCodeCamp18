#include "IA.h"
#include "actions/Random.h"

IA::IA() {

}

Action * IA::commandAnt(Ant & ant, bool isConflict) {
    // Escribir aquí la lógica de la inteligencia artificial
    return new Random(ant);
}