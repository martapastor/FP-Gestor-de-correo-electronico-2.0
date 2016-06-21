#include "gestor.h"
#include "usuario.h"
#include "sesion.h"
#include "utilsWin.h"

bool iniciarSesion (tSesion &sesion, tGestor * gestor, tUsuario * usuario) {
	tListaBandeja * bandejaActiva;

	if (gestor == nullptr && usuario == nullptr) return false;

	else {
		sesion.usuario->bandeja = true; //Ponemos la bandeja de entrada como bandeja activa por defecto (true -> entrada, false -> salida)

		//Completa el numAccs a cada correo de listaCorreos:
		for (int i = 0; i < sesion.gestor->listCorreos.cont; i++) { //Cada correo
			int numAccs = 0;
			for (int j = 0; j < sesion.gestor->listUsuarios.cont; j++) { //Cada usuario
				for (int k = 0; k < sesion.gestor->listUsuarios.usuario[j]->listaMnsjRecibidos.cont; k++) { //Cada correo de la listaBandeja del usuario
					if (sesion.gestor->listUsuarios.usuario[j]->listaMnsjRecibidos.correo[k].identificador == sesion.gestor->listCorreos.correo[i].id_correo) {
						numAccs++;
					}
				}

				for (int k = 0; k < sesion.gestor->listUsuarios.usuario[j]->listaMnsjEnviados.cont; k++) {
					if (sesion.gestor->listUsuarios.usuario[j]->listaMnsjEnviados.correo[k].identificador == sesion.gestor->listCorreos.correo[i].id_correo) {
						numAccs++;
					}
				}

				sesion.gestor->listCorreos.correo[i].numAccs = numAccs;
			}
		}

		//sesion.usuario = usuario;

		//Muestra la interfaz de la bandeja de entrada/salida:
		encabezadoBandeja(sesion);
		int opcion = menuBandeja(sesion);

		while (opcion != 0) {
			if (sesion.usuario->bandeja) 
				bandejaActiva = &sesion.usuario->listaMnsjRecibidos;
			else 
				bandejaActiva = &sesion.usuario->listaMnsjEnviados;
				

			//Opciones del menú:
			if (opcion == 1) {
				if (bandejaActiva->cont != 0) 
						leerCorreo(sesion);
				else {
					cout << "\nNo hay correos para mostrar." << endl;
					pausa();
				}
			}
			else if (opcion == 2) 
				enviarCorreo(sesion);
			
			else if (opcion == 3) 
				borrarCorreo(sesion);
			
			else if (opcion == 4) 
				cambiarBandeja(*sesion.usuario);
			
			else if (opcion == 5) {
				//Los correos deben aparecer ordenados por asunto (ignorando RE: y los FW:) y por fecha
				lecturaRapida(sesion);
				cout << "Pulse Intro para regresar a la bandeja...";
				pausa();
			}

			clearConsole();
			encabezadoBandeja(sesion);
			opcion = menuBandeja(sesion);
		}

		if (opcion == 0) {
			clearConsole();
			cout << "¡Hasta pronto, " << sesion.usuario->identificador << "!" << endl;
			cout << "Pulse Intro para regresar al menú de inicio...";
			pausa();
		}

		return true;
	}
}

//Opciones del menú:

void leerCorreo (tSesion &sesion) {
	tListaBandeja * bandejaActiva;
	int opcion, pos, cont;
	string identificador;
	
	cout << left << setw(80) << setfill('-') << '-';
	cout << "LEER CORREO" << endl;
	cout << "¿Qué correo desea leer? : ";
	cin >> opcion;

	if (sesion.usuario->bandeja) 
		bandejaActiva = &sesion.usuario->listaMnsjRecibidos;	
	else 
		bandejaActiva = &sesion.usuario->listaMnsjEnviados;

	cont = bandejaActiva->cont;
	while (!buscar(sesion.gestor->listCorreos, bandejaActiva->correo[cont - opcion].identificador, pos)) {
		cout << "Error: por favor, seleccione un correo válido." << endl;
		cin >> opcion;
	}

	identificador = bandejaActiva->correo[cont - opcion].identificador;
	correoLeido(*bandejaActiva, identificador);

	//Muestra el menú con las opciones para contestar o reenviar el correo:
	if (buscar(sesion.gestor->listCorreos, identificador, pos)) {
		int opcionCorreo;

		clearConsole();
		cout << to_string(sesion.gestor->listCorreos.correo[pos]);

		cout << left << setw(80) << setfill('-') << '-';
		cout << "Elija una opción: " << endl;
		cout << "1 - Contestar al correo." << endl;
		cout << "2 - Reenviar el correo." << endl;
		cout << "0 - Volver al menú principal." << endl;
		cout << left << setw(80) << setfill('-') << '-';
		cout << "Introduzca una opción: ";
		cin >> opcionCorreo;

		while (opcionCorreo < 0 || opcionCorreo > 2) {
			cout << "\nError: introduzca una opción válida.";
			cin >> opcionCorreo;
		}

		if (opcionCorreo == 1) {
			tCorreo correoNuevo;
			correoContestacion(sesion.gestor->listCorreos.correo[pos], correoNuevo, sesion.usuario->identificador);
			enviarCorreo(*sesion.gestor, *sesion.usuario, correoNuevo);
		}
		else if (opcionCorreo == 2) {
			tCorreo correoNuevo;
			correoReenviar(sesion.gestor->listCorreos.correo[pos], correoNuevo, sesion.usuario->identificador);
			enviarCorreo(*sesion.gestor, *sesion.usuario, correoNuevo);
		}
	}
}

void enviarCorreo (tSesion &sesion) {
	tCorreo correo;
	correoNuevo(correo, sesion.usuario->identificador);
	enviarCorreo(*sesion.gestor, *sesion.usuario, correo);
}

void borrarCorreo (tSesion &sesion) {
	int opcion, cont, pos;
	tListaBandeja * bandejaActiva;

	cout << left << setw(80) << setfill('-') << '-';
	cout << "BORRAR UN CORREO" << endl;
	cout << "¿Qué correo desea borrar?: ";
	cin >> opcion;

	if (sesion.usuario->bandeja) 
		bandejaActiva = &sesion.usuario->listaMnsjRecibidos;	
	else 
		bandejaActiva = &sesion.usuario->listaMnsjEnviados;

	cont = bandejaActiva->cont;
		if (buscar(sesion.gestor->listCorreos, bandejaActiva->correo[cont - opcion].identificador, pos))
			eliminarCorreo(*sesion.gestor, *sesion.usuario, sesion.gestor->listCorreos.correo[pos].id_correo);	
}

void lecturaRapida (tSesion &sesion) {
	tListaCorreos correos;
	correos.correo = nullptr;
	correos.cont = 0;

	tListaBandeja * bandejaActiva;
	
	if (sesion.usuario->bandeja) 
		bandejaActiva = &sesion.usuario->listaMnsjRecibidos;
	else 
		bandejaActiva = &sesion.usuario->listaMnsjEnviados;
	
	iniciar(correos);
	//correos.cont = 0;

	//Copiamos los datos de sesion.gestor->listCorreos a un array auxiliar:
	tListaCorreos listaCorreos;
	iniciar(listaCorreos);
	listaCorreos.cont = sesion.gestor->listCorreos.cont;

	for (int i = 0; i < listaCorreos.cont; i++) {
		listaCorreos.correo[i] = sesion.gestor->listCorreos.correo[i];
	}

	//Buscamos los correos no leídos:
	int pos;
	for (int i = 0; i < bandejaActiva->cont; i++) {
		if (!bandejaActiva->correo[i].leido){		
			if (buscar(sesion.gestor->listCorreos, bandejaActiva->correo[i].identificador, pos)) {
				if (llena(correos)) {
					int dim = (correos.dim * 3) / 2 + 1;
					redimensionar(correos, dim);
				}

				correos.correo[correos.cont] = listaCorreos.correo[pos];
				correos.cont++;
			}

			//Marcamos los correos como leídos:
			bandejaActiva->correo[i].leido = true;
		}
	}

	//Ordenamos el array auxiliar por el método de ordenación por selección:
	ordenarSeleccion(correos, correos.cont);

	//Mostramos el array con los correos ya ordenados por asunto:
	clearConsole();
	for (int i = 0; i < correos.cont; i++) {
		cout << to_string(correos.correo[i]);
		cout << left << setw(80) << setfill('-') << '-';
	}

	liberar(correos);
}

//Funciones auxiliares:

int menuBandeja(tSesion &sesion) {
	int opcion;

	cout << left << setw(80) << setfill('-') << '-';

	cout << "Elija una opción:" << endl;
	cout << "1 - Leer correo." << endl;
	cout << "2 - Enviar correo." << endl;
	cout << "3 - Borrar correo." << endl;
	if (sesion.usuario->bandeja)
		cout << "4 - Ver bandeja de salida." << endl;
	else 
		cout << "4 - Ver bandeja de entrada." << endl;
	cout << "5 - Lectura rápida de correos sin leer." << endl;
	cout << "0 - Cerrar sesión." << endl;

	cout << left << setw(80) << setfill('-') << '-';

	cout << "Introduzca una opción: ";
	cin >> opcion;

	while (opcion < 0 || opcion > 5) {
		cout << "\nError: la opción introducida no es válida." << endl;
		cout << "Introduzca una opción: ";
		cin >> opcion;
	}

	return opcion;
}

void encabezadoBandeja(tSesion &sesion) {
	clearConsole();

	int pos;
	tListaBandeja * bandejaActiva;

	cout << "Gestor de correo de " << sesion.usuario->identificador << endl;
	cout << setw(30) << setfill('-') << '-';

	if (sesion.usuario->bandeja)
		cout << left << setw(50) << setfill('-') << "Bandeja de entrada";
	else 
		cout << left << setw(50) << setfill('-') << "Bandeja de salida";

	cout << left << setw(80) << setfill('-') << '-';
	cout << "L" << " " << "N" << "   " << "EMISOR" << '\t' << '\t' << '\t' << "ASUNTO" << '\t' << '\t' << '\t' << '\t' << "FECHA" << endl;
	cout << left << setw(80) << setfill('-') << '-';
	
	if (sesion.usuario->bandeja) 
		bandejaActiva = &sesion.usuario->listaMnsjRecibidos;
	else 
		bandejaActiva = &sesion.usuario->listaMnsjEnviados;

	if (bandejaActiva->cont > 0) {
		for (int i = bandejaActiva->cont, j = 0; 0 <= i && j < bandejaActiva->cont; i--, j++) {
			//Nos devuelve la posición del correo en listaCorreos (&pos):
			buscar(sesion.gestor->listCorreos, bandejaActiva->correo[i - 1].identificador, pos);

			if (bandejaActiva->correo[i - 1].leido) 
				cout << "  " << j + 1 << " - ";
			else 
				cout << "* " << j + 1 << " - ";

			cout  << cabecera (sesion.gestor->listCorreos.correo[pos]);
		}
	}

	else 
		cout << endl << "No hay elementos en la bandeja de entrada para mostrar." << endl << endl;

}

void ordenarSeleccion(tListaCorreos &correos, int longitud) {
	for (int i = 0; i < longitud - 1; i++) {
		int menor = i;

		//Comparamos los asuntos:
		for (int j = i + 1; j < longitud; j++) {
			if (ignorarREyFW(correos.correo[j]) < ignorarREyFW(correos.correo[menor])) menor = j;
		}

		//Intercambiamos el menor con correos.arrCorreos[i]
		if (i < menor) {
			tCorreo aux = correos.correo[i];
			correos.correo[i] = correos.correo[menor];
			correos.correo[menor] = aux;
		}
	}
}

string ignorarREyFW (tCorreo &correo) {
	string asunto;
	char c;

	//Va leyendo el asunto char a char:
	bool noCorreoContest = false;
	bool noCorreoReenviar = false;
	istringstream flujo(correo.asunto);

	while (flujo >> c && (!noCorreoContest || !noCorreoReenviar)) {
		//Tiene que encontrar 'R', 'E', ':', ' ':
		if (c == 'R' && !noCorreoContest) {
			flujo >> c;
			if (c == 'E' && !noCorreoContest) {
				flujo >> c;
				if (c == ':' && !noCorreoContest) {
					flujo >> c;
					if (c == ' ' && !noCorreoContest)
						flujo >> c;
					else
						noCorreoContest = true;
				}
				else
					noCorreoContest = true;
			}
			else
				noCorreoContest = true;
		}
		else
			noCorreoContest = true;

		//Tiene que encontrar 'F', 'W', ':', ' ':
		if (c == 'F' && !noCorreoReenviar) {
			flujo >> c;
			if (c == 'W' && !noCorreoReenviar) {
				flujo >> c;
				if (c == ':' && !noCorreoReenviar) {
					flujo >> c;
					if (c == ' ' && !noCorreoReenviar)
						flujo >> c;
					else
						noCorreoReenviar = true;
				}
				else
					noCorreoReenviar = true;
			}
			else
				noCorreoReenviar = true;
		}
		else
			noCorreoReenviar = true;

	}

	flujo >> asunto;

	return asunto;
}