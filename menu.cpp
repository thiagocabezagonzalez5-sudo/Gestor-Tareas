#include "menu.h"
#include "utils.h"
#include "config.h"
#include "visual.h"

#include <string>

int mostrarMenu(){
    std::string error = "";

    while(true){
        limpiarPantalla();

        linea(0);
        centrarTexto("TASKMANAGER.cpp");
        linea(1);
        textoIzquierda("[1] Ver tareas.");
        textoIzquierda("[2] Agregar tarea.");
        textoIzquierda("[3] Modificar tarea.");
        textoIzquierda("[4] Borrar tarea.");
        textoIzquierda("[0] Salir.");
        linea(2);

        if(error != ""){
            escribirColor(ROJO, "\n" + error + "\n");
        }

        int opcion = leerOpcionUnDigito("Opcion: ", 0, 4);

        if(opcion != -1){
            return opcion;
        }

        error = "Opcion invalida. Solo puede elegir 0, 1, 2, 3 o 4.";
    }
}
