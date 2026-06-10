#include "visual.h"
#include "config.h"
#include "utils.h"

#include <iostream>
#include <iomanip>

void escribirColor(const std::string& color, const std::string& texto){
    std::cout << color << texto << RESET;
}

void linea(int pos){
    std::string izq, der;

    switch(pos){
        case 0:
            izq = "╔";
            der = "╗";
            break;

        case 1:
            izq = "╠";
            der = "╣";
            break;

        case 2:
            izq = "╚";
            der = "╝";
            break;

        default:
            izq = "╠";
            der = "╣";
            break;
    }

    escribirColor(CYAN, izq);

    for(int i = 0; i < ANCHO; i++){
        escribirColor(CYAN, "═");
    }

    escribirColor(CYAN, der + "\n");
}

void lineaTabla(int posicion){
    std::string izq, sep, der;

    switch(posicion){
        case 0:
            izq = "╠";
            sep = "╬";
            der = "╣";
            break;

        case 1:
            izq = "╠";
            sep = "╦";
            der = "╣";
            break;

        case 2:
            izq = "╠";
            sep = "╩";
            der = "╣";
            break;

        default:
            izq = "╠";
            sep = "╬";
            der = "╣";
            break;
    }

    escribirColor(CYAN, izq);

    for(int i = 0; i < COL_ID; i++) escribirColor(CYAN, "═");
    escribirColor(CYAN, sep);

    for(int i = 0; i < COL_NOMBRE; i++) escribirColor(CYAN, "═");
    escribirColor(CYAN, sep);

    for(int i = 0; i < COL_DESCRIPCION; i++) escribirColor(CYAN, "═");
    escribirColor(CYAN, sep);

    for(int i = 0; i < COL_IMPORTANCIA; i++) escribirColor(CYAN, "═");
    escribirColor(CYAN, sep);

    for(int i = 0; i < COL_FECHA; i++) escribirColor(CYAN, "═");
    escribirColor(CYAN, sep);

    for(int i = 0; i < COL_ESTADO; i++) escribirColor(CYAN, "═");

    escribirColor(CYAN, der + "\n");
}

void centrarTexto(const std::string& textoOriginal){
    std::string texto = textoOriginal;
    int espacios = (ANCHO - (int)texto.length()) / 2;
    int restante = ANCHO - (int)texto.length() - espacios;

    if(espacios < 0){
        texto = ajustarTexto(texto, ANCHO);
        espacios = 0;
        restante = 0;
    }

    escribirColor(CYAN, "║");
    std::cout << std::string(espacios, ' ')
              << texto
              << std::string(restante, ' ');
    escribirColor(CYAN, "║\n");
}

void centrarTextoColor(const std::string& color, const std::string& textoOriginal){
    std::string texto = textoOriginal;
    int espacios = (ANCHO - (int)texto.length()) / 2;
    int restante = ANCHO - (int)texto.length() - espacios;

    if(espacios < 0){
        texto = ajustarTexto(texto, ANCHO);
        espacios = 0;
        restante = 0;
    }

    escribirColor(CYAN, "║");
    std::cout << std::string(espacios, ' ')
              << color
              << texto
              << RESET
              << std::string(restante, ' ');
    escribirColor(CYAN, "║\n");
}

void textoIzquierda(const std::string& texto){
    escribirColor(CYAN, "║");
    std::cout << std::setw(ANCHO) << std::left << ajustarTexto(texto, ANCHO);
    escribirColor(CYAN, "║\n");
}

void textoIzquierdaColor(const std::string& color, const std::string& texto){
    escribirColor(CYAN, "║");
    std::cout << color << std::setw(ANCHO) << std::left << ajustarTexto(texto, ANCHO) << RESET;
    escribirColor(CYAN, "║\n");
}

void filaTabla(const std::string& id, const std::string& nombre, const std::string& descripcion,
               const std::string& importancia, const std::string& fecha, const std::string& estado){
    escribirColor(CYAN, "║");
    std::cout << std::setw(COL_ID) << std::left << ajustarTexto(id, COL_ID);

    escribirColor(CYAN, "║");
    std::cout << std::setw(COL_NOMBRE) << std::left << ajustarTexto(nombre, COL_NOMBRE);

    escribirColor(CYAN, "║");
    std::cout << std::setw(COL_DESCRIPCION) << std::left << ajustarTexto(descripcion, COL_DESCRIPCION);

    escribirColor(CYAN, "║");
    std::cout << std::setw(COL_IMPORTANCIA) << std::left << ajustarTexto(importancia, COL_IMPORTANCIA);

    escribirColor(CYAN, "║");
    std::cout << std::setw(COL_FECHA) << std::left << ajustarTexto(fecha, COL_FECHA);

    escribirColor(CYAN, "║");
    std::cout << std::setw(COL_ESTADO) << std::left << ajustarTexto(estado, COL_ESTADO);

    escribirColor(CYAN, "║");
    std::cout << std::endl;
}

void mostrarOpcionesVerTareas(){
    std::cout << "\n";
    std::cout << "[N] Siguiente pagina   ";
    std::cout << "[P] Pagina anterior   ";
    std::cout << "[D] Ver detalle   ";
    std::cout << "[ESC] Volver\n";

    std::cout << "[1] ID   ";
    std::cout << "[2] Importancia   ";
    std::cout << "[3] Nombre   ";
    std::cout << "[4] Fecha entrega\n";

    std::cout << "\nOpcion: ";
}

void mostrarFormularioAgregarTarea(const std::string& nombre, const std::string& descripcion,
                                   const std::string& fecha, const std::string& importancia,
                                   const std::string& error){
    limpiarPantalla();

    linea(0);
    centrarTextoColor(VERDE, "AGREGAR TAREA");
    linea(1);

    textoIzquierda("NOMBRE: " + nombre);
    textoIzquierda("DESCRIPCION: " + descripcion);
    textoIzquierda("FECHA: " + fecha);
    textoIzquierda("IMPORTANCIA: " + importancia);

    linea(2);

    if(error != ""){
        escribirColor(ROJO, "\n" + error + "\n");
    }

    std::cout << "\n";
}

void mostrarEncabezadoSimple(const std::string& titulo){
    linea(0);
    centrarTexto(titulo);
    linea(2);
}
