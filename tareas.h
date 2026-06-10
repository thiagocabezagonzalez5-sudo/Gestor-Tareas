#ifndef TAREAS_H
#define TAREAS_H

#include <string>
#include <vector>

struct Tarea{
    int id;
    std::string nombre;
    std::string descripcion;
    int importancia;
    std::string fecha_entrega;
    bool completada;
};

extern std::vector<Tarea> tareas;

int generarNuevoId();
void agregarTarea(int id, const std::string& nombre, const std::string& descripcion,
                  int importancia, const std::string& fecha_entrega, bool completada);
int buscarIndicePorId(int id);
std::string mostrarEstrellas(int importancia);
std::string mostrarEstado(bool completada);

void agregarTareaInteractiva();
void mostrarTareasPaginadas(int paginaActual, int tareasPorPagina);
void verTareas();
void pedirDetalleTarea();
void mostrarDetalleTarea(const Tarea& t);
void modificarTarea();
void borrarTarea();

int seleccionarTareaPaginada(const std::string& titulo, const std::string& accion);

#endif
