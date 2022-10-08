#ifndef __C_UTILS_H__
#define __C_UTILS_H__

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
#include <nlohmann/json.hpp>

using namespace std;
using json = nlohmann::json;

// Converte char* para String
string toString(char *str)
{
	stringstream data1;
	data1 << str;
	return data1.str();
}

// Verifica se um char* contém uma String
bool contains(char* first, string second)
{
	return (toString(first).find(second) == -1) ? false : true;         
}

// Apaga espaços e TABs de um dado
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

// Verifica se uma String é um número
bool isNumber(string str)
{
    for (int i = 0; i < str.length(); i++) {
        char c = str[i];
        if(c != '.')
            if (std::isdigit(c) == 0 && c != '-') return false;

        if (c == '-' && str.length() == 1) return false;
        if (c == '.' && str.length() == 1) return false;
    }
    return true;
}

string replace_all(string s, string x, string y){
	size_t pos = 0;
    while (pos += y.length())
    {
        pos = s.find(x, pos);
        if (pos == std::string::npos) {
            break;
        }
 
        s.replace(pos, x.length(), y);
    }

	return s;
}

#endif