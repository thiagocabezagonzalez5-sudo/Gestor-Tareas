#include "tareas.h"
#include "config.h"
#include "utils.h"
#include "visual.h"
#include "validaciones.h"
#include "archivos.h"
#include "ordenamiento.h"

#include <iostream>
#include <conio.h>

std::vector<Tarea> tareas;

int generarNuevoId(){
    int mayorId = 0;

    for(int i = 0; i < (int)tareas.size(); i++){
        if(tareas[i].id > mayorId){
            mayorId = tareas[i].id;
        }
    }

    return mayorId + 1;
}

void agregarTarea(int id, const std::string& nombre, const std::string& descripcion,
                  int importancia, const std::string& fecha_entrega, bool completada){
    Tarea nueva;

    nueva.id = id;
    nueva.nombre = nombre;
    nueva.descripcion = descripcion;
    nueva.importancia = importancia;
    nueva.fecha_entrega = fecha_entrega;
    nueva.completada = completada;

    tareas.push_back(nueva);
}

int buscarIndicePorId(int id){
    for(int i = 0; i < (int)tareas.size(); i++){
        if(tareas[i].id == id){
            return i;
        }
    }

    return -1;
}

std::string mostrarEstrellas(int importancia){
    std::string estrellas = "";

    for(int i = 0; i < importancia; i++){
        estrellas += "*";
    }

    for(int i = importancia; i < 5; i++){
        estrellas += "-";
    }

    return estrellas;
}

std::string mostrarEstado(bool completada){
    if(completada){
        return "OK";
    }

    return "Pend.";
}

void agregarTareaInteractiva(){
    int id = generarNuevoId();

    std::string nombre = "";
    std::string descripcion = "";
    std::string fecha = "";
    std::string importanciaTexto = "";
    std::string error = "";

    do{
        mostrarFormularioAgregarTarea(nombre, descripcion, fecha, importanciaTexto, error);

        nombre = leerTexto("Ingrese el nombre: ");

        if(nombre == ""){
            error = "El nombre no puede estar vacio.";
        }else if((int)nombre.length() > COL_NOMBRE){
            error = "El nombre no puede superar " + std::to_string(COL_NOMBRE) + " caracteres.";
            nombre = "";
        }else{
            error = "";
        }

    }while(error != "");

    do{
        mostrarFormularioAgregarTarea(nombre, descripcion, fecha, importanciaTexto, error);

        descripcion = leerTexto("Ingrese la descripcion: ");

        if(descripcion == ""){
            error = "La descripcion no puede estar vacia.";
        }else if((int)descripcion.length() > COL_DESCRIPCION){
            error = "La descripcion no puede superar " + std::to_string(COL_DESCRIPCION) + " caracteres.";
            descripcion = "";
        }else{
            error = "";
        }

    }while(error != "");

    do{
        mostrarFormularioAgregarTarea(nombre, descripcion, fecha, importanciaTexto, error);

        fecha = leerFechaAutomatica("Ingrese la fecha [aaaammdd]: ");

        if(!validarFecha(fecha)){
            error = "Fecha invalida. Ejemplo correcto: 20260520.";
            fecha = "";
        }else{
            error = "";
        }

    }while(error != "");

    do{
        mostrarFormularioAgregarTarea(nombre, descripcion, fecha, importanciaTexto, error);

        int importancia = leerEntero("Ingrese la importancia [1-5]: ", 1, 5);
        importanciaTexto = std::to_string(importancia);

        error = "";

    }while(error != "");

    int importanciaFinal = std::stoi(importanciaTexto);

    agregarTarea(id, nombre, descripcion, importanciaFinal, fecha, false);
    guardarTareas();

    mostrarFormularioAgregarTarea(nombre, descripcion, fecha, importanciaTexto, "");
    std::cout << "Tarea agregada correctamente.\n";

    pausar();
}

void mostrarTareasPaginadas(int paginaActual, int tareasPorPagina){
    std::string completada, estrellas;

    int totalTareas = (int)tareas.size();
    int totalPaginas = (totalTareas + tareasPorPagina - 1) / tareasPorPagina;

    if(totalPaginas == 0){
        totalPaginas = 1;
    }

    int inicio = paginaActual * tareasPorPagina;
    int fin = inicio + tareasPorPagina;

    if(fin > totalTareas){
        fin = totalTareas;
    }

    linea(0);
    centrarTexto("GESTOR TAREAS 6761");
    centrarTexto("Pagina " + std::to_string(paginaActual + 1) + " de " + std::to_string(totalPaginas));
    lineaTabla(1);

    filaTabla("ID", "NOMBRE", "DESCRIPCION", "IMPOR", "ENTREGA", "ESTADO");
    lineaTabla(0);

    if(tareas.empty()){
        filaTabla("-", "Sin tareas", "-", "-", "-", "-");
        lineaTabla(2);
        centrarTexto("Thiago Cabeza Gonzalez - 2026");
        linea(2);
        return;
    }

    for(int i = inicio; i < fin; i++){
        Tarea t = tareas[i];

        completada = mostrarEstado(t.completada);
        estrellas = mostrarEstrellas(t.importancia);

        filaTabla(
            std::to_string(t.id),
            t.nombre,
            t.descripcion,
            estrellas,
            t.fecha_entrega,
            completada
        );

        if(i == fin - 1){
            lineaTabla(2);
        }else{
            lineaTabla(0);
        }
    }

    centrarTexto("Thiago Cabeza Gonzalez - 2026");
    linea(2);
}

void verTareas(){
    int tecla;
    bool volver = false;

    int paginaActual = 0;
    int tareasPorPagina = 5;

    do{
        limpiarPantalla();

        int totalTareas = (int)tareas.size();
        int totalPaginas = (totalTareas + tareasPorPagina - 1) / tareasPorPagina;

        if(totalPaginas == 0){
            totalPaginas = 1;
        }

        if(paginaActual >= totalPaginas){
            paginaActual = totalPaginas - 1;
        }

        if(paginaActual < 0){
            paginaActual = 0;
        }

        mostrarTareasPaginadas(paginaActual, tareasPorPagina);
        mostrarOpcionesVerTareas();

        tecla = _getch();

        switch(tecla){
            case 'n':
            case 'N':
                if(paginaActual < totalPaginas - 1){
                    paginaActual++;
                }
                break;

            case 'p':
            case 'P':
                if(paginaActual > 0){
                    paginaActual--;
                }
                break;

            case '1':
                ordenar(1);
                paginaActual = 0;
                break;

            case '2':
                ordenar(2);
                paginaActual = 0;
                break;

            case '3':
                ordenar(3);
                paginaActual = 0;
                break;

            case '4':
                ordenar(4);
                paginaActual = 0;
                break;

            case 'd':
            case 'D':
                pedirDetalleTarea();
                break;

            case 27:
                volver = true;
                break;

            default:
                std::cout << "\nOpcion invalida.\n";
                pausar();
                break;
        }

    }while(!volver);
}

void pedirDetalleTarea(){
    if(tareas.empty()){
        std::cout << "\nNo hay tareas para mostrar.\n";
        pausar();
        return;
    }

    limpiarPantalla();

    int id = leerEntero("Ingrese el ID de la tarea que quiere ver en detalle: ", 1, 999999);
    int indice = buscarIndicePorId(id);

    if(indice == -1){
        std::cout << "\nNo existe una tarea con ese ID.\n";
        pausar();
    }else{
        mostrarDetalleTarea(tareas[indice]);
    }
}

void mostrarDetalleTarea(const Tarea& t){
    int tecla;

    do{
        limpiarPantalla();

        linea(0);
        centrarTextoColor(AZUL, "DETALLE DE TAREA");
        linea(1);

        textoIzquierda("ID: " + std::to_string(t.id));
        textoIzquierda("Nombre: " + t.nombre);
        textoIzquierda("Descripcion: " + t.descripcion);
        textoIzquierda("Importancia: " + mostrarEstrellas(t.importancia));
        textoIzquierda("Fecha de entrega: " + t.fecha_entrega);
        textoIzquierda("Estado: " + mostrarEstado(t.completada));

        linea(2);

        std::cout << "\n[ESC] Volver a la lista";

        tecla = _getch();

    }while(tecla != 27);
}

int seleccionarTareaPaginada(const std::string& titulo, const std::string& accion){
    int paginaActual = 0;
    int tareasPorPagina = 5;
    bool salir = false;

    if(tareas.empty()){
        limpiarPantalla();
        escribirColor(NEGRITA, "No hay tareas disponibles.\n");
        pausar();
        return -1;
    }

    do{
        limpiarPantalla();

        int totalTareas = (int)tareas.size();
        int totalPaginas = (totalTareas + tareasPorPagina - 1) / tareasPorPagina;

        if(totalPaginas == 0){
            totalPaginas = 1;
        }

        if(paginaActual >= totalPaginas){
            paginaActual = totalPaginas - 1;
        }

        if(paginaActual < 0){
            paginaActual = 0;
        }

        linea(0);
        centrarTextoColor(MAGENTA, titulo);
        centrarTexto("Pagina " + std::to_string(paginaActual + 1) + " de " + std::to_string(totalPaginas));
        lineaTabla(1);

        filaTabla("ID", "NOMBRE", "DESCRIPCION", "IMPOR", "ENTREGA", "ESTADO");
        lineaTabla(0);

        int inicio = paginaActual * tareasPorPagina;
        int fin = inicio + tareasPorPagina;

        if(fin > totalTareas){
            fin = totalTareas;
        }

        for(int i = inicio; i < fin; i++){
            filaTabla(
                std::to_string(tareas[i].id),
                tareas[i].nombre,
                tareas[i].descripcion,
                mostrarEstrellas(tareas[i].importancia),
                tareas[i].fecha_entrega,
                mostrarEstado(tareas[i].completada)
            );

            if(i == fin - 1){
                lineaTabla(2);
            }else{
                lineaTabla(0);
            }
        }

        linea(2);

        std::cout << "\n[N] Siguiente pagina   [P] Pagina anterior   [ESC] Volver";
        std::cout << "\nIngrese el ID de la tarea a " << accion << ": ";

        std::string entrada = leerTexto("");

        if(entrada == "n" || entrada == "N"){
            if(paginaActual < totalPaginas - 1){
                paginaActual++;
            }
        }else if(entrada == "p" || entrada == "P"){
            if(paginaActual > 0){
                paginaActual--;
            }
        }else if(entrada == "esc" || entrada == "ESC"){
            salir = true;
        }else{
            int id = 0;

            try{
                id = std::stoi(entrada);
            }catch(...){
                id = 0;
            }

            int indice = buscarIndicePorId(id);

            if(indice == -1){
                escribirColor(ROJO, "\nNo existe una tarea con ese ID.\n");
                pausar();
            }else{
                return indice;
            }
        }

    }while(!salir);

    return -1;
}

void modificarTarea(){
    int indice = seleccionarTareaPaginada("MODIFICAR TAREA", "modificar");

    if(indice == -1){
        return;
    }

    bool salir = false;

    do{
        limpiarPantalla();

        linea(0);
        centrarTextoColor(MAGENTA, "MODIFICAR TAREA");
        linea(1);
        textoIzquierda("ID: " + std::to_string(tareas[indice].id));
        textoIzquierda("Nombre actual: " + tareas[indice].nombre);
        textoIzquierda("Descripcion actual: " + tareas[indice].descripcion);
        textoIzquierda("Importancia actual: " + mostrarEstrellas(tareas[indice].importancia));
        textoIzquierda("Fecha actual: " + tareas[indice].fecha_entrega);
        textoIzquierda("Estado actual: " + mostrarEstado(tareas[indice].completada));
        linea(1);
        textoIzquierda("[1] Nombre");
        textoIzquierda("[2] Descripcion");
        textoIzquierda("[3] Importancia");
        textoIzquierda("[4] Fecha de entrega");
        textoIzquierda("[5] Cambiar estado");
        textoIzquierda("[0] Volver");
        linea(2);

        int opcion = leerEntero("Opcion: ", 0, 5);

        switch(opcion){
            case 1:{
                std::string nuevoNombre = leerTexto("Nuevo nombre: ");

                if(nuevoNombre != "" && (int)nuevoNombre.length() <= COL_NOMBRE){
                    tareas[indice].nombre = nuevoNombre;
                    guardarTareas();
                    escribirColor(VERDE, "\nNombre modificado correctamente.\n");
                }else{
                    escribirColor(ROJO, "\nNombre invalido.\n");
                }

                pausar();
                break;
            }

            case 2:{
                std::string nuevaDescripcion = leerTexto("Nueva descripcion: ");

                if(nuevaDescripcion != "" && (int)nuevaDescripcion.length() <= COL_DESCRIPCION){
                    tareas[indice].descripcion = nuevaDescripcion;
                    guardarTareas();
                    escribirColor(VERDE, "\nDescripcion modificada correctamente.\n");
                }else{
                    escribirColor(ROJO, "\nDescripcion invalida.\n");
                }

                pausar();
                break;
            }

            case 3:
                tareas[indice].importancia = leerEntero("Nueva importancia [1-5]: ", 1, 5);
                guardarTareas();
                escribirColor(VERDE, "\nImportancia modificada correctamente.\n");
                pausar();
                break;

            case 4:{
                std::string fecha;

                do{
                    fecha = leerFechaAutomatica("Nueva fecha [aaaammdd]: ");

                    if(!validarFecha(fecha)){
                        escribirColor(ROJO, "Fecha invalida. Ejemplo correcto: 20260520\n");
                    }

                }while(!validarFecha(fecha));

                tareas[indice].fecha_entrega = fecha;
                guardarTareas();
                escribirColor(VERDE, "\nFecha modificada correctamente.\n");
                pausar();
                break;
            }

            case 5:
                tareas[indice].completada = !tareas[indice].completada;
                guardarTareas();
                escribirColor(VERDE, "\nEstado modificado correctamente.\n");
                pausar();
                break;

            case 0:
                salir = true;
                break;
        }

    }while(!salir);
}

void borrarTarea(){
    int indice = seleccionarTareaPaginada("BORRAR TAREA", "borrar");

    if(indice == -1){
        return;
    }

    limpiarPantalla();

    linea(0);
    centrarTextoColor(ROJO, "BORRAR TAREA");
    linea(1);
    textoIzquierda("ID: " + std::to_string(tareas[indice].id));
    textoIzquierda("Nombre: " + tareas[indice].nombre);
    textoIzquierda("Descripcion: " + tareas[indice].descripcion);
    linea(2);

    std::cout << "\nSeguro que desea borrar esta tarea? [S/N]: ";

    int tecla = _getch();

    if(tecla == 's' || tecla == 'S'){
        tareas.erase(tareas.begin() + indice);
        guardarTareas();

        std::cout << "\n\nTarea borrada correctamente.\n";
    }else{
        std::cout << "\n\nOperacion cancelada.\n";
    }

    pausar();
}
