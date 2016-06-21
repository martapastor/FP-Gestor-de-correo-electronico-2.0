#ifndef LISTABANDEJA_H
#define LISTABANDEJA_H

#include <iostream>
#include <string>
#include <fstream>
#include "checkML.h"
using namespace std;

typedef struct {
	string identificador;
	bool leido;
} tElemBandeja;

//const int MAX_ELEMS = 100;

typedef struct {
	tElemBandeja * correo;
	int cont;
	int dim;
} tListaBandeja;

//Funciones específicas para la práctica 5:
void iniciar (tListaBandeja & list, int dim);
void liberar (tListaBandeja & list);
void redimensionar (tListaBandeja & list, int dim);

//Funciones de la práctica 4:
inline void iniciar(tListaBandeja & list) { iniciar(list, 10); }
inline bool llena (const tListaBandeja &listElems){ return listElems.cont == listElems.dim; } 
inline int longitud(const tListaBandeja & listElems){ return listElems.cont; }
void guardar (/*const*/ tListaBandeja &listElems, ofstream &archivo);
void cargar (tListaBandeja &listElems, ifstream &archivo);
bool insertar (tListaBandeja &listElems, const tElemBandeja &elem);
int buscar (const tListaBandeja &listElems, const string &idMail);
bool eliminar (tListaBandeja &listElems, const string &idMail);
bool correoLeido (tListaBandeja &listElems, const string &idMail);

#endif