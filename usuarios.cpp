#include "usuarios.h"
#include "config.h"
#include "utils.h"
#include "visual.h"
#include "archivos.h"

#include <iostream>
#include <algorithm>
#include <windows.h>
#include <conio.h>

std::string archivoTareasGlobal = "";
std::string usuarioActual = "";

std::string construirRutaArchivoTareas(const std::string& nombreUsuario){
    return CARPETA_USUARIOS + "\\" + nombreUsuario + "\\" + ARCHIVO_TAREAS;
}

void crearCarpeta(const std::string& nombreUsuario){
    CreateDirectory(CARPETA_USUARIOS.c_str(), NULL);

    std::string carpetaUsuario = CARPETA_USUARIOS + "\\" + nombreUsuario;

    if(CreateDirectory(carpetaUsuario.c_str(), NULL)){
        std::cout << "Carpeta creada para el usuario: " << nombreUsuario << std::endl;
    }
}

std::vector<std::string> obtenerUsuarios(){
    std::vector<std::string> usuarios;

    CreateDirectory(CARPETA_USUARIOS.c_str(), NULL);

    std::string patron = CARPETA_USUARIOS + "\\*";
    WIN32_FIND_DATA datos;
    HANDLE hFind = FindFirstFile(patron.c_str(), &datos);

    if(hFind == INVALID_HANDLE_VALUE){
        return usuarios;
    }

    do{
        std::string nombre = datos.cFileName;

        if((datos.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) &&
           nombre != "." && nombre != ".."){
            usuarios.push_back(nombre);
        }

    }while(FindNextFile(hFind, &datos));

    FindClose(hFind);

    std::sort(usuarios.begin(), usuarios.end());

    return usuarios;
}

void mostrarUsuariosPaginados(const std::vector<std::string>& usuarios, int paginaActual, int usuariosPorPagina){
    int totalUsuarios = (int)usuarios.size();
    int totalPaginas = (totalUsuarios + usuariosPorPagina - 1) / usuariosPorPagina;

    if(totalPaginas == 0){
        totalPaginas = 1;
    }

    int inicio = paginaActual * usuariosPorPagina;
    int fin = inicio + usuariosPorPagina;

    if(fin > totalUsuarios){
        fin = totalUsuarios;
    }

    linea(0);
    centrarTextoColor(AZUL, "SELECCION DE USUARIO");
    centrarTexto("Pagina " + std::to_string(paginaActual + 1) + " de " + std::to_string(totalPaginas));
    linea(1);

    if(usuarios.empty()){
        textoIzquierda("No hay usuarios creados.");
    }else{
        for(int i = inicio; i < fin; i++){
            textoIzquierda("[" + std::to_string(i + 1) + "] " + usuarios[i]);
        }
    }

    linea(2);

    std::cout << "\n[N] Siguiente pagina   [P] Pagina anterior";
    std::cout << "\n[C] Crear nuevo usuario   [0] Salir";
    std::cout << "\n\nOpcion o numero de usuario: ";
}

void ingresarUsuario(){
    std::vector<std::string> usuarios = obtenerUsuarios();

    int paginaActual = 0;
    int usuariosPorPagina = 7;
    bool salir = false;

    do{
        limpiarPantalla();

        int totalUsuarios = (int)usuarios.size();
        int totalPaginas = (totalUsuarios + usuariosPorPagina - 1) / usuariosPorPagina;

        if(totalPaginas == 0){
            totalPaginas = 1;
        }

        if(paginaActual >= totalPaginas){
            paginaActual = totalPaginas - 1;
        }

        if(paginaActual < 0){
            paginaActual = 0;
        }

        mostrarUsuariosPaginados(usuarios, paginaActual, usuariosPorPagina);

        std::string entrada = leerTexto("");

        if(entrada == "n" || entrada == "N"){
            if(paginaActual < totalPaginas - 1){
                paginaActual++;
            }
        }else if(entrada == "p" || entrada == "P"){
            if(paginaActual > 0){
                paginaActual--;
            }
        }else if(entrada == "c" || entrada == "C"){
            std::string nombre = leerTexto("\nIngrese el nombre del nuevo usuario: ");

            if(nombre == ""){
                escribirColor(ROJO, "\nEl nombre no puede estar vacio.\n");
                pausar();
            }else{
                usuarioActual = nombre;
                crearCarpeta(nombre);
                archivoTareasGlobal = construirRutaArchivoTareas(nombre);
                cargarTareas();

                escribirColor(VERDE, "\nUsuario ingresado correctamente.\n");
                std::cout << "Archivo de tareas: " << archivoTareasGlobal << std::endl;
                pausar();
                salir = true;
            }
        }else if(entrada == "0"){
            salir = true;
        }else{
            int numeroUsuario = 0;

            try{
                numeroUsuario = std::stoi(entrada);
            }catch(...){
                numeroUsuario = 0;
            }

            if(numeroUsuario >= 1 && numeroUsuario <= (int)usuarios.size()){
                usuarioActual = usuarios[numeroUsuario - 1];
                archivoTareasGlobal = construirRutaArchivoTareas(usuarioActual);
                cargarTareas();

                escribirColor(VERDE, "\nUsuario ingresado correctamente.\n");
                std::cout << "Usuario actual: " << usuarioActual << std::endl;
                std::cout << "Archivo de tareas: " << archivoTareasGlobal << std::endl;
                pausar();
                salir = true;
            }else{
                escribirColor(ROJO, "\nOpcion invalida.\n");
                pausar();
            }
        }

    }while(!salir);
}

int menuUsuario(){
    limpiarPantalla();

    linea(0);
    centrarTexto("GESTOR DE TAREAS 6761 - MENU DE USUARIOS");
    linea(1);
    textoIzquierda("[1] Seleccionar o crear usuario.");
    textoIzquierda("[0] Salir.");
    linea(2);

    return leerEntero("Opcion: ", 0, 1);
}
