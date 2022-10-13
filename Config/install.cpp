#include <cstdlib>
#include <iostream>
#include <windows.h>
#include <string>
#include <sstream>

using namespace std;

int main(int argc, char *argv[])
{
	// Descobre o diretório atual e forma o diretório do compilador
	TCHAR szDirAtual[MAX_PATH];
	GetCurrentDirectory(MAX_PATH, szDirAtual);
	
	string directory = szDirAtual;
	directory.replace(directory.find("Config"), 6, "PlaxLang");
	
	// Insere diretório na variável de ambiente PATH
	cout << "Inserindo '" << directory << "' na variavel de ambiente PATH" << endl;
	stringstream environment;
	environment << "setx PATH \"%PATH%;" << directory << ";\" /M";
	system(environment.str().c_str());
	
	
}