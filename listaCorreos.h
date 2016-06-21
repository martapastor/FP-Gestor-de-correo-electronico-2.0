#ifndef LISTACORREOS_H
#define LISTACORREOS_H

#include <iostream>
#include <string>
#include <fstream>
#include "correo.h"
#include "checkML.h"
using namespace std;

typedef struct {
	tCorreo * correo;
	int cont;
	int dim;
} tListaCorreos;

//Funciones específicas para la práctica 5:
void iniciar (tListaCorreos & list, int dim);
void liberar (tListaCorreos & list);
void redimensionar (tListaCorreos & list, int dim);

//Funciones de la práctica 4:
inline void iniciar(tListaCorreos & list) { iniciar(list, 10); }
inline bool llena (const tListaCorreos &list){ return list.cont == list.dim; } 
inline int longitud (const tListaCorreos &correos){ return correos.cont; }
void guardar (/*const*/ tListaCorreos &correos, const string &nombre);
bool cargar (tListaCorreos &correos, const string &nombre);
bool insertar (tListaCorreos &correos, const tCorreo &correo);
bool buscar (const tListaCorreos &correos, const string &idMail, int &pos);
bool eliminar (tListaCorreos &correos, const string &idMail);

//Funciones auxiliares:
bool eliminar (tListaCorreos &list, int pos);

#endif