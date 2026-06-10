#include "config.h"

#include <windows.h>
#include <clocale>

const std::string RESET = "\033[0m";
const std::string ROJO = "\033[31m";
const std::string VERDE = "\033[32m";
const std::string AMARILLO = "\033[33m";
const std::string AZUL = "\033[34m";
const std::string MAGENTA = "\033[35m";
const std::string CYAN = "\033[36m";
const std::string BLANCO = "\033[37m";
const std::string NEGRITA = "\033[1m";

const int ANCHO = 130;
const std::string ARCHIVO_TAREAS = "tareas.csv";
const std::string CARPETA_USUARIOS = "Usuarios";

const int COL_ID = 4;
const int COL_NOMBRE = 32;
const int COL_DESCRIPCION = 55;
const int COL_IMPORTANCIA = 8;
const int COL_FECHA = 18;
const int COL_ESTADO = 8;

const int TECLA_ESC = 27;

void configurarConsola(){
    HWND consola = GetConsoleWindow();
    ShowWindow(consola, SW_MAXIMIZE);

    setlocale(LC_ALL, "es_ES.UTF-8");

    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD dwMode = 0;

    if(GetConsoleMode(hOut, &dwMode)){
        SetConsoleMode(hOut, dwMode | ENABLE_VIRTUAL_TERMINAL_PROCESSING);
    }

    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
}
