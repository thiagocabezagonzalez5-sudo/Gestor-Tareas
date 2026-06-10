#ifndef VALIDACIONES_H
#define VALIDACIONES_H

#include <string>

bool esDigito(char c);
int validarMes(int mes, int anio);
bool validarFecha(const std::string& fecha);
std::string leerFechaAutomatica(const std::string& mensaje);
std::string convertirFechaAFormatoPrograma(const std::string& fecha);

#endif
