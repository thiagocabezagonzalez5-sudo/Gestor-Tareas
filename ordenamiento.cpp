#include "ordenamiento.h"

#include <algorithm>

bool comparar(int tipo, const Tarea& a, const Tarea& b){
    switch(tipo){
        case 1:
            return a.id < b.id;

        case 2:
            return a.importancia > b.importancia;

        case 3:
            return a.nombre < b.nombre;

        case 4:
            return a.fecha_entrega < b.fecha_entrega;

        default:
            return a.id < b.id;
    }
}

void ordenar(int tipo){
    std::sort(tareas.begin(), tareas.end(), [tipo](const Tarea& a, const Tarea& b){
        return comparar(tipo, a, b);
    });
}
