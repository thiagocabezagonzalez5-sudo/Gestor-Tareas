GESTOR DE TAREAS 6761 - PROYECTO REORGANIZADO

Archivos que deben agregarse al proyecto de Embarcadero:
- main.cpp
- config.cpp
- utils.cpp
- validaciones.cpp
- visual.cpp
- tareas.cpp
- archivos.cpp
- usuarios.cpp
- ordenamiento.cpp
- menu.cpp

Los archivos .h deben estar en la misma carpeta:
- config.h
- utils.h
- validaciones.h
- visual.h
- tareas.h
- archivos.h
- usuarios.h
- ordenamiento.h
- menu.h

Importante:
1. Copia TODOS estos archivos a la misma carpeta donde esta tu proyecto.
2. En Embarcadero, usa Project > Add to Project y agrega TODOS los .cpp.
3. Si el proyecto viejo tenia un main.cpp anterior, reemplazalo por este main.cpp.
4. Si aparecen errores de includes, revisa que los .h esten en la misma carpeta que los .cpp.
5. Este proyecto evita std::filesystem para mayor compatibilidad con Embarcadero.

Git:
Despues de copiar y probar:
git status
git add .
git commit -m "Reorganiza proyecto en modulos y agrega paginacion"
