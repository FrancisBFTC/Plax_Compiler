// Inclusões de Bibliotecas de C e C++
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <sstream>
#include <string.h>
#include <vector>
#include <windows.h>
#include <fstream>
#include <dirent.h>
#include <cstdlib>
#include <TIME.H>

#include <nlohmann/json.hpp>


using namespace std;
using json = nlohmann::json;

json assembly;

string toString(char *str)
{
	stringstream data1;
	data1 << str;
	return data1.str();
}

bool contains(char* first, string second)
{
	return (toString(first).find(second) == -1) ? false : true;         
}

char * EraseSpace( char * in )
{
    char * p = in;
	char * out;
    int i = 0;
    while( *p ){
        if( !isspace(*p) && strcmp(p, "	") != 0)
            in[i++] = *p;

        p++;
    }
    in[i] = 0;
    return in;
}

string getString(char *line)
{

	string lineStr = toString(line);

    int space = (lineStr.find(0x09) != -1) ? lineStr.find(0x09) : lineStr.find(" ");
    string variable = lineStr.substr(lineStr.find("@"), space);
	int sizeStr = lineStr.find(variable)+variable.length();

	lineStr = lineStr.substr(sizeStr, lineStr.find("\n"));
	lineStr[lineStr.length()-1] = 0;

    if(contains(line, "'")){
        size_t pos = 0;
        int pos1 = 0;
        int pos2 = 0;
        int i = 0;
        while (pos += 1){
            pos = lineStr.find("'", pos);
            if(pos != -1){
                switch(i){
                    case 0: pos1 = pos; break;
                    case 1: pos2 = pos; break;
                }
            }

            if (pos == std::string::npos) 
                break;

            i++;
                    
        }
                    
        return lineStr.substr(pos1+1, pos2-pos1-1);
    }else{
        return toString(EraseSpace((char *) lineStr.c_str()));   
    }
}

string getVariable(char *line)
{
    string lineStr = toString(line);

    int space = (lineStr.find(0x09) != -1) ? lineStr.find(0x09) : lineStr.find(" ");
    string variable = lineStr.substr(lineStr.find("@")+1, space-1);
	
    return variable;
}

bool isNumber(string str)
{
    for (int i = 0; i < str.length(); i++) {
        char c = str[i];
        if (std::isdigit(c) == 0) return false;
    }
    return true;
}

int main(int argc, char** argv) {

    if(argc > 1){
        if(argv[1] != NULL){
            FILE *file_read;
            char line[1024];

            if((file_read = fopen(argv[1], "r")) == NULL){
                cout << "Erro em abrir o arquivo" << endl;
                return 1;
            }

            assembly["global"][0] = "main";
            assembly["extern"][0] = "ExitProcess";
            assembly["text"][0] = "main:";

            int i = 0;
            while((fgets(line, sizeof(line), file_read)) != NULL){
                if(contains(line, "@")){
                    string str = getString(line);
                    string var = getVariable(line);
                    if(isNumber(str)){
                        cout << "NUMBER: " << str << endl;
                        stringstream var_conc;
                        var_conc << var << " dd " << str;
                        assembly["data"][i] = var_conc.str();
                    }else{
                        cout << "STRING: " << "\"" << str << "\"" << endl;
                        stringstream var_conc;
                        var_conc << var << " db '" << str << "',0";

                        assembly["data"][i] = var_conc.str();
                        var_conc.str("");

                        var_conc << var << ".size equ $ - " << var;
                        assembly["data"][++i] = var_conc.str();
                    }

                    i++;
                }
            }

            assembly["text"][1] = "push 0";
            assembly["text"][2] = "call ExitProcess";

            fstream output;
            stringstream file;
            file << argv[1] << ".asm";
		    output.open(file.str(), std::ios_base::app);
            string code;

            for(int i = 0; i < assembly["global"].size(); i++){
                code = assembly["global"][i];
                output << "global " << code << endl;
            }

            output << endl;

            for(int i = 0; i < assembly["extern"].size(); i++){
                code = assembly["extern"][i];
                output << "extern " << code << endl;
            }

            output << endl;

            for(int i = 0; i < assembly["data"].size(); i++){
                if(i == 0)
                    output << "section .data" << endl << endl;
                
                code = assembly["data"][i];
                output << code << endl;
            }

            output << endl;

            for(int i = 0; i < assembly["text"].size(); i++){
                if(i == 0)
                    output << "section .text" << endl << endl;
                
                code = assembly["text"][i];
                output << code << endl;
            }

            output.close();

            //nasm -f win32 test.asm -o test.obj
            stringstream nasm;
            nasm << "nasm -f win32 " << file.str() << " -o " << file.str() << ".obj";
            string nasm_cmd = nasm.str();
            system(nasm_cmd.c_str());

            //GoLink /console /entry main test.obj kernel32.dll user32.dll
            stringstream golink;
            golink << "GoLink /console /entry main " << file.str() << ".obj /fo " << argv[2] << " kernel32.dll user32.dll";
            string golink_cmd = golink.str();
            system(golink_cmd.c_str());

            system("del *.asm");
            system("del *.obj");

            system(argv[2]);

        }
    }

    return 0;
}