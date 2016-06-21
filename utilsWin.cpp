#include "utilsWin.h"

void pausa() { 
	cin.clear();
	cin.sync();
	cin.get();
}

void clearConsole() {
	system("cls");
}

void cpConsoleOut (int cp) {
	SetConsoleOutputCP(cp);
}
