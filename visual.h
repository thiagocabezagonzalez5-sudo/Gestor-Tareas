#ifndef VISUAL_H
#define VISUAL_H

#include <string>

void escribirColor(const std::string& color, const std::string& texto);
void linea(int pos);
void lineaTabla(int posicion);
void centrarTexto(const std::string& texto);
void centrarTextoColor(const std::string& color, const std::string& texto);
void textoIzquierda(const std::string& texto);
void textoIzquierdaColor(const std::string& color, const std::string& texto);
void filaTabla(const std::string& id, const std::string& nombre, const std::string& descripcion,
               const std::string& importancia, const std::string& fecha, const std::string& estado);
void mostrarOpcionesVerTareas();
void mostrarFormularioAgregarTarea(const std::string& nombre, const std::string& descripcion,
                                   const std::string& fecha, const std::string& importancia,
                                   const std::string& error);
void mostrarEncabezadoSimple(const std::string& titulo);

#endif
