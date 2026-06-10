#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <iomanip>
#include <conio.h>
#include <windows.h>
#include <limits>
#include <clocale>

using namespace std;
//─────COLORES─────
const string RESET = "\033[0m";
const string ROJO = "\033[31m";
const string VERDE = "\033[32m";
const string AMARILLO = "\033[33m";
const string AZUL = "\033[34m";
const string MAGENTA = "\033[35m";
const string CYAN = "\033[36m";
const string BLANCO = "\033[37m";

const string NEGRITA = "\033[1m";
//─────CONFIGURACION GENERAL─────
const int ANCHO = 130;
const string ARCHIVO_TAREAS = "tareas.csv";
const string CARPETA_USUARIOS = "Usuarios";
string archivoTareasGlobal = "";
string usuarioActual = "";

//─────ANCHO DE COLUMNAS TABLAS─────
const int COL_ID = 4;
const int COL_NOMBRE = 32;
const int COL_DESCRIPCION = 55;
const int COL_IMPORTANCIA = 8;
const int COL_FECHA = 18;
const int COL_ESTADO = 8;

//─────TECLAS─────
const int TECLA_ESC = 27;

//─────STRUCT PARA TAREAS─────
struct Tarea{
	int id;
	string nombre;
	string descripcion;
	int importancia;
	string fecha_entrega;
	bool completada;
};

//─────VECTOR GLOBAL DE TAREAS─────
vector<Tarea> tareas;

//─────PROTOTIPOS─────

// Configuracion
void configurarConsola();

// Utilidades
void pausar();
void limpiarBuffer();
int leerEntero(string mensaje, int minimo, int maximo);
string leerTexto(string mensaje);
string ajustarTexto(string texto, int ancho);
string limpiarCampoArchivo(string texto);
string leerFechaAutomatica(string mensaje);

// Validaciones
int validarMes(int mes, int anio);
bool validarFecha(string fecha);
bool esDigito(char c);

// Visual
void escribirColor(string color, string texto);
void linea(int pos);
void lineaTabla(int posicion);
void centrarTexto(string texto);
void centrarTextoColor(string color, string texto);
void textoIzquierda(string texto);
void textoIzquierdaColor(string color, string texto);
void filaTabla(string id, string nombre, string descripcion, string importancia, string fecha, string estado);
void mostrarOpcionesVerTareas();
void mostrarFormularioAgregarTarea(string nombre, string descripcion, string fecha, string importancia, string error);
string convertirFechaAFormatoPrograma(string fecha);

// Tareas base
int generarNuevoId();
void agregarTarea(int id, string nombre, string descripcion, int importancia, string fecha_entrega, bool completada);
int buscarIndicePorId(int id);
string mostrarEstrellas(int importancia);
string mostrarEstado(bool completada);

// Pantallas de tareas
void agregarTareaInteractiva();
void mostrarTareasPaginadas(int paginaActual, int tareasPorPagina);
void verTareas();
void pedirDetalleTarea();
void mostrarDetalleTarea(Tarea t);
void modificarTarea();
void borrarTarea();

// Pantallas de usuario
void ingresarUsuario();

// Ordenamiento
bool comparar(int tipo, Tarea a, Tarea b);
void ordenar(int tipo);

// Carpeta
void crearCarpeta(string nombreUsuario);
string construirRutaArchivoTareas(string nombreUsuario);

// Archivos
void guardarTareas();
void cargarTareas();

// Menu
int menuUsuario();
int mostrarMenu();

//─────MAIN─────
int main(){
	configurarConsola();

	bool salir = false, salirUsuario = false;

	do{
		int opcion_u = menuUsuario();

		switch(opcion_u){
			case 1:
				salir = false;
				ingresarUsuario();
				break;

			case 0:
				salir = true;
				salirUsuario = true;
				break;
		}
		
		while(!salir){
			int opcion = mostrarMenu();

			switch(opcion){
				case 1:
					verTareas();
					break;

				case 2:
					agregarTareaInteractiva();
					break;

				case 3:
					modificarTarea();
					break;

				case 4:
					borrarTarea();
					break;

				case 0:
					guardarTareas();
					salir = true;
					break;

				default:   
					escribirColor(ROJO, "\nError, ingrese una opcion valida.\n");
					pausar();
					break;
			}	
		}

	}while(!salirUsuario);

	return 0;
}

//─────CONFIGURACION─────
void configurarConsola(){
    HWND consola = GetConsoleWindow();
    ShowWindow(consola, SW_MAXIMIZE);
    setlocale(LC_ALL, "es_ES.UTF-8");
	HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	DWORD dwMode = 0;

	GetConsoleMode(hOut, &dwMode);
	SetConsoleMode(hOut, dwMode | ENABLE_VIRTUAL_TERMINAL_PROCESSING);
	SetConsoleOutputCP(CP_UTF8);
	SetConsoleCP(CP_UTF8);
}

void pausar(){
	escribirColor(NEGRITA, "\nPresione una tecla para continuar...");
	_getch();
}

void limpiarBuffer(){
	cin.clear();
	cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

int leerEntero(string mensaje, int minimo, int maximo){
	int valor;
	bool valido = false;

	do{
		cout << mensaje;
		cin >> valor;
		
		if(cin.fail()){
			limpiarBuffer();
			escribirColor(ROJO, "Dato invalido. Ingrese un numero.\n");
		}else if(valor < minimo || valor > maximo){
			limpiarBuffer();
			escribirColor(ROJO, "El numero debe estar entre " + to_string(minimo) + " y " + to_string(maximo) + ".\n");
		}else{
			limpiarBuffer();
			valido = true;
		}

	}while(!valido);

	return valor;
}

string leerTexto(string mensaje){
	string texto;

	cout << mensaje;
	getline(cin, texto);

	return texto;
}

string ajustarTexto(string texto, int ancho){
	if((int)texto.length() > ancho){
		return texto.substr(0, ancho - 3) + "...";
	}

	return texto;
}

string limpiarCampoArchivo(string texto){
	for(int i = 0; i < (int)texto.length(); i++){
		if(texto[i] == ';'){
			texto[i] = ',';
		}
	}

	return texto;
}

string leerFechaAutomatica(string mensaje){
	string numeros = "";
	int tecla;

	cout << mensaje;

	while(true){
		tecla = _getch();

		// ENTER
		if(tecla == 13){
			if(numeros.length() == 8){
				cout << endl;
				return numeros.substr(0, 4) + "/" + numeros.substr(4, 2) + "/" + numeros.substr(6, 2);
			}else{
				cout << "\nDebe ingresar 8 numeros. Ejemplo: 20260520\n";
				numeros = "";
				cout << mensaje;
			}
		}

		// BACKSPACE
		else if(tecla == 8){
			if(numeros.length() > 0){
				numeros.pop_back();

				cout << "\r" << string(40, ' ') << "\r";
				cout << mensaje;

				for(int i = 0; i < (int)numeros.length(); i++){
					cout << numeros[i];

					if(i == 3 || i == 5){
						cout << "/";
					}
				}
			}
		}

		// SOLO NUMEROS
		else if(tecla >= '0' && tecla <= '9'){
			if(numeros.length() < 8){
				numeros += char(tecla);
				cout << char(tecla);

				if(numeros.length() == 4 || numeros.length() == 6){
					cout << "/";
				}
			}
		}
	}
}

//─────VALIDACIONES─────
bool esDigito(char c){
	return c >= '0' && c <= '9';
}

int validarMes(int mes, int anio){
	bool biciesto = (anio % 4 == 0) ? true : false;
	int dias = 0;
	
	switch(mes){
		case 1:	case 3:	case 5:	case 7:	case 8:	case 10: case 12:
			dias = 31;
			break;
		case 4:	case 6:	case 9:	case 11:
			dias = 30;
			break;
		case 2:
			dias = (biciesto) ? 29 : 28;
			break;
	}
		
	return dias;
}
bool validarFecha(string fecha){
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

	int anio = stoi(fecha.substr(0, 4));
	int mes = stoi(fecha.substr(5, 2));
	int dia = stoi(fecha.substr(8, 2));

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

//─────VISUAL GENERAL─────
void escribirColor(string color, string texto){
	cout << color << texto << RESET;
}

void linea(int pos){
	string izq, der;

	switch(pos){
		case 0:
			izq = "╔";
			der = "╗";
			break;

		case 1:
			izq = "╠";
			der = "╣";
			break;

		case 2:
			izq = "╚";
			der = "╝";
			break;

		default:
			izq = "╠";
			der = "╣";
			break;
	}

	escribirColor(CYAN, izq);

	for(int i = 0; i < ANCHO; i++) escribirColor(CYAN, "═");

	escribirColor(CYAN, der + "\n");
}

void lineaTabla(int posicion){
	string izq, sep, der;

	switch(posicion){
		case 0:
			izq = "╠";
			sep = "╬";
			der = "╣";
			break;

		case 1:
			izq = "╠";
			sep = "╦";
			der = "╣";
			break;

		case 2:
			izq = "╠";
			sep = "╩";
			der = "╣";
			break;

		default:
			izq = "╠";
			sep = "╬";
			der = "╣";
			break;
	}

	escribirColor(CYAN, izq);

	for(int i = 0; i < COL_ID; i++) escribirColor(CYAN, "═");
	escribirColor(CYAN, sep);

	for(int i = 0; i < COL_NOMBRE; i++) escribirColor(CYAN, "═");
	escribirColor(CYAN, sep);

	for(int i = 0; i < COL_DESCRIPCION; i++) escribirColor(CYAN, "═");
	escribirColor(CYAN, sep);

	for(int i = 0; i < COL_IMPORTANCIA; i++) escribirColor(CYAN, "═");
	escribirColor(CYAN, sep);

	for(int i = 0; i < COL_FECHA; i++) escribirColor(CYAN, "═");
	escribirColor(CYAN, sep);

	for(int i = 0; i < COL_ESTADO; i++) escribirColor(CYAN, "═");

	escribirColor(CYAN, der + "\n");
}

void centrarTexto(string texto){
	int espacios = (ANCHO - texto.length()) / 2;
	int restante = ANCHO - texto.length() - espacios;

	if(espacios < 0){
		texto = ajustarTexto(texto, ANCHO);
		espacios = 0;
		restante = 0;
	}

	escribirColor(CYAN, "║");
	cout << string(espacios, ' ')
	     << texto
	     << string(restante, ' ');
	escribirColor(CYAN, "║\n");
}

void centrarTextoColor(string color, string texto){
	int espacios = (ANCHO - texto.length()) / 2;
	int restante = ANCHO - texto.length() - espacios;

	if(espacios < 0){
		texto = ajustarTexto(texto, ANCHO);
		espacios = 0;
		restante = 0;
	}

	escribirColor(CYAN, "║");
	cout << string(espacios, ' ')
		 << color
	     << texto
	     << RESET
	     << string(restante, ' ');
	escribirColor(CYAN, "║\n");
}
void textoIzquierda(string texto){
	escribirColor(CYAN, "║");
	cout << setw(ANCHO) << left << ajustarTexto(texto, ANCHO);
	escribirColor(CYAN, "║\n");
}

void textoIzquierdaColor(string color, string texto){
	escribirColor(CYAN, "║");
	cout << color << setw(ANCHO) << left << ajustarTexto(texto, ANCHO) << RESET;
	escribirColor(CYAN, "║\n");
}

void filaTabla(string id, string nombre, string descripcion, string importancia, string fecha, string estado){
	escribirColor(CYAN, "║");
	cout << setw(COL_ID) << left << ajustarTexto(id, COL_ID);
	escribirColor(CYAN, "║");
	cout << setw(COL_NOMBRE) << left << ajustarTexto(nombre, COL_NOMBRE);
	escribirColor(CYAN, "║");
	cout << setw(COL_DESCRIPCION) << left << ajustarTexto(descripcion, COL_DESCRIPCION);
	escribirColor(CYAN, "║");
	cout << setw(COL_IMPORTANCIA) << left << ajustarTexto(importancia, COL_IMPORTANCIA);
	escribirColor(CYAN, "║");
	cout << setw(COL_FECHA) << left << ajustarTexto(fecha, COL_FECHA);
	escribirColor(CYAN, "║");
	cout << setw(COL_ESTADO) << left << ajustarTexto(estado, COL_ESTADO);
	escribirColor(CYAN, "║");
	cout << endl;
}

void mostrarOpcionesVerTareas(){
	cout << "\n";
	cout << "[N] Siguiente pagina   ";
	cout << "[P] Pagina anterior   ";
	cout << "[D] Ver detalle   ";
	cout << "[ESC] Volver\n";

	cout << "[1] ID   ";
	cout << "[2] Importancia   ";
	cout << "[3] Nombre   ";
	cout << "[4] Fecha entrega\n";

	cout << "\nOpcion: ";
}

void mostrarFormularioAgregarTarea(string nombre, string descripcion, string fecha, string importancia, string error){
	system("cls");

	linea(0);
	centrarTextoColor(VERDE, "AGREGAR TAREA");
	linea(1);

	textoIzquierda("NOMBRE: " + nombre);
	textoIzquierda("DESCRIPCION: " + descripcion);
	textoIzquierda("FECHA: " + fecha);
	textoIzquierda("IMPORTANCIA: " + importancia);

	linea(2);

	if(error != ""){
		escribirColor(ROJO, "\n" + error + "\n");
	}

	cout << "\n";
}

string convertirFechaAFormatoPrograma(string fecha){
	if(fecha.length() == 10 && fecha[4] == '/' && fecha[7] == '/'){
		return fecha;
	}

	if(fecha.length() == 10 && fecha[2] == '/' && fecha[5] == '/'){
		string dia = fecha.substr(0, 2);
		string mes = fecha.substr(3, 2);
		string anio = fecha.substr(6, 4);

		return anio + "/" + mes + "/" + dia;
	}
	
	if(fecha.length() < 10 && fecha.length() > 7){
		string dia;
		string mes;
		string anio;
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
//─────TAREAS BASE─────
int generarNuevoId(){
	int mayorId = 0;

	for(int i = 0; i < (int)tareas.size(); i++){
		if(tareas[i].id > mayorId){
			mayorId = tareas[i].id;
		}
	}

	return mayorId + 1;
}

void agregarTarea(int id, string nombre, string descripcion, int importancia, string fecha_entrega, bool completada){
	Tarea nueva;

	nueva.id = id;
	nueva.nombre = nombre;
	nueva.descripcion = descripcion;
	nueva.importancia = importancia;
	nueva.fecha_entrega = fecha_entrega;
	nueva.completada = completada;

	tareas.push_back(nueva);
}

int buscarIndicePorId(int id){
	for(int i = 0; i < (int)tareas.size(); i++){
		if(tareas[i].id == id){
			return i;
		}
	}

	return -1;
}


string mostrarEstrellas(int importancia){
	string estrellas = "";

	for(int i = 0; i < importancia; i++) estrellas += "*";
	for(int i = importancia; i < 5; i++) estrellas += "-";

	return estrellas;
}

string mostrarEstado(bool completada){
	if(completada){
		return "OK";
	}

	return "Pend.";
}

//─────AGREGAR TAREA─────
void agregarTareaInteractiva(){
	int id = generarNuevoId();

	string nombre = "";
	string descripcion = "";
	string fecha = "";
	string importanciaTexto = "";
	string error = "";

	// NOMBRE
	do{
		mostrarFormularioAgregarTarea(nombre, descripcion, fecha, importanciaTexto, error);

		nombre = leerTexto("Ingrese el nombre: ");

		if(nombre == ""){
			error = "El nombre no puede estar vacio.";
		}else if((int)nombre.length() > COL_NOMBRE){
			error = "El nombre no puede superar " + to_string(COL_NOMBRE) + " caracteres.";
			nombre = "";
		}else{
			error = "";
		}

	}while(error != "");

	// DESCRIPCION
	do{
		mostrarFormularioAgregarTarea(nombre, descripcion, fecha, importanciaTexto, error);

		descripcion = leerTexto("Ingrese la descripcion: ");

		if(descripcion == ""){
			error = "La descripcion no puede estar vacia.";
		}else if((int)descripcion.length() > COL_DESCRIPCION){
			error = "La descripcion no puede superar " + to_string(COL_DESCRIPCION) + " caracteres.";
			descripcion = "";
		}else{
			error = "";
		}

	}while(error != "");

	// FECHA
	do{
		mostrarFormularioAgregarTarea(nombre, descripcion, fecha, importanciaTexto, error);

		fecha = leerFechaAutomatica("Ingrese la fecha [aaaammdd]: ");

		if(!validarFecha(fecha)){
			error = "Fecha invalida. Ejemplo correcto: 20260520.";
			fecha = "";
		}else{
			error = "";
		}

	}while(error != "");

	// IMPORTANCIA
	do{
		mostrarFormularioAgregarTarea(nombre, descripcion, fecha, importanciaTexto, error);

		int importancia = leerEntero("Ingrese la importancia [1-5]: ", 1, 5);
		importanciaTexto = to_string(importancia);

		error = "";

	}while(error != "");

	int importanciaFinal = stoi(importanciaTexto);

	agregarTarea(id, nombre, descripcion, importanciaFinal, fecha, false);
	guardarTareas();

	mostrarFormularioAgregarTarea(nombre, descripcion, fecha, importanciaTexto, "");
	cout << "Tarea agregada correctamente.\n";

	pausar();
}

//─────MOSTRAR TAREAS─────
void mostrarTareasPaginadas(int paginaActual, int tareasPorPagina){
	string completada, estrellas;

	int totalTareas = tareas.size();
	int totalPaginas = (totalTareas + tareasPorPagina - 1) / tareasPorPagina;

	if(totalPaginas == 0){
		totalPaginas = 1;
	}

	int inicio = paginaActual * tareasPorPagina;
	int fin = inicio + tareasPorPagina;

	if(fin > totalTareas){
		fin = totalTareas;
	}

	linea(0);
	centrarTexto("GESTOR TAREAS 6761");
	centrarTexto("Pagina " + to_string(paginaActual + 1) + " de " + to_string(totalPaginas));
	lineaTabla(1);

	filaTabla("ID", "NOMBRE", "DESCRIPCION", "IMPOR", "ENTREGA", "ESTADO");
	lineaTabla(0);

	if(tareas.empty()){
		filaTabla("-", "Sin tareas", "-", "-", "-", "-");
		lineaTabla(2);
		centrarTexto("Thiago Cabeza Gonzalez - 2026");
		linea(2);
		return;
	}

	for(int i = inicio; i < fin; i++){
		Tarea t = tareas[i];

		completada = mostrarEstado(t.completada);
		estrellas = mostrarEstrellas(t.importancia);

		filaTabla(
			to_string(t.id),
			t.nombre,
			t.descripcion,
			estrellas,
			t.fecha_entrega,
			completada
		);

		if(i == fin - 1){
			lineaTabla(2);
		}else{
			lineaTabla(0);
		}
	}

	centrarTexto("Thiago Cabeza Gonzalez - 2026");
	linea(2);
}

//─────VER TAREAS─────
void verTareas(){
	int tecla;
	bool volver = false;

	int paginaActual = 0;
	int tareasPorPagina = 5;

	do{
		system("cls");

		int totalTareas = tareas.size();
		int totalPaginas = (totalTareas + tareasPorPagina - 1) / tareasPorPagina;

		if(totalPaginas == 0){
			totalPaginas = 1;
		}

		if(paginaActual >= totalPaginas){
			paginaActual = totalPaginas - 1;
		}

		if(paginaActual < 0){
			paginaActual = 0;
		}

		mostrarTareasPaginadas(paginaActual, tareasPorPagina);
		mostrarOpcionesVerTareas();

		tecla = _getch();

		switch(tecla){
			case 'n':
			case 'N':
				if(paginaActual < totalPaginas - 1){
					paginaActual++;
				}
				break;

			case 'p':
			case 'P':
				if(paginaActual > 0){
					paginaActual--;
				}
				break;

			case '1':
				ordenar(1);
				paginaActual = 0;
				break;

			case '2':
				ordenar(2);
				paginaActual = 0;
				break;

			case '3':
				ordenar(3);
				paginaActual = 0;
				break;

			case '4':
				ordenar(4);
				paginaActual = 0;
				break;

			case 'd':
			case 'D':
				pedirDetalleTarea();
				break;

			case TECLA_ESC:
				volver = true;
				break;

			default:
				cout << "\nOpcion invalida.\n";
				pausar();
				break;
		}

	}while(!volver);
}

//─────DETALLE DE TAREA─────
void pedirDetalleTarea(){
	if(tareas.empty()){
		cout << "\nNo hay tareas para mostrar.\n";
		pausar();
		return;
	}

	system("cls");

	int id = leerEntero("Ingrese el ID de la tarea que quiere ver en detalle: ", 1, 999999);
	int indice = buscarIndicePorId(id);

	if(indice == -1){
		cout << "\nNo existe una tarea con ese ID.\n";
		pausar();
	}else{
		mostrarDetalleTarea(tareas[indice]);
	}
}

void mostrarDetalleTarea(Tarea t){
	int tecla;

	do{
		system("cls");

		linea(0);
		centrarTextoColor(AZUL, "DETALLE DE TAREA");
		linea(1);

		textoIzquierda("ID: " + to_string(t.id));
		textoIzquierda("Nombre: " + t.nombre);
		textoIzquierda("Descripcion: " + t.descripcion);
		textoIzquierda("Importancia: " + mostrarEstrellas(t.importancia));
		textoIzquierda("Fecha de entrega: " + t.fecha_entrega);
		textoIzquierda("Estado: " + mostrarEstado(t.completada));

		linea(2);

		cout << "\n[ESC] Volver a la lista";

		tecla = _getch();

	}while(tecla != TECLA_ESC);
}

//─────MODIFICAR TAREA─────
void modificarTarea(){
	if(tareas.empty()){
		system("cls");
		escribirColor(NEGRITA, "No hay tareas para modificar.\n");
		pausar();
		return;
	}

	system("cls");

	int id = leerEntero("Ingrese el ID de la tarea que quiere modificar: ", 1, 999999);
	int indice = buscarIndicePorId(id);

	if(indice == -1){
		escribirColor(NEGRITA, "\nNo existe una tarea con ese ID.\n");
		pausar();
		return;
	}

	bool salir = false;

	do{
		system("cls");

		mostrarDetalleTarea(tareas[indice]);

		system("cls");
		linea(0);
		centrarTextoColor(MAGENTA, "MODIFICAR TAREA");
		linea(1);
		textoIzquierda("[1] Nombre");
		textoIzquierda("[2] Descripcion");
		textoIzquierda("[3] Importancia");
		textoIzquierda("[4] Fecha de entrega");
		textoIzquierda("[5] Cambiar estado");
		textoIzquierda("[0] Volver");
		linea(2);

		int opcion = leerEntero("Opcion: ", 0, 5);

		switch(opcion){
			case 1:
				tareas[indice].nombre = leerTexto("Nuevo nombre: ");
				break;

			case 2:
				tareas[indice].descripcion = leerTexto("Nueva descripcion: ");
				break;

			case 3:
				tareas[indice].importancia = leerEntero("Nueva importancia [1-5]: ", 1, 5);
				break;

			case 4:{
				string fecha;

				do{
					fecha = leerFechaAutomatica("Nueva fecha [aaaammdd]: ");
				
					if(!validarFecha(fecha)){
						cout << "Fecha invalida. Ejemplo correcto: 20260520\n";
					}
				
				}while(!validarFecha(fecha));

				tareas[indice].fecha_entrega = fecha;
				break;
			}

			case 5:
				tareas[indice].completada = !tareas[indice].completada;
				break;

			case 0:
				salir = true;
				break;
		}

		guardarTareas();

	}while(!salir);
}

//─────BORRAR TAREA─────
void borrarTarea(){
	if(tareas.empty()){
		system("cls");
		cout << "No hay tareas para borrar.\n";
		pausar();
		return;
	}

	system("cls");

	int id = leerEntero("Ingrese el ID de la tarea que quiere borrar: ", 1, 999999);
	int indice = buscarIndicePorId(id);

	if(indice == -1){
		escribirColor(ROJO, "\nNo existe una tarea con ese ID.\n");
		pausar();
		return;
	}

	system("cls");

	linea(0);
	centrarTextoColor(ROJO, "BORRAR TAREA");
	linea(1);
	textoIzquierda("ID: " + to_string(tareas[indice].id));
	textoIzquierda("Nombre: " + tareas[indice].nombre);
	textoIzquierda("Descripcion: " + tareas[indice].descripcion);
	linea(2);

	cout << "\nSeguro que desea borrar esta tarea? [S/N]: ";

	int tecla = _getch();

	if(tecla == 's' || tecla == 'S'){
		tareas.erase(tareas.begin() + indice);
		guardarTareas();

		cout << "\n\nTarea borrada correctamente.\n";
	}else{
		cout << "\n\nOperacion cancelada.\n";
	}

	pausar();
}

//─────NUEVO USUARIO─────
void ingresarUsuario(){
	system("cls");

	string nombre = leerTexto("Ingrese su nombre: ");

	usuarioActual = nombre;
	archivoTareasGlobal = construirRutaArchivoTareas(nombre);

	crearCarpeta(nombre);
	cargarTareas();

	cout << "\nUsuario ingresado correctamente.\n";
	cout << "Archivo de tareas: " << archivoTareasGlobal << endl;

	pausar();
}

//─────ORDENAMIENTO─────
bool comparar(int tipo, Tarea a, Tarea b){
	switch(tipo){
		case 1:
			return a.id < b.id;

		case 2:
			return a.importancia > b.importancia;

		case 3:
			return a.nombre < b.nombre;

		case 4:
			return a.fecha_entrega < b.fecha_entrega;

		default:
			return a.id < b.id;
	}
}

void ordenar(int tipo){
	sort(tareas.begin(), tareas.end(), [tipo](Tarea a, Tarea b){
		return comparar(tipo, a, b);
	});
}

//─────CREAR CARPETAS─────

void crearCarpeta(string nombreUsuario){
	CreateDirectory(CARPETA_USUARIOS.c_str(), NULL);

	string carpetaUsuario = CARPETA_USUARIOS + "\\" + nombreUsuario;

	if(CreateDirectory(carpetaUsuario.c_str(), NULL)){
		cout << "Carpeta creada para el usuario: " << nombreUsuario << endl;
	}else{
		cout << "La carpeta del usuario ya existe o no se pudo crear." << endl;
	}
}

string construirRutaArchivoTareas(string nombreUsuario){
	return CARPETA_USUARIOS + "\\" + nombreUsuario + "\\" + ARCHIVO_TAREAS;
}

//─────ARCHIVOS─────
void guardarTareas(){
	if(archivoTareasGlobal == ""){
		cout << "\nNo hay usuario seleccionado. No se pueden guardar tareas.\n";
		return;
	}

	ofstream archivo(archivoTareasGlobal.c_str());

	if(!archivo.is_open()){
		cout << "\nNo se pudo abrir el archivo para guardar tareas.\n";
		return;
	}

	for(int i = 0; i < (int)tareas.size(); i++){
		archivo << tareas[i].id << ";"
		        << limpiarCampoArchivo(tareas[i].nombre) << ";"
		        << limpiarCampoArchivo(tareas[i].descripcion) << ";"
		        << tareas[i].importancia << ";"
		        << tareas[i].fecha_entrega << ";"
		        << tareas[i].completada << endl;
	}

	archivo.close();
}

void cargarTareas(){
	if(archivoTareasGlobal == ""){
		cout << "\nNo hay usuario seleccionado. No se pueden cargar tareas.\n";
		return;
	}

	ifstream archivo(archivoTareasGlobal.c_str());

	tareas.clear();

	if(!archivo.is_open()){
		return;
	}

	string lineaArchivo;

	while(getline(archivo, lineaArchivo)){
		stringstream ss(lineaArchivo);

		string idTexto;
		string nombre;
		string descripcion;
		string importanciaTexto;
		string fecha;
		string completadaTexto;

		getline(ss, idTexto, ';');
		getline(ss, nombre, ';');
		getline(ss, descripcion, ';');
		getline(ss, importanciaTexto, ';');
		getline(ss, fecha, ';');
		getline(ss, completadaTexto, ';');

		if(idTexto != "" && importanciaTexto != "" && completadaTexto != ""){
			Tarea t;

			t.id = stoi(idTexto);
			t.nombre = nombre;
			t.descripcion = descripcion;
			t.importancia = stoi(importanciaTexto);
			t.fecha_entrega = convertirFechaAFormatoPrograma(fecha);
			t.completada = stoi(completadaTexto);

			tareas.push_back(t);
		}
	}

	archivo.close();
}

//─────MENU USUARIO─────
int menuUsuario(){
	system("cls");

	linea(0);
	centrarTexto("GESTOR DE TAREAS 6761 - menu de usuarios");
	linea(1);
	textoIzquierda("[1] Ingresar usuario.");
	textoIzquierda("[0] Salir.");
	linea(2);

	return leerEntero("Opcion: ", 0, 1);
}

//─────MENU PRINCIPAL─────
int mostrarMenu(){
	system("cls");

	linea(0);
	centrarTexto("GESTOR DE TAREAS 6761");
	linea(1);
	textoIzquierda("[1] Ver tareas.");
	textoIzquierda("[2] Agregar tarea.");
	textoIzquierda("[3] Modificar tarea.");
	textoIzquierda("[4] Borrar tarea.");
	textoIzquierda("[0] Salir.");
	linea(2);

	return leerEntero("Opcion: ", 0, 4);
}
