#ifndef LISTAUSUARIOS_H
#define LISTAUSUARIOS_H

#include "usuario.h"
#include "checkML.h"
const int MAX_USUARIOS = 100;


typedef struct {
	tUsuario * usuario[MAX_USUARIOS];
	int cont;
} tListaUsuarios;

//Funciones específicas de la práctica 5:
void liberar (tListaUsuarios &usuarios);

//Funciones de la práctica 4:
void iniciar (tListaUsuarios & usuarios);
inline bool llena (const tListaUsuarios &usuarios){ return usuarios.cont == MAX_USUARIOS; }
inline int longitud (const tListaUsuarios &usuarios){ return usuarios.cont; }
bool cargar (tListaUsuarios &usuarios, const string &nombre);
void guardar (/*const*/ tListaUsuarios &usuarios, const string &nombre);
bool insertar (tListaUsuarios &usuarios, /*const*/ tUsuario &usuario);
bool buscar (const tListaUsuarios &usuarios, const string &idUser, int &posicion);

#endif