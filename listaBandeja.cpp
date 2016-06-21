#include "listaBandeja.h"

//Funciones específicas para la práctica 5:

void iniciar (tListaBandeja & list, int dim) {
	list.correo = new tElemBandeja[dim];
	list.cont = 0;
	list.dim = dim;
}

void liberar (tListaBandeja & list) {
	delete[] list.correo;
	list.correo = nullptr;
	list.cont = 0;
	list.dim = 0;
}

void redimensionar (tListaBandeja & list, int dim) {
	tElemBandeja * correo;
	if (dim > list.dim) {
		correo = new tElemBandeja[dim];

		for (int i = 0; i < list.cont; i++)
			correo[i] = list.correo[i];

		delete[] list.correo;

		list.correo = correo;
		list.dim = dim;
	}
}

//Funciones de la práctica 4:

void guardar (/*const*/ tListaBandeja &listElems, ofstream &archivo) {
	archivo << listElems.cont << endl;

	for (int i = 0; i < listElems.cont; i++) {
		archivo << listElems.correo[i].identificador << " ";
		if (listElems.correo[i].leido)
			archivo << 1 << endl;
		else
			archivo << 0 << endl;
	}
}

void cargar (tListaBandeja & listElems, ifstream &archivo) {
	int auxCont, leido;
	string correo;

	archivo >> auxCont;
	int dim = (auxCont / 10 + 1) * 10;
	iniciar(listElems, dim);
	listElems.cont = auxCont;

	for (int i = 0; i < listElems.cont; i++) {
		archivo >> correo >> leido;
		listElems.correo[i].identificador = correo;
		if (leido == 1)
			listElems.correo[i].leido = true;
		else
			listElems.correo[i].leido = false;
	}
}

bool insertar (tListaBandeja &listElems, const tElemBandeja &elem) {
	bool inserCorrecta = false;

	if (llena(listElems)) {
		int dim = (listElems.dim * 3) / 2 + 1;
		redimensionar(listElems, dim);
	}

	listElems.correo[listElems.cont].identificador = elem.identificador;
	listElems.correo[listElems.cont].leido = elem.leido;
	listElems.cont++;

	inserCorrecta = true;
	

	return inserCorrecta;
}

int buscar (const tListaBandeja &listElems, const string &idMail) {
	int posicion;
	bool encontrado = false;
	int ini = 0;
	int fin = listElems.cont;
	int mitad = (ini + fin) / 2;
	
	while (ini < fin && encontrado == false) {
		if (idMail < listElems.correo[mitad].identificador)
			fin = mitad;
		else if (idMail > listElems.correo[mitad].identificador)
			ini = mitad + 1;
		else
			encontrado = true;

		mitad = (ini + fin) / 2;
	} 

	if (encontrado == true)
		posicion = mitad;
	else posicion = -1;

	return posicion;
}

bool eliminar (tListaBandeja &listElems, const string &idMail) {
	bool eliminado = false;
	int posicion;

	posicion = buscar(listElems, idMail);

	if (posicion != -1) {
		for (int i = posicion; i < listElems.cont - 1; i++)
			listElems.correo[i] = listElems.correo[i + 1];
		listElems.cont--;
		eliminado = true;
	}

	return eliminado;
}

bool correoLeido (tListaBandeja &listElems, const string &idMail) {
	bool encontrado = false;
	int cont = 0;

	while (cont < listElems.cont && !encontrado) {
		if (listElems.correo[cont].identificador == idMail) {
			listElems.correo[cont].leido = true;
			encontrado = true;
		}

		cont++;
	}

	return encontrado;
}
