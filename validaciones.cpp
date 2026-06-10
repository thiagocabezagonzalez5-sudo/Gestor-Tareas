#include "validaciones.h"

#include <iostream>
#include <conio.h>

bool esDigito(char c){
    return c >= '0' && c <= '9';
}

int validarMes(int mes, int anio){
    bool bisiesto = (anio % 4 == 0 && (anio % 100 != 0 || anio % 400 == 0));
    int dias = 0;

    switch(mes){
        case 1: case 3: case 5: case 7: case 8: case 10: case 12:
            dias = 31;
            break;

        case 4: case 6: case 9: case 11:
            dias = 30;
            break;

        case 2:
            dias = bisiesto ? 29 : 28;
            break;

        default:
            dias = 0;
            break;
    }

    return dias;
}

bool validarFecha(const std::string& fecha){
    if(fecha.length() != 10){
        return false;
    }

    if(fecha[4] != '/' || fecha[7] != '/'){
        return false;
    }

    for(int i = 0; i < 10; i++){
        if(i != 4 && i != 7){
            if(!esDigito(fecha[i])){
                return false;
            }
        }
    }

    int anio = std::stoi(fecha.substr(0, 4));
    int mes = std::stoi(fecha.substr(5, 2));
    int dia = std::stoi(fecha.substr(8, 2));

    int diasMes = validarMes(mes, anio);

    if(anio < 1900 || anio > 3000){
        return false;
    }

    if(mes < 1 || mes > 12){
        return false;
    }

    if(dia < 1 || dia > diasMes){
        return false;
    }

    return true;
}

std::string leerFechaAutomatica(const std::string& mensaje){
    std::string numeros = "";
    int tecla;

    std::cout << mensaje;

    while(true){
        tecla = _getch();

        if(tecla == 13){
            if(numeros.length() == 8){
                std::cout << std::endl;
                return numeros.substr(0, 4) + "/" + numeros.substr(4, 2) + "/" + numeros.substr(6, 2);
            }else{
                std::cout << "\nDebe ingresar 8 numeros. Ejemplo: 20260520\n";
                numeros = "";
                std::cout << mensaje;
            }
        }else if(tecla == 8){
            if(numeros.length() > 0){
                numeros.erase(numeros.length() - 1);

                std::cout << "\r" << std::string(60, ' ') << "\r";
                std::cout << mensaje;

                for(int i = 0; i < (int)numeros.length(); i++){
                    std::cout << numeros[i];

                    if(i == 3 || i == 5){
                        std::cout << "/";
                    }
                }
            }
        }else if(tecla >= '0' && tecla <= '9'){
            if(numeros.length() < 8){
                numeros += char(tecla);
                std::cout << char(tecla);

                if(numeros.length() == 4 || numeros.length() == 6){
                    std::cout << "/";
                }
            }
        }
    }
}

std::string convertirFechaAFormatoPrograma(const std::string& fecha){
    if(fecha.length() == 10 && fecha[4] == '/' && fecha[7] == '/'){
        return fecha;
    }

    if(fecha.length() == 10 && fecha[2] == '/' && fecha[5] == '/'){
        std::string dia = fecha.substr(0, 2);
        std::string mes = fecha.substr(3, 2);
        std::string anio = fecha.substr(6, 4);

        return anio + "/" + mes + "/" + dia;
    }

    if(fecha.length() < 10 && fecha.length() > 7){
        std::string dia;
        std::string mes;
        std::string anio;
        int posAnio;

        if(fecha[1] == '/'){
            dia = "0" + fecha.substr(0,1);

            if(fecha[4] == '/'){
                mes = fecha.substr(2,2);
            }else if(fecha[3] == '/'){
                mes = "0" + fecha.substr(2,1);
            }
        }else{
            dia = fecha.substr(0,2);
            mes = "0" + fecha.substr(3,1);
        }

        posAnio = fecha.length() - 4;
        anio = fecha.substr(posAnio, 4);

        return anio + "/" + mes + "/" + dia;
    }

    return fecha;
}
