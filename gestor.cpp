#include "listaUsuarios.h"
#include "listaCorreos.h"
#include "correo.h"
#include "usuario.h"
#include "gestor.h"
#include "utilsWin.h"

bool arrancar (tGestor &gestor, const string &dominio)  {
	bool inicioCorrecto = false;
	bool cargaCorreos, cargaUsuarios;

	if (dominio == gestor.dominio) {
		cargaCorreos = cargar(gestor.listCorreos, dominio);
		cargaUsuarios = cargar(gestor.listUsuarios, dominio);

		if (cargaCorreos && cargaUsuarios)
			inicioCorrecto = true;
	}

	return inicioCorrecto;
}

void apagar (/*const*/ tGestor &gestor) {
	guardar(gestor.listCorreos, gestor.dominio);
	guardar(gestor.listUsuarios, gestor.dominio);
}

tUsuario * registraUsuario (tGestor &gestor) {
	tUsuario * usuario = nullptr;

	int pos;
	string nombreUsuario, password;
	cout << left << setw(30) << setfill('-') << '-';
	cout << left << setw(50) << setfill('-') << "INICIAR SESIÓN" << endl;
	cout << "Nombre de usuario: ";
	cin >> nombreUsuario;
	cout << "Contraseña: ";
	password = noMostrarContrasenia();

	if(buscar(gestor.listUsuarios, nombreUsuario, pos)) {
		if (validarContrasenia(*gestor.listUsuarios.usuario[pos], password)) {
			iniciar(*(gestor.listUsuarios.usuario[pos]), nombreUsuario, password);

			usuario = gestor.listUsuarios.usuario[pos];

			//usuario->identificador = nombreUsuario;
			//usuario->password = password;

			clearConsole();
			cout << "¡Bienvenido de nuevo, " << nombreUsuario << "!" << endl;
			cout << "Pulse una tecla para continuar...";
			pausa();
		}
		else {
			usuario = nullptr;
			clearConsole();
			cout << "Error: la contraseña introducida no es correcta." << endl;
			cout << "Pulse una tecla para regresar al menú de inicio...";
			pausa();
		}
	}
	else {
		usuario = nullptr;
		clearConsole();
		cout << "Error: el nombre de usuario introducido no existe." << endl;
		cout << "Pulse una tecla para regresar al menú de inicio...";
		pausa();
	}

	return usuario;
}

tUsuario * crearCuenta (tGestor &gestor) { //Modificada, ya que siempre colocaba el usuario nuevo en la posición 0
	tUsuario * puntUsuario = new tUsuario;

	int pos;
	string nombreUsuario, password, password2;
	cout << left << setw(30) << setfill('-') << '-';
	cout << left << setw(50) << setfill('-') << "CREAR CUENTA NUEVA" << endl;

	cout << "Nombre de usuario: ";
	cin >> nombreUsuario;

	//No nos muestra la contraseña en pantalla cuando la escribimos, aunque sí nos la guarda en los string password y password2:
	cout << "Contraseña: ";
	password = noMostrarContrasenia();
	cout << "\nVuelve a escribir la contraseña: ";
	password2 = noMostrarContrasenia();

	//Comprueba que el nombre de usuario respeta el formato con @dominio.com, y que tanto la contraseña como su confirmación coinciden:
	if (!usuarioCorrecto (nombreUsuario, dominio) || !contraseniaCorrecta(password, password2)) {
		clearConsole();
		puntUsuario = nullptr;
		cout << "Error: su cuenta no ha podido crearse." << endl;
		cout << "Pulse Intro para regresar al menú de inicio...";

		pausa();
	}

	else {
		//Guardamos el identificador del nuevo usuario para buscarlo luego e insertarlo si no existe en listaUsuarios:
		puntUsuario->identificador = nombreUsuario;
		puntUsuario->password = password;
		puntUsuario->bandeja = true;
		iniciar (puntUsuario->listaMnsjRecibidos);
		iniciar (puntUsuario->listaMnsjEnviados);

		if (!buscar(gestor.listUsuarios, nombreUsuario, pos)) {
			if (insertar(gestor.listUsuarios, *puntUsuario)) {
				clearConsole();
				cout << "¡Bienvenido, " << nombreUsuario << "!" << endl;
				cout << "Pulse una tecla para continuar...";
				pausa();
			}

			else {
				puntUsuario = nullptr;
				clearConsole();
				cout << "Error: su cuenta no ha podido crearse." << endl;
				cout << "Pulse una tecla para regresar al menú de inicio...";
				pausa();
			}
		}
		else {
			puntUsuario = nullptr;
			clearConsole();
			cout << "Error: el nombre de usuario no está disponible." << endl;
			cout << "Pulse una tecla para regresar al menú de inicio...";
			pausa();
		}
	}

	return puntUsuario;
}

void enviarCorreo (tGestor &gestor, tUsuario &usuario, const tCorreo &correo) {
	bool insertado = false;
	bool insertEntrada = false;
	bool insertSalida = false;
	int pos;

	if (buscar(gestor.listUsuarios, correo.destinatario, pos)) {
		insertado = insertar(gestor.listCorreos, correo);

		tElemBandeja elem;
		elem.identificador = correo.id_correo; //ATENCIÓN: no confundir usuario.identificador con correo.id_correo
		elem.leido = 0;

		if (insertado) {
			//Insertar mensaje en la bandeja de salida del emisor:
			insertSalida = insertar(usuario.listaMnsjEnviados, elem); //usuario es el usuario activo que ha iniciado sesión

			//Insertar mensaje en la bandeja de entrada del destinatario:
			insertEntrada = insertar(gestor.listUsuarios.usuario[pos]->listaMnsjRecibidos, elem);
		}
	}
	else
		cout << "Error: el destinatario introducido no existe.";

	if (!insertEntrada || !insertSalida)
		cout << "\nError: el mensaje no ha podido enviarse." << endl << endl;
}

void eliminarCorreo (tGestor &gestor, tUsuario &usuario, const string &idMail) {
	bool eliminado = false;
	int pos = -1;

	//Elimina de la bandeja activa del usuario el usuario.listaMnsjRecibidos/Enviados.correo[i].identificador correspondiente al idMail
	if (usuario.bandeja) { //Si la bandeja activa es la de entrada
		eliminado = eliminar(usuario.listaMnsjRecibidos, idMail);
		pos = buscar(usuario.listaMnsjRecibidos, idMail);
	}
	else {
		eliminado = eliminar(usuario.listaMnsjEnviados, idMail);
		pos = buscar(usuario.listaMnsjEnviados, idMail);
	}
	
	//Disminuir número de accesos
	if (eliminado) gestor.listCorreos.correo[pos].numAccs = disminuirNumAccs(gestor.listCorreos.correo[pos]);

	//Si numAccs == 0, entonces elimina el correo de la lista de correos
	if (gestor.listCorreos.correo[pos].numAccs == 0) eliminar(gestor.listCorreos, idMail);
}

//Funciones auxiliares:

bool usuarioCorrecto (const string &nombreUsuario, const string &dominio) {
	bool correcto = false;
	char c;
	string tmpDominio;

	istringstream flujo(nombreUsuario);
	flujo >> c;

	while (c != 64) { //64 corresponde al símbolo @
		flujo >> c;
	}

	flujo >> tmpDominio;

	if (tmpDominio == dominio + ".com")
		correcto = true;

	return correcto;
}

bool contraseniaCorrecta (const string &contrasenia, const string &contrasenia2) {
	bool correcta = false;

	if (contrasenia == contrasenia2)
		correcta = true;

	return correcta;
}

string noMostrarContrasenia() { //ATENCIÓN: esta función sólo sirve para consola MS-DOS/Windows
	string pass = "";
	char c;
	c = _getch();
	
	while (c != 13) { //No funciona si ponemos '\n', y el Intro corresponde al número de caracter 13
		if (c == 8) { //El 8 corresponde a la tecla BACKSPACE
			if (pass.length() != 0) {
				cout << "\b \b";
				pass.resize(pass.length() - 1);
			}
		}

		else {
			pass.push_back(c);
			cout << '*';
		}
		c = _getch();
   }

	return pass;
}