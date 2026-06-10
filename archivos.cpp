#include "archivos.h"
#include "tareas.h"
#include "usuarios.h"
#include "utils.h"
#include "validaciones.h"

#include <iostream>
#include <fstream>
#include <sstream>

void guardarTareas(){
    if(archivoTareasGlobal == ""){
        std::cout << "\nNo hay usuario seleccionado. No se pueden guardar tareas.\n";
        return;
    }

    std::ofstream archivo(archivoTareasGlobal.c_str());

    if(!archivo.is_open()){
        std::cout << "\nNo se pudo abrir el archivo para guardar tareas.\n";
        return;
    }

    for(int i = 0; i < (int)tareas.size(); i++){
        archivo << tareas[i].id << ";"
                << limpiarCampoArchivo(tareas[i].nombre) << ";"
                << limpiarCampoArchivo(tareas[i].descripcion) << ";"
                << tareas[i].importancia << ";"
                << tareas[i].fecha_entrega << ";"
                << tareas[i].completada << std::endl;
    }

    archivo.close();
}

void cargarTareas(){
    if(archivoTareasGlobal == ""){
        std::cout << "\nNo hay usuario seleccionado. No se pueden cargar tareas.\n";
        return;
    }

    std::ifstream archivo(archivoTareasGlobal.c_str());

    tareas.clear();

    if(!archivo.is_open()){
        return;
    }

    std::string lineaArchivo;

    while(std::getline(archivo, lineaArchivo)){
        std::stringstream ss(lineaArchivo);

        std::string idTexto;
        std::string nombre;
        std::string descripcion;
        std::string importanciaTexto;
        std::string fecha;
        std::string completadaTexto;

        std::getline(ss, idTexto, ';');
        std::getline(ss, nombre, ';');
        std::getline(ss, descripcion, ';');
        std::getline(ss, importanciaTexto, ';');
        std::getline(ss, fecha, ';');
        std::getline(ss, completadaTexto, ';');

        if(idTexto != "" && importanciaTexto != "" && completadaTexto != ""){
            Tarea t;

            try{
                t.id = std::stoi(idTexto);
                t.nombre = nombre;
                t.descripcion = descripcion;
                t.importancia = std::stoi(importanciaTexto);
                t.fecha_entrega = convertirFechaAFormatoPrograma(fecha);
                t.completada = std::stoi(completadaTexto) != 0;

                tareas.push_back(t);
            }catch(...){
                // Si una linea del archivo esta dañada, se ignora para no romper el programa.
            }
        }
    }

    archivo.close();
}
