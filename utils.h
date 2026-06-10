#ifndef UTILS_H
#define UTILS_H

#include <string>

void pausar();
void limpiarBuffer();
int leerEntero(const std::string& mensaje, int minimo, int maximo);
std::string leerTexto(const std::string& mensaje);
std::string ajustarTexto(const std::string& texto, int ancho);
std::string limpiarCampoArchivo(std::string texto);
void limpiarPantalla();

#endif
