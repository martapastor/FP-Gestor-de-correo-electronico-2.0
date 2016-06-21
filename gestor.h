#ifndef GESTOR_H
#define GESTOR_H

#include <iostream>
#include <string>
#include "listaUsuarios.h"
#include "listaCorreos.h"
#include "correo.h"
#include "usuario.h"
#include "utilsWin.h"
using namespace std;

const string dominio = "fdimail";

typedef struct {
	string dominio;
	tListaCorreos listCorreos;
	tListaUsuarios listUsuarios;
} tGestor;

bool arrancar (tGestor &gestor, const string &dominio);
void apagar (/*const*/ tGestor &gestor);
tUsuario * registraUsuario (tGestor &gestor);
tUsuario * crearCuenta (tGestor &gestor);
void enviarCorreo (tGestor &gestor, tUsuario &usuario, const tCorreo &correo);
void eliminarCorreo (tGestor &gestor, tUsuario &usuario, const string &idMail);

//Funciones auxiliares:
bool usuarioCorrecto (const string &nombreUsuario, const string &dominio);
bool contraseniaCorrecta (const string &contrasenia, const string &contrasenia2);
string noMostrarContrasenia();

#endif