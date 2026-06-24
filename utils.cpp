#include "utils.h"
#include "visual.h"
#include "config.h"

#include <iostream>
#include <limits>
#include <conio.h>
#include <cstdlib>

void pausar(){
    escribirColor(NEGRITA, "\nPresione una tecla para continuar...");
    _getch();
}

void limpiarBuffer(){
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

int leerEntero(const std::string& mensaje, int minimo, int maximo){
    int valor;
    bool valido = false;

    do{
        std::cout << mensaje;
        std::cin >> valor;

        if(std::cin.fail()){
            limpiarBuffer();
            escribirColor(ROJO, "Dato invalido. Ingrese un numero.\n");
        }else if(valor < minimo || valor > maximo){
            limpiarBuffer();
            escribirColor(ROJO, "El numero debe estar entre " + std::to_string(minimo) + " y " + std::to_string(maximo) + ".\n");
        }else{
            limpiarBuffer();
            valido = true;
        }

    }while(!valido);

    return valor;
}

int leerOpcionUnDigito(const std::string& mensaje, int minimo, int maximo){
    int tecla;

    std::cout << mensaje;
    tecla = _getch();

    if(tecla >= '0' && tecla <= '9'){
        int valor = tecla - '0';

        if(valor >= minimo && valor <= maximo){
            std::cout << char(tecla) << std::endl;
            return valor;
        }
    }

    return -1;
}

char convertirAMayuscula(char letra){
    if(letra >= 'a' && letra <= 'z'){
        return char(letra - 32);
    }

    return letra;
}

char leerOpcionCaracter(const std::string& mensaje, const std::string& opcionesValidas){
    int tecla;
    char opcion;

    std::cout << mensaje;
    tecla = _getch();
    opcion = convertirAMayuscula(char(tecla));

    for(int i = 0; i < (int)opcionesValidas.length(); i++){
        if(opcion == convertirAMayuscula(opcionesValidas[i])){
            std::cout << opcion << std::endl;
            return opcion;
        }
    }

    return '\0';
}

std::string leerTexto(const std::string& mensaje){
    std::string texto;

    std::cout << mensaje;
    std::getline(std::cin, texto);

    return texto;
}

std::string ajustarTexto(const std::string& texto, int ancho){
    if((int)texto.length() > ancho){
        return texto.substr(0, ancho - 3) + "...";
    }

    return texto;
}

std::string limpiarCampoArchivo(std::string texto){
    for(int i = 0; i < (int)texto.length(); i++){
        if(texto[i] == ';'){
            texto[i] = ',';
        }
    }

    return texto;
}

void limpiarPantalla(){
    system("cls");
}
