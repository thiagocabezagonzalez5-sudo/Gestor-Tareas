#ifndef USUARIOS_H
#define USUARIOS_H

#include <string>
#include <vector>

extern std::string archivoTareasGlobal;
extern std::string usuarioActual;

std::string construirRutaArchivoTareas(const std::string& nombreUsuario);
void crearCarpeta(const std::string& nombreUsuario);
std::vector<std::string> obtenerUsuarios();
void mostrarUsuariosPaginados(const std::vector<std::string>& usuarios, int paginaActual, int usuariosPorPagina);
void ingresarUsuario();
int menuUsuario();

#endif
