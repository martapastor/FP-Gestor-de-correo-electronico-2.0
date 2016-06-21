#include "listaBandeja.h"
#include "usuario.h"


void iniciar (tUsuario &usuario, const string &idUser, const string &contrasenia) {
	usuario.identificador = idUser;
	usuario.password = contrasenia;
	usuario.bandeja = true; //Entrada -> true
}

void guardar (/*const*/ tUsuario &usuario, ofstream &archivo) {
	archivo << usuario.identificador << endl;
	archivo << usuario.password << endl;

	guardar(usuario.listaMnsjRecibidos, archivo);
	guardar(usuario.listaMnsjEnviados, archivo);

	liberar(usuario.listaMnsjRecibidos);
	liberar(usuario.listaMnsjEnviados);
}

bool cargar (tUsuario &usuario, ifstream &archivo) {
	bool cargaCorrecta = false;

	string idUser, pass;


	archivo >> idUser;
	archivo >> pass;

	if (idUser != "XXX") {
		usuario.identificador = idUser;
		usuario.password = pass;

		cargar(usuario.listaMnsjRecibidos, archivo);
		

		cargar(usuario.listaMnsjEnviados, archivo);
		

		cargaCorrecta = true;
	}

	return cargaCorrecta;
}

bool validarContrasenia (const tUsuario &usuario, const string &contrasenia) {
	return usuario.password == contrasenia;
}

void cambiarBandeja (tUsuario &usuario) {
	if (usuario.bandeja) usuario.bandeja = false;
	else usuario.bandeja = true;
}