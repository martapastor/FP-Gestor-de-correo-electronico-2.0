#ifndef CORREO_H
#define CORREO_H

#include <iostream>
#include <string>
#include <ctime>
#include <fstream>
#include <sstream>
#include <iomanip>
#include "utilsWin.h"
using namespace std;

const string cent = "X";

typedef time_t tFecha;

typedef struct {
	string id_correo;
	string emisor;
	string destinatario;
	string asunto;
	string cuerpo;
	tFecha fecha;
	int numAccs;
} tCorreo; 

void correoNuevo (tCorreo &correo, const string &emisor);
void correoContestacion (const tCorreo &correoOriginal, tCorreo &correo, const string &emisor);
void guardar (const tCorreo &correo, ofstream &archivo);
bool cargar (tCorreo &correo, ifstream &archivo);
int disminuirNumAccs (tCorreo &correo);
string to_string (const tCorreo &correo);
string cabecera (const tCorreo &correo);

string mostrarFecha (tFecha fecha);
string mostrarSoloDia (tFecha fecha);

//Funciones auxiliares:
void correoReenviar (const tCorreo &correoOriginal, tCorreo &correo, const string &emisor);

#endif