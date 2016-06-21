#include "correo.h"
#include "listaCorreos.h"

//Funciones específicas para la práctica 5:

void iniciar (tListaCorreos & list, int dim) {
	list.correo = new tCorreo[dim];
	list.cont = 0;
	list.dim = dim;
}

void liberar (tListaCorreos & list) {
	delete[] list.correo;
	list.correo = nullptr;
	list.cont = 0;
	list.dim = 0;
}

void redimensionar (tListaCorreos & list, int dim) {
	tCorreo * correo;
	if (dim > list.dim) {
		correo = new tCorreo[dim];

		for (int i = 0; i < list.cont; i++)
			correo[i] = list.correo[i];

		delete[] list.correo;
		list.correo = correo;
		list.dim = dim;
	}
}

//Funciones de la práctica 4:

void guardar (/*const*/ tListaCorreos &correos, const string &nombre) {
	ofstream fichero;
	string nombreFichero = nombre + "_correos.txt";
	fichero.open(nombreFichero);

	if (fichero.is_open()) {
		fichero << correos.cont << endl;

		for (int i = 0; i < correos.cont; i++) {
			guardar(correos.correo[i], fichero);
		}
	}

	liberar(correos);

	fichero.close();
}

bool cargar (tListaCorreos &correos, const string &nombre) {
	//Como ahora tListaCorreos utiliza un array dinámico, debemos iniciar dicho array:
	
	ifstream fichero;
	string nombreFichero = nombre + "_correos.txt";
	fichero.open(nombreFichero);
	bool cargaCorrecta = false;
	

	if (fichero.is_open()) {
		int auxCont;
		fichero >> auxCont;
		int dim = (auxCont / 10 + 1) * 10;
		iniciar(correos, dim);
		correos.cont = auxCont;
		fichero.ignore();

		cargaCorrecta = true;
		for (int i = 0; i < correos.cont && cargaCorrecta; i++) 
			cargaCorrecta = cargar(correos.correo[i], fichero);
	}

	fichero.close();

	return cargaCorrecta;
}

bool insertar (tListaCorreos &correos, const tCorreo &correo) {
	bool inserCorrecta = false;
	int pos, cont = correos.cont;

	if (llena(correos)) {
		int dim = (correos.dim * 3) / 2 + 1;
		redimensionar(correos, dim);
	}

	if (!buscar(correos, correo.id_correo, pos))  {
		for (int i = cont; pos < i; i--)
			correos.correo[i] = correos.correo[i - 1];
		correos.correo[pos] = correo;
		correos.cont++;
		inserCorrecta = true;
	}
	
	return inserCorrecta;
}

bool buscar (const tListaCorreos &correos, const string &idMail, int &pos) {
	bool encontrado = false;
	int ini = 0;
	int fin = correos.cont;
	int mitad = (ini + fin) / 2;

	while (ini < fin && !encontrado) {
		if (idMail < correos.correo[mitad].id_correo)
			fin = mitad;
		else if (idMail > correos.correo[mitad].id_correo)
			ini = mitad + 1;
		else
			encontrado = true;

		mitad = (ini + fin) / 2;
	} 

	//Tanto si encontrado como si !encontrado, devolver la pos en la que iría
	pos = mitad;

	return encontrado;
}

bool eliminar (tListaCorreos &correos, const string &idMail) {
	bool eliminado = false;
	bool encontrado;
	int pos;

	encontrado = buscar(correos, idMail, pos);

	if (encontrado) 
		eliminado = eliminar(correos, pos);

	return eliminado;
}

//Funciones auxiliares:

bool eliminar (tListaCorreos &list, int pos) {
	for (int i = pos; i < list.cont - 1; i++)
			list.correo[i] = list.correo[i + 1];
		list.cont--;

	return true;
}