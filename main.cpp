/* Pr�ctica 5 - Gestor de correo electr�nico 2.0
Marta Pastor Puente y �ngela Mart�nez Ros
1� Doble Grado en Matem�ticas e Ingenier�a Inform�tica
*/

#include "sesion.h"
#include "gestor.h"
#include "usuario.h"
#include "utilsWin.h"
using namespace std;

int menuInicio();

int main() {
	_CrtSetDbgFlag ( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );

	cpConsoleOut(1252);

	tGestor gestor;
	tSesion sesion = {nullptr, nullptr};

	gestor.dominio = dominio;

	if (!arrancar(gestor, dominio)) {
		iniciar(gestor.listCorreos);
		iniciar(gestor.listUsuarios);
	}

	int opcionMenu = menuInicio(); //Muestra el men� para iniciar sesi�n o crear una cuenta nueva (funci�n auxiliar)
	
	while (opcionMenu != 0) {
		if (opcionMenu == 1) { //Pide los datos de un usuario ya registrado
			clearConsole();
			sesion.usuario = registraUsuario(gestor);
		}

		else if (opcionMenu == 2) { //Pide los datos para crear un nuevo usuario
			clearConsole();
			sesion.usuario = crearCuenta(gestor);
		}
			
		if (sesion.usuario != nullptr)
			sesion.gestor = &gestor;
		else
			sesion.gestor = nullptr;

		//Devuelve false si sesion.usuario == nullptr && sesion.gestor == nullptr
		bool sesionIniciada = iniciarSesion(sesion, sesion.gestor, sesion.usuario);

		if (!sesionIniciada) {
			clearConsole();
			cout << "Error: no se ha podido iniciar sesi�n." << endl;
			cout << "Pulse Intro para regresar al men� principal...";
			pausa();
		}

		opcionMenu = menuInicio();
	}

	if (opcionMenu == 0) { //Salir del gestor
		apagar(gestor);

		clearConsole();
		cout << "�Hasta pronto! Pulse Intro para continuar...";
		pausa();
	}

	return 0;
}

//Funciones auxiliares:

int menuInicio() {
	clearConsole();

	int opcionMenu;
	cout << left << setw(30) << setfill('-') << '-';
	cout << left << setw(50) << setfill('-') << "GESTOR DE CORREO " + dominio;
	cout << "Elija la opci�n deseada: " << endl;
	cout << left << setw(5) << "1 - Acceder a mi cuenta de correo" << endl;
	cout << left << setw(5) << "2 - Crear cuenta de correo" << endl << endl;
	cout << left << setw(5) << "0 - Salir" << endl;
	cout << left << setw(80) << setfill('-') << '-';
	cin >> opcionMenu;

	while (opcionMenu < 0 || opcionMenu > 2) {
		clearConsole();
		cout << left << setw(30) << setfill('-') << '-';
		cout << left << setw(50) << setfill('-') << "GESTOR DE CORREO " + dominio;
		cout << "Por favor, introduzca una opci�n v�lida." << endl << endl;

		cout << "Elija la opci�n deseada: " << endl;
		cout << left << setw(5) << "1 - Acceder a mi cuenta de correo." << endl;
		cout << left << setw(5) << "2 - Crear cuenta de correo." << endl << endl;
		cout << left << setw(5) << "0 - Salir." << endl;
		cout << left << setw(80) << setfill('-') << '-';
		cin >> opcionMenu;
	}

	return opcionMenu;
}