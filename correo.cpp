#include "correo.h"
#include "utilsWin.h"

void correoNuevo (tCorreo &correo, const string &emisor) {
	clearConsole();
	correo.emisor = emisor;
	correo.fecha = time(0);
	correo.id_correo = correo.emisor + "_" + to_string(correo.fecha);

	//Cabecera:
	cout << "De: " << correo.emisor << endl;
	cin.ignore();
	cout << "Para: ";
	getline(cin, correo.destinatario);
	cout << "Asunto: ";
	getline(cin, correo.asunto);
	
	//Cuerpo:
	string lineaCuerpo;
	ostringstream flujo;
	getline(cin, lineaCuerpo);

	if (lineaCuerpo == cent) correo.cuerpo = "(Sin cuerpo)\n";

	else {
		while (lineaCuerpo != cent) {
			flujo << lineaCuerpo << endl;
			getline(cin, lineaCuerpo);
		}
		correo.cuerpo = flujo.str();
	}
}

void correoContestacion (const tCorreo &correoOriginal, tCorreo &correo, const string &emisor) {
	clearConsole();
	correo.emisor = emisor;
	correo.fecha = time(0);
	correo.id_correo = correo.emisor + "_" + to_string(correo.fecha);
	correo.destinatario = correoOriginal.emisor;
	correo.asunto = "RE: " + correoOriginal.asunto;

	//Cabecera:
	cout << "De: " << correo.emisor << endl;
	cout << "Para: " << correo.destinatario << endl;
	cout << "Asunto: " << correo.asunto << endl;

	//Cuerpo:
	string lineaCuerpo;
	ostringstream flujo;
	getline(cin, lineaCuerpo);

	while (lineaCuerpo != cent) {
		flujo << lineaCuerpo << endl;
		getline(cin, lineaCuerpo);
	}

	flujo << endl << endl << mostrarFecha(correoOriginal.fecha) << endl; //Cambiar esto y poner guiones o algo para que quede bonito
	flujo << correoOriginal.emisor << endl;
	flujo << correoOriginal.destinatario << endl;
	flujo << correoOriginal.asunto << endl << endl;
	flujo << correoOriginal.cuerpo << endl;

	correo.cuerpo = flujo.str();
}

void guardar (const tCorreo &correo, ofstream &archivo) {
	archivo << correo.id_correo << endl;
	archivo << correo.fecha << endl;
	archivo << correo.emisor << endl;
	archivo << correo.destinatario << endl;
	archivo << correo.asunto << endl;
	archivo << correo.cuerpo;
	archivo << cent << endl;
}

bool cargar (tCorreo &correo, ifstream &archivo) {
	//Cabecera:
	if (archivo.eof() || archivo.fail()) return false; //¿archivo.eof() o archivo.fail()?

	else {
		getline(archivo, correo.id_correo);
		archivo >> correo.fecha;
		archivo >> correo.emisor;
		archivo >> correo.destinatario;
		archivo.ignore();
		getline(archivo, correo.asunto);

		//Cuerpo:
		string lineaCuerpo;
		ostringstream flujo;
		getline(archivo, lineaCuerpo);

		if (lineaCuerpo == cent) correo.cuerpo = "(Sin cuerpo)";

		else {
			while (lineaCuerpo != cent) {
				flujo << lineaCuerpo << endl;
				getline(archivo, lineaCuerpo);
			}
			correo.cuerpo = flujo.str();
		}

	return true;
	}
}

int disminuirNumAccs (tCorreo &correo) { return correo.numAccs - 1; }

string to_string (const tCorreo &correo) {
	string correoCompleto;
	ostringstream flujo;
	
	flujo << "De: " << correo.emisor;
	flujo << setw(50) << mostrarFecha(correo.fecha) << endl;
	flujo << "Para: " << correo.destinatario << endl;
	flujo << "Asunto: " << correo.asunto << endl << endl;
	flujo << correo.cuerpo << endl;
	correoCompleto = flujo.str();

	return correoCompleto;
}

string cabecera (const tCorreo &correo) {
	string cabecera;
	ostringstream flujo;

	flujo << left << setw(26) << correo.emisor << setw(32) << correo.asunto << setw(16) << mostrarSoloDia(correo.fecha);
	cabecera = flujo.str();

	return cabecera;
}

string mostrarFecha (tFecha fecha){
  ostringstream resultado;
  tm ltm;
  localtime_s(&ltm, &fecha);
  resultado << 1900 + ltm.tm_year << '/' << 1 + ltm.tm_mon << '/' << ltm.tm_mday;
  resultado << " <" << ltm.tm_hour << ':' << ltm.tm_min << ':' << ltm.tm_sec << '>';
  return resultado.str(); 
}

string mostrarSoloDia (tFecha fecha){
  ostringstream resultado;
  tm ltm;
  localtime_s(&ltm, &fecha);
  resultado << 1900 + ltm.tm_year << '/' << 1 + ltm.tm_mon << '/' << ltm.tm_mday;
  return resultado.str();
} 

//Funciones auxiliares:

void correoReenviar (const tCorreo &correoOriginal, tCorreo &correo, const string &emisor) {
	clearConsole();
	correo.emisor = emisor;
	correo.fecha = time(0);
	correo.id_correo = correo.emisor + "_" + to_string(correo.fecha);
	correo.asunto = "FW: " + correoOriginal.asunto;

	//Cabecera:
	cout << "De: " << correo.emisor << endl;
	cout << "Para: ";
	cin >> correo.destinatario;
	cout << "Asunto: " << correo.asunto << endl;

	//Cuerpo:
	string lineaCuerpo;
	ostringstream flujo;

	flujo << endl << endl << mostrarFecha(correoOriginal.fecha) << endl; //Cambiar esto y poner guiones o algo para que quede bonito
	flujo << correoOriginal.emisor << endl;
	flujo << correoOriginal.destinatario << endl;
	flujo << correoOriginal.asunto << endl << endl;
	flujo << correoOriginal.cuerpo << endl;

	correo.cuerpo = flujo.str();

	cout << correo.cuerpo;

	cout << "Pulse Intro para enviar...";
	pausa();
}