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
            int numeroVisible = i - inicio + 1;
            textoIzquierda("[" + std::to_string(numeroVisible) + "] " + usuarios[i]);
        }
    }

    linea(2);

    std::cout << "\n[N] Siguiente pagina   [P] Pagina anterior";
    std::cout << "\n[C] Crear nuevo usuario   [0] Salir";
}

void ingresarUsuario(){
    std::vector<std::string> usuarios = obtenerUsuarios();

    int paginaActual = 0;
    int usuariosPorPagina = 7;
    bool salir = false;
    std::string error = "";

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

        int inicio = paginaActual * usuariosPorPagina;
        int fin = inicio + usuariosPorPagina;

        if(fin > totalUsuarios){
            fin = totalUsuarios;
        }

        int cantidadVisible = fin - inicio;

        mostrarUsuariosPaginados(usuarios, paginaActual, usuariosPorPagina);

        if(error != ""){
            escribirColor(ROJO, "\n" + error + "\n");
        }

        std::cout << "\nOpcion o numero de usuario: ";
        int tecla = _getch();

        if(tecla == 'n' || tecla == 'N'){
            if(paginaActual < totalPaginas - 1){
                paginaActual++;
                error = "";
            }else{
                error = "Ya esta en la ultima pagina.";
            }
        }else if(tecla == 'p' || tecla == 'P'){
            if(paginaActual > 0){
                paginaActual--;
                error = "";
            }else{
                error = "Ya esta en la primera pagina.";
            }
        }else if(tecla == 'c' || tecla == 'C'){
            std::cout << "C" << std::endl;

            std::string nombre = leerTexto("\nIngrese el nombre del nuevo usuario: ");

            if(nombre == ""){
                error = "El nombre no puede estar vacio.";
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
        }else if(tecla == '0'){
            std::cout << "0" << std::endl;
            salir = true;
        }else if(tecla >= '1' && tecla <= '9'){
            int numeroVisible = tecla - '0';

            if(numeroVisible >= 1 && numeroVisible <= cantidadVisible){
                std::cout << char(tecla) << std::endl;

                int indiceUsuario = inicio + numeroVisible - 1;

                usuarioActual = usuarios[indiceUsuario];
                archivoTareasGlobal = construirRutaArchivoTareas(usuarioActual);
                cargarTareas();

                escribirColor(VERDE, "\nUsuario ingresado correctamente.\n");
                std::cout << "Usuario actual: " << usuarioActual << std::endl;
                std::cout << "Archivo de tareas: " << archivoTareasGlobal << std::endl;
                pausar();
                salir = true;
            }else{
                error = "Opcion invalida. Elija un usuario visible, N, P, C o 0.";
            }
        }else{
            error = "Opcion invalida. Elija un usuario visible, N, P, C o 0.";
        }

    }while(!salir);
}

int menuUsuario(){
    std::string error = "";

    while(true){
        limpiarPantalla();

        linea(0);
        centrarTexto("TASKMANAGER.cpp - MENU DE USUARIOS");
        linea(1);
        textoIzquierda("[1] Seleccionar o crear usuario.");
        textoIzquierda("[0] Salir.");
        linea(2);

        if(error != ""){
            escribirColor(ROJO, "\n" + error + "\n");
        }

        int opcion = leerOpcionUnDigito("Opcion: ", 0, 1);

        if(opcion != -1){
            return opcion;
        }

        error = "Opcion invalida. Solo puede elegir 1 o 0.";
    }
}
