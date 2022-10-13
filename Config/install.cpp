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
	
	// Inserir dados no Registro do Windows (Pega diretório raiz)
	string rootDir = szDirAtual;
	rootDir.replace(rootDir.find("Config"), 6, "");

	// Cria uma chave principal para menu do mouse e descrição do tipo de arquivo
	cout << "Gravando Registros no Windows de arquivos PLAX" << endl;
	stringstream regadds;
	regadds << "REG ADD HKCR\\plax.file /t REG_SZ /d \"PLAX Programming File\"";
	system(regadds.str().c_str());
	regadds.str("");
	regadds << "REG ADD HKCR\\plax.file\\DefaultIcon /t REG_SZ /d \"" << rootDir << "icon.png\"";
	system(regadds.str().c_str());
	regadds.str("");
	regadds << "REG ADD HKCR\\plax.file\\shell\\Edit /t REG_SZ /d \"&Editar\"";
	system(regadds.str().c_str());
	regadds.str("");
	regadds << "REG ADD HKCR\\plax.file\\shell\\Edit\\command /t REG_SZ /d \"C:\\Program Files\\Notepad++\\notepad++.exe %1\"";
	system(regadds.str().c_str());
	regadds.str("");
	regadds << "REG ADD HKCR\\plax.file\\shell\\Open /t REG_SZ /d \"&Abrir\"";
	system(regadds.str().c_str());
	regadds.str("");
	regadds << "REG ADD HKCR\\plax.file\\shell\\Open\\command /t REG_SZ /d \"" << directory << "\\plax.exe %1\"";
	system(regadds.str().c_str());
	regadds.str("");

	// Cria e Configura uma chave de extensão no ROOT
	regadds << "REG ADD HKCR\\.plax /t REG_SZ /d \"plax.file\"";
	system(regadds.str().c_str());
	regadds.str("");
	regadds << "REG ADD HKCR\\.plax /v \"Content Type\" /t REG_SZ /d \"text/plax\"";
	system(regadds.str().c_str());
	regadds.str("");
	regadds << "REG ADD HKCR\\.plax /v \"PerceivedType\" /t REG_SZ /d \"text\"";
	system(regadds.str().c_str());
	regadds.str("");
	regadds << "REG ADD HKCR\\.plax /v \"plax.bak\" /t REG_SZ /d \"plax.file\"";
	system(regadds.str().c_str());
	regadds.str("");

	regadds << "REG ADD HKCR\\.plax\\OpenWithProgids /t REG_SZ /d \"plax.file\"";
	system(regadds.str().c_str());
	regadds.str("");
	regadds << "REG ADD HKCR\\.plax\\OpenWithProgids /v plax.file /t REG_BINARY";
	system(regadds.str().c_str());
	regadds.str("");

	// Cria e Configura uma chave de extensão no USER
	regadds << "REG ADD HKCU\\Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\FileExts\\.plax\\OpenWithList /t REG_SZ /d \"plax.exe\"";
	system(regadds.str().c_str());
	regadds.str("");
	regadds << "REG ADD HKCU\\Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\FileExts\\.plax\\OpenWithList /v \"a\" /t REG_SZ /d \"C:/Program Files/Notepad++/Notepad++.exe\"";
	system(regadds.str().c_str());
	regadds.str("");
	regadds << "REG ADD HKCU\\Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\FileExts\\.plax\\OpenWithList /v \"b\" /t REG_SZ /d \"C:/Program Files/Sublime Text/sublime_text.exe\"";
	system(regadds.str().c_str());
	regadds.str("");
	regadds << "REG ADD HKCU\\Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\FileExts\\.plax\\OpenWithList /v \"c\" /t REG_SZ /d \"code.exe\"";
	system(regadds.str().c_str());
	regadds.str("");
	regadds << "REG ADD HKCU\\Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\FileExts\\.plax\\OpenWithList /v \"MRUList\" /t REG_SZ /d \"abc\"";
	system(regadds.str().c_str());
	regadds.str("");

	regadds << "REG ADD HKCU\\Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\FileExts\\.plax\\OpenWithProgids /t REG_SZ /d \"plax.file\"";
	system(regadds.str().c_str());
	regadds.str("");
	regadds << "REG ADD HKCU\\Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\FileExts\\.plax\\OpenWithProgids /v plax.file /t REG_BINARY";
	system(regadds.str().c_str());
	regadds.str("");

	// Confira Syntax highlight do VSCode e Sublime Text
	cout << "Configurando Highlight do VSCode e Sublime Text" << endl;
	stringstream dirsyntax;
	dirsyntax << rootDir << "Highlight\\VSCode Syntax\\plax";
	stringstream copy;
	copy << "xcopy /I /E \"" << dirsyntax.str() << "\" \"%userprofile%\\appdata\\Local\\Programs\\Microsoft VS Code\\resources\\app\\extensions\\plax\\\"";
	system(copy.str().c_str());
	dirsyntax.str("");
	copy.str("");

	dirsyntax << rootDir << "Highlight\\Sublime Syntax\\plax.sublime-syntax";
	copy << "copy \"" << dirsyntax.str() << "\" \"%appdata%\\Sublime Text\\Packages\\User\\\"";
	system(copy.str().c_str());
	dirsyntax.str("");
	copy.str("");
}