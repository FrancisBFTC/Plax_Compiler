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

    lineStr = lineStr.substr(lineStr.find(":")+1, lineStr.find("\n"));
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
    string variable = lineStr.substr(lineStr.find("@")+1, lineStr.find(":")-1);
	
    return toString(EraseSpace((char *) variable.c_str()));
}

bool isNumber(string str)
{
    for (int i = 0; i < str.length(); i++) {
        char c = str[i];
        if (std::isdigit(c) == 0 && c != '-') return false;
        if (c == '-' && str.length() == 1) return false;
    }
    return true;
}

int processOperationConst(string operation, int counter, int j){
    int result = 0;
    for(j = 0; j < assembly["operations"][counter].size(); j++){
       string element = assembly["operations"][counter][j];

        if(element == operation){
            string number1 = assembly["operations"][counter][j - 1];
            string number2 = assembly["operations"][counter][j + 1];
            
            if(operation == "*")
                result = stoi(number1) * stoi(number2);
            if(operation == "/")
                result = stoi(number1) / stoi(number2);
            if(operation == "+")
                result = stoi(number1) + stoi(number2);
            if(operation == "-")
                result = stoi(number1) - stoi(number2);

            counter = counter + 1;
            string elem1;
            int y = 0;

            for(int x = 0; x < assembly["operations"][counter-1].size(); x++){
                elem1 = assembly["operations"][counter-1][x];
                if(x == (j - 1)){
                    assembly["operations"][counter][y] = to_string(result).c_str();
                    x = x + 2;
                }else{
                    assembly["operations"][counter][y] = elem1.c_str();
                }
                                                    
                y++;
            }
            j = 0;
        }
    }
    return counter;
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

            int ext_index = 0;
            int text_index = 0;

            assembly["global"][0] = "main";
            assembly["extern"][ext_index++] = "ExitProcess";
            assembly["extern"][ext_index++] = "malloc";
            assembly["extern"][ext_index++] = "realloc";
            assembly["text"][text_index++] = "main:";

            int i = 0;
            int bss = 0;
            
            while((fgets(line, sizeof(line), file_read)) != NULL){
                if(contains(line, "@") && contains(line, ":")){
                    string str = getString(line);
                    string var = getVariable(line);

                    bool is_const = false;

                    if(contains(line, "const['") && contains(line, "']")){
                        assembly["type_vars"][var] = "CONST STRING";
                        is_const = true;
                    }else{
                        if(contains(line, "const[") && contains(line, "]")){
                            assembly["type_vars"][var] = "CONST NUMBER";
                            str = str.substr(str.find("[")+1, str.find("]")-str.find("[")-1);
                            is_const = true;
                        }else{
                            if(contains(line, "'")){
                                assembly["type_vars"][var] = "STRING";
                            }else{
                                assembly["type_vars"][var] = "NUMBER";
                            }
                        }
                    }
                    
                    bool exist_var = false;

                    if(!assembly["vars"][var].is_null())
                        exist_var = true;

                    assembly["vars"][var] = str.c_str();

                    stringstream var_name;
                    bool variable = false;
                    char op;
                    int count_op = 0;
                    for(int i = 0; i < str.length(); i++){
                        variable = false;

                        if(str[i] == '@'){
                            variable = true;
                            ++i;
                        }
                        
                        if(variable){

                            while(i < str.length() && str[i] != '+' && str[i] != '-' && str[i] != '*' && str[i] != '/')
                            {
                                var_name << str[i];
                                i++;
                            }

                            op = str[i];

                            string type = assembly["type_vars"][var_name.str()];
                            string value = assembly["vars"][var_name.str()];

                            if(i == str.length()){

                                if(type == "CONST STRING" || type == "CONST NUMBER")
                                    is_const = true;

                                assembly["type_vars"][var] = type.c_str();
                                assembly["vars"][var] = value.c_str();
                                str = assembly["vars"][var];

                                if(assembly["operations"][0].size() > 1){
                                    int result = 0;
                                    int counter = 0;
                                    assembly["operations"][0][count_op++] = value.c_str();
                                    int j;
                                    
                                    counter = processOperationConst("*", counter, j);
                                    counter = processOperationConst("/", counter, j);
                                    counter = processOperationConst("+", counter, j);
                                    counter = processOperationConst("-", counter, j);

                                    string value_result = assembly["operations"][counter][0];
                                    assembly["vars"][var] = value_result.c_str();
                                    str = assembly["vars"][var];

                                    count_op = 0;
                                }
                            
                            }else{
                                stringstream oper;
                                oper << op;
                                assembly["operations"][0][count_op++] = value.c_str();
                                assembly["operations"][0][count_op++] = oper.str().c_str();
                                var_name.clear();
                                var_name.str("");
                            }

                            variable = false;
                        }else{
                            //cout << "'" << var_name.str() << "' nao e uma variavel" << endl;
                        }
                    }

                    assembly["operations"].clear();

                   if(is_const){
                        if(isNumber(str)){
                            string type = assembly["type_vars"][var];
                            cout << type << ": " << str << ", VAR: @" << var << endl;
                            stringstream var_conc;
                            var_conc << var << " dd " << str;
                            assembly["data"][i] = var_conc.str();
                        }else{
                            string type = assembly["type_vars"][var];
                            cout << type << ": " << "\"" << str << "\", VAR: @" << var << endl;
                            stringstream var_conc;
                            var_conc << var << " db '" << str << "',0";

                            assembly["data"][i] = var_conc.str();
                            var_conc.str("");

                            var_conc << var << ".size equ $ - " << var;
                            assembly["data"][++i] = var_conc.str();
                        }

                        i++;
                   }else{

                        stringstream var_conc;
                    
                        if(!exist_var){
                            if(isNumber(str)){
                                var_conc << var << " dd " << str;
                                assembly["data"][i] = var_conc.str();
                                i++;
                            }else{
                                var_conc << "_" << var << "_" << " resd 1 ";
                                assembly["bss"][bss++] = var_conc.str();
                                assembly["realloc"][var] = false;
                            }
                        }else{
                            
                            if(isNumber(str)){
                                var_conc << "mov DWORD[" << var << "], " << str;
                                assembly["text"][text_index++] = var_conc.str();
                            }else{
                                var_conc << "_" << var << "_";

                                stringstream push_inst;
                                if(!assembly["realloc"][var]){
                                    push_inst << "push " << str.length();
                                    assembly["text"][text_index++] = push_inst.str();
                                    assembly["text"][text_index++] = "call malloc";
                                }else{
                                    push_inst << "push " << str.length();
                                    assembly["text"][text_index++] = push_inst.str();
                                    push_inst.str("");
                                    push_inst << "push DWORD[_" << var << "_]";
                                    assembly["text"][text_index++] = push_inst.str();
                                    assembly["text"][text_index++] = "call realloc";
                                }

                                stringstream addr_inst;
                                addr_inst << "mov [" << var_conc.str() << "], eax";
                                var_conc.str("");
                                var_conc << var << "__" << i << " db '" << str << "',0";

                                assembly["data"][i] = var_conc.str();
                                var_conc.str("");

                                var_conc << var << "__" << i << ".size equ $ - " << var << "__" << i;
                                assembly["data"][++i] = var_conc.str();
                                var_conc.str("");

                                var_conc << "mov esi, [" << var << "__" << i-1 << "]"; 
                                assembly["text"][text_index++] = addr_inst.str();
                                assembly["text"][text_index++] = "mov edi, eax";
                                assembly["text"][text_index++] = var_conc.str();
                                var_conc.str("");

                                var_conc << "mov ecx, " << str.length();
                                assembly["text"][text_index++] = var_conc.str();
                                assembly["text"][text_index++] = "rep movsb";

                                i++;
                            
                                assembly["realloc"][var] = true;
                            }
                        }
                   }

                    
                }
            }

            

            assembly["text"][text_index++] = "push 0";
            assembly["text"][text_index++] = "call ExitProcess";

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

            output << endl;

            for(int i = 0; i < assembly["bss"].size(); i++){
                if(i == 0)
                    output << "section .bss" << endl << endl;
                
                code = assembly["bss"][i];
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
            golink << "GoLink /console /entry main " << file.str() << ".obj /fo " << argv[2] << " msvcrt.dll kernel32.dll user32.dll";
            string golink_cmd = golink.str();
            system(golink_cmd.c_str());

            system("del *.asm");
            system("del *.obj");

            system(argv[2]);

        }
    }

    return 0;
}