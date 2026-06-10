#include "menu.h"
#include "utils.h"
#include "visual.h"

int mostrarMenu(){
    limpiarPantalla();

    linea(0);
    centrarTexto("GESTOR DE TAREAS 6761");
    linea(1);
    textoIzquierda("[1] Ver tareas.");
    textoIzquierda("[2] Agregar tarea.");
    textoIzquierda("[3] Modificar tarea.");
    textoIzquierda("[4] Borrar tarea.");
    textoIzquierda("[0] Salir.");
    linea(2);

    return leerEntero("Opcion: ", 0, 4);
}
