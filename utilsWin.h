#ifndef UTILSWIN_H
#define UTILSWIN_H

//Biblioteca necesaria para que al introducir la contraseña por consola, nos muestre asteriscos en vez de la contraseña como tal:
#include <conio.h>
#include <Windows.h>
#include <iostream>
using namespace std;

void pausa();
void clearConsole();
void cpConsoleOut(int cp);

#endif