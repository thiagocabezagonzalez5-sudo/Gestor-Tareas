#ifndef CONFIG_H
#define CONFIG_H

#include <string>

extern const std::string RESET;
extern const std::string ROJO;
extern const std::string VERDE;
extern const std::string AMARILLO;
extern const std::string AZUL;
extern const std::string MAGENTA;
extern const std::string CYAN;
extern const std::string BLANCO;
extern const std::string NEGRITA;

extern const int ANCHO;
extern const std::string ARCHIVO_TAREAS;
extern const std::string CARPETA_USUARIOS;

extern const int COL_ID;
extern const int COL_NOMBRE;
extern const int COL_DESCRIPCION;
extern const int COL_IMPORTANCIA;
extern const int COL_FECHA;
extern const int COL_ESTADO;

extern const int TECLA_ESC;

void configurarConsola();

#endif
