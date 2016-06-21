#ifndef USUARIO_H
#define USUARIO_H

#include <iostream>
#include <string>
#include <fstream>
#include "listaBandeja.h"
using namespace std;

typedef struct {
	string identificador;
	string password;
	tListaBandeja listaMnsjRecibidos;
	tListaBandeja listaMnsjEnviados;
	bool bandeja; //true para la bandeja de entrada, false para la bandeja de salida
} tUsuario;

void iniciar (tUsuario &usuario, const string &idUser, const string &contrasenia);
void guardar (/*const*/ tUsuario &usuario, ofstream &archivo);
bool cargar (tUsuario &usuario, ifstream &archivo);
bool validarContrasenia (const tUsuario &usuario, const string &contrasenia);
void cambiarBandeja (tUsuario &usuario);

#endif