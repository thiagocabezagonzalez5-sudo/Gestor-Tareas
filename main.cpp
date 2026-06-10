#include "config.h"
#include "usuarios.h"
#include "tareas.h"
#include "visual.h"
#include "utils.h"
#include "archivos.h"
#include "menu.h"

int main(){
    configurarConsola();

    bool salir = false;
    bool salirUsuario = false;

    do{
        int opcionUsuario = menuUsuario();

        switch(opcionUsuario){
            case 1:
                salir = false;
                ingresarUsuario();

                if(usuarioActual == ""){
                    salir = true;
                }
                break;

            case 0:
                salir = true;
                salirUsuario = true;
                break;
        }

        while(!salir){
            int opcion = mostrarMenu();

            switch(opcion){
                case 1:
                    verTareas();
                    break;

                case 2:
                    agregarTareaInteractiva();
                    break;

                case 3:
                    modificarTarea();
                    break;

                case 4:
                    borrarTarea();
                    break;

                case 0:
                    guardarTareas();
                    salir = true;
                    usuarioActual = "";
                    archivoTareasGlobal = "";
                    break;

                default:
                    escribirColor(ROJO, "\nError, ingrese una opcion valida.\n");
                    pausar();
                    break;
            }
        }

    }while(!salirUsuario);

    return 0;
}
