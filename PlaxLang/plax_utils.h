#ifndef __PLAX_UTILS_H__
#define __PLAX_UTILS_H__

#include "c_utils.h"

// Variáveis e Objetos
json assembly;
bool is_constant;
string str;
string var;
string func_name;
string type;
int local_offsets = 0;

int ext_index = 0;
int text_index = 0;
int text_funcs = 0;
int rodata_index = 0;
int dll_count = 0;

int bss = 0;
int index_symbol = 0;
bool is_func = false;

// Declarações de Protótipos
string getString(char*);
string getVariable(char*);
int processOperationConst(string, int, int);
void StoreTypeVars(char*);
void parsingVariables(char*);
bool importCommand(string);

string getString(char *line)
{

	string lineStr = toString(line);

    int index_end = lineStr.length();
    int index_start = lineStr.find(":")+1;

    index_end = index_end - (index_start + 1);

    lineStr = lineStr.substr(index_start, index_end);
	lineStr[lineStr.length()] = 0;

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
        if(lineStr.find("import") == -1)
            return toString(EraseSpace((char *) lineStr.c_str())); 
        else
            return lineStr;  
    }
}

string getVariable(char *line)
{
    string lineStr = toString(line);

    int index_end = lineStr.find(":");
    int index_start = lineStr.find("@")+1;

    index_end = index_end - (index_start + 1);

    string variable = lineStr.substr(index_start, index_end);
	
    return toString(EraseSpace((char *) variable.c_str()));
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

void StoreTypeVars(char *line)
{
    is_constant = false;
    
    if(toString(line).find("import") == -1){
        if(contains(line, "const['") && contains(line, "']")){
            assembly["type_vars"][var] = "CONST STRING";
            is_constant = true;
        }else{
            if(contains(line, "const[") && contains(line, "]")){
                assembly["type_vars"][var] = (contains(line, ".")) ? "CONST FLOAT" : "CONST INT";
                str = str.substr(str.find("[")+1, str.find("]")-str.find("[")-1); 

                if(str == "true" || str == "false")
                    assembly["type_vars"][var] = "CONST BOOL";

                is_constant = true;
            }else{
                if(contains(line, "'")){
                    assembly["type_vars"][var] = "STRING";
                }else{
                    assembly["type_vars"][var] = "INT";
                }
                //cout << "TYPE:" << assembly["type_vars"][var] << ", VAR: " << var << endl;
            }
        }
    }else{
        assembly["type_vars"][var] = "RETURN";
    }
}

void parsingVariables(char *line){
    stringstream var_name;
    bool variable = false;
    char op;
    int count_op = 0;
    bool const_finded = true;
    bool has_variable = false;

    if(!contains(line, "const[") && !isNumber(str) && str.find("import") == -1){
        for(int i = 0; i < str.length(); i++){
            variable = false;

            if(str[i] == '@'){
                variable = true;
                ++i;
            }

                while(i < str.length() && str[i] != '+' && str[i] != '-' && str[i] != '*' && str[i] != '/')
                {
                    var_name << str[i++];
                }

                op = str[i];

                string type;
                string value;

                if(variable){
                    type = assembly["type_vars"][var_name.str()];
                    value = assembly["vars"][var_name.str()];
                    has_variable = true;
                }else{
                    value = var_name.str();
                }

                if(i == str.length()){

                    if(variable){
                        if(type == "CONST STRING" || type == "CONST INT" || type == "CONST FLOAT" || type == "CONST BOOL")
                            is_constant = true;

                        if(type.find("CONST") == -1){
                            const_finded = false;
                        }else{
                            if(const_finded){
                                assembly["type_vars"][var] = type.c_str();
                            }
                        }
                        
                        assembly["vars"][var] = value.c_str();
                        str = assembly["vars"][var];
                    }else{
                        if(has_variable && const_finded){
                            is_constant = true;
                            if(isNumber(value) && value.find(".") != -1)
                                assembly["type_vars"][var] = "CONST FLOAT";
                            else
                                if(isNumber(value))
                                    assembly["type_vars"][var] = "CONST INT";
                                else
                                    assembly["type_vars"][var] = "CONST STRING";
                        }else{
                            if(isNumber(value) && value.find(".") != -1)
                                assembly["type_vars"][var] = "FLOAT";
                            else
                                if(isNumber(value))
                                    assembly["type_vars"][var] = "INT";
                                else
                                    assembly["type_vars"][var] = "STRING";

                        }
                    }

                    if(assembly["operations"][0].size() > 1){
                        int result = 0;
                        int counter = 0;
                        assembly["operations"][0][count_op++] = value.c_str();
                        int j;
                                        
                        counter = processOperationConst("*", counter, j);
                        counter = processOperationConst("/", counter, j);
                        counter = processOperationConst("+", counter, j);
                        counter = processOperationConst("-", counter, j);

                        if(variable){
                            string value_result = assembly["operations"][counter][0];
                            assembly["vars"][var] = value_result.c_str();
                            str = assembly["vars"][var];
                        }else{
                            str = assembly["operations"][counter][0];
                            assembly["vars"][var] = str.c_str();
                        }

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
        }
    }  

    assembly["operations"].clear(); 
}

bool importCommand(string lineImport){
                
                    int pushes = 0;
                    int i = 0;
                    bool ok = false;
                    while(lineImport[i] == ' ' || lineImport[i] == 0x09) i++;

                    stringstream importStr;
                    int quant_params = 0;
                    for(; i < lineImport.length(); i++){
                        if(!ok){
                            ok = true;
                            if(lineImport[i] != 'i')
                                break;
                        }


                        importStr << lineImport[i];
                        if(importStr.str() == "import"){
                            
                            if(lineImport[i+1] != ' '){
                                cout << "Erro: Adicione um espaco apos o comando import" << endl;
                                return false;
                            }else{
                                ++i;
                                while(lineImport[i] == ' ' || lineImport[i] == 0x09) i++;

                                stringstream dllName;
                                while(lineImport[i] != '.'){
                                    dllName << lineImport[i];

                                    if(lineImport[i++] == ' '){
                                        cout << "Erro: Adicione um nome de DLL seguido de ponto" << endl;
                                        return false;
                                    }
                                }
                                ++i;


                                stringstream verify;
                                verify << lineImport[i];
                                if(lineImport[i] == ' ' || lineImport[i] == 0x09 || isNumber(verify.str())){
                                    cout << "Erro: A forma escrita apos o ponto nao e permitida" << endl;
                                    return false;
                                }

                                dllName << ".dll";

                                string dll_file = dllName.str();
                                bool already_exist = false;

                                for(int j = 0; j < assembly["dlls"].size(); j++){
                                    string current_dll = assembly["dlls"][j];
                                    if(current_dll == dll_file){
                                        already_exist = true;
                                        break;
                                    }
                                }
                                if(!already_exist)
                                    assembly["dlls"][dll_count++] = dll_file;
                                
                                 

                                stringstream dllfunc;

                                while(lineImport[i] != ' ' && lineImport[i] != '\n'){
                                    dllfunc << lineImport[i];

                                    if(lineImport[i] == '@'){
                                        cout << "Erro: A funcao nao foi escrita de forma correta" << endl;
                                        return false;
                                    }
                                    i++;
                                }

                                already_exist = false;

                                for(int j = 0; j < assembly["extern"].size(); j++){
                                    string current_ext = assembly["extern"][j];
                                    if(current_ext == dllfunc.str()){
                                        already_exist = true;
                                        break;
                                    }
                                }
                                if(!already_exist)
                                    assembly["extern"][ext_index++] = dllfunc.str();

                                while(lineImport[i] == ' ' || lineImport[i] == 0x09) i++;

                                stringstream call_func;
                                if(lineImport[i] == '\n'){
                                    quant_params = 0;
                                    call_func << "\tcall " << dllfunc.str();
                                    if(!is_func){
                                        assembly["text"][text_index++] = call_func.str();
                                    }else{
                                        assembly["text_funcs"][text_funcs++] = call_func.str();
                                    }
                                   break;
                                }

                                int j = i;
                                for(; lineImport[i] != '\n'; i++)
                                    if(lineImport[i] == ',')
                                        quant_params++;

                                ++quant_params;
                                i = j;

                               
                                /*
                                for(j = 0; j < quant_params; j++){
                                    if(!is_func)
                                        assembly["text"][text_index++] = " ";
                                    else
                                        assembly["text_funcs"][text_funcs++] = " ";
                                }
                                */

                                stringstream param;
                                bool is_var = false;
                                for(; lineImport[i] != '\n'; i++){
                                     
                                    if(lineImport[i] == '@')
                                    {
                                        is_var = true;
                                        ++i;
                                    }
                                        

                                    if(lineImport[i] != ',' && lineImport[i] != ' ' && lineImport[i] != '\n')
                                    {
                                        param << lineImport[i];
                                    }else{
                                        if(lineImport[i] != ' '){
                                             
                                            stringstream push_params;
                                            string typevar;

                                            if(!assembly["type_vars"][param.str()].is_null())
                                                typevar = assembly["type_vars"][param.str()];

                                            if(is_var){
                                                
                                                if(!is_func){                
                                                    if(typevar == "CONST INT" || typevar == "CONST FLOAT" || typevar == "INT" || typevar == "FLOAT"){
                                                        push_params << "\tpush DWORD[" << param.str() << "]";
                                                    }else{
                                                        if(typevar == "CONST STRING"){
                                                            push_params << "\tpush " << param.str();
                                                        }else{
                                                            push_params << "\tpush DWORD[_" << param.str() << "_]";
                                                        }
                                                    }
                                                    assembly["pushes_call"][pushes++] = push_params.str();
                                                    //assembly["text"][text_index++] = push_params.str();
                                                }else{
                                                    if(!assembly["local_funcs"][func_name][param.str()].is_null()){
                                                        string params = assembly["local_funcs"][func_name][param.str()];
                                                        push_params << "\tpush " << params;
                                                    }else{
                                                        if(!assembly["local_names"][func_name][param.str()].is_null()){
                                                            string stackname = assembly["local_names"][func_name][param.str()];
                                                            push_params << "\tpush " << stackname;
                                                        }else{
                                                            if(!assembly["vars"][param.str()].is_null()){
                                                                if(typevar == "CONST INT" || typevar == "CONST FLOAT" || typevar == "INT" || typevar == "FLOAT"){
                                                                    push_params << "\tpush DWORD[" << param.str() << "]";
                                                                }else{
                                                                    if(typevar == "CONST STRING"){
                                                                        push_params << "\tpush " << param.str();
                                                                    }else{
                                                                        push_params << "\tpush DWORD[_" << param.str() << "_]";
                                                                    }
                                                                }
                                                            }else{
                                                                cout << "Erro: A variavel " << param.str() << " nao existe!" << endl;
                                                            }
                                                        }
                                                    }
                                                    assembly["pushes_call"][pushes++] = push_params.str();
                                                    //assembly["text_funcs"][text_funcs++] = push_params.str();
                                                }
                                            }else{
                                                if(!is_func){
                                                    push_params << "\tpush " << param.str();
                                                    //assembly["text"][text_index++] = push_params.str();
                                                }else{
                                                    push_params << "\tpush " << param.str();
                                                    //assembly["text_funcs"][text_funcs++] = push_params.str();
                                                }
                                                assembly["pushes_call"][pushes++] = push_params.str();
                                            }
                                            is_var = false;
                                            
                                            param.str("");
                                        }
                            
                                    }

                                    if(lineImport[i+1] == '\n'){
                                        
                                            stringstream push_params;
                                            string typevar;
                                            if(!assembly["type_vars"][param.str()].is_null())
                                                typevar = assembly["type_vars"][param.str()];
                                                
                                            if(is_var){
                                                if(!is_func){                
                                                    if(typevar == "CONST INT" || typevar == "CONST FLOAT" || typevar == "INT" || typevar == "FLOAT"){
                                                        push_params << "\tpush DWORD[" << param.str() << "]";
                                                    }else{
                                                        if(typevar == "CONST STRING"){
                                                            push_params << "\tpush " << param.str();
                                                        }else{
                                                            push_params << "\tpush DWORD[_" << param.str() << "_]";
                                                        }
                                                    }
                                                    assembly["pushes_call"][pushes++] = push_params.str();
                                                    //assembly["text"][text_index++] = push_params.str();
                                                }else{
                                                    if(!assembly["local_funcs"][func_name][param.str()].is_null()){
                                                        string params = assembly["local_funcs"][func_name][param.str()];
                                                        push_params << "\tpush " << params;
                                                    }else{
                                                        if(!assembly["local_names"][func_name][param.str()].is_null()){
                                                            string stackname = assembly["local_names"][func_name][param.str()];
                                                            push_params << "\tpush " << stackname;
                                                        }else{
                                                            if(!assembly["vars"][param.str()].is_null()){
                                                                if(typevar == "CONST INT" || typevar == "CONST FLOAT" || typevar == "INT" || typevar == "FLOAT"){
                                                                    push_params << "\tpush DWORD[" << param.str() << "]";
                                                                }else{
                                                                    if(typevar == "CONST STRING"){
                                                                        push_params << "\tpush " << param.str();
                                                                    }else{
                                                                        push_params << "\tpush DWORD[_" << param.str() << "_]";
                                                                    }
                                                                }
                                                            }else{
                                                                cout << "Erro: A variavel " << param.str() << " nao existe!" << endl;
                                                            }
                                                        }
                                                    }
                                                    assembly["pushes_call"][pushes++] = push_params.str();
                                                    //assembly["text_funcs"][text_funcs++] = push_params.str();
                                                }
                                            }else{
                                                if(!is_func){
                                                    push_params << "\tpush " << param.str();
                                                    //assembly["text"][text_index++] = push_params.str();
                                                }else{
                                                    push_params << "\tpush " << param.str();
                                                    //assembly["text_funcs"][text_funcs++] = push_params.str();
                                                }
                                                assembly["pushes_call"][pushes++] = push_params.str();
                                            }
                                            is_var = false;

                                            push_params.str("");
                                            push_params << "\tcall " << dllfunc.str();

                                            for(int x = assembly["pushes_call"].size()-1; x >= 0 ; x--){
                                                string push_call = assembly["pushes_call"][x];
                                                if(!is_func){
                                                    assembly["text"][text_index++] = push_call;
                                                    if(x == 0){
                                                        assembly["text"][text_index++] = push_params.str();
                                                        push_params.str("");
                                                        push_params << "\tadd esp, " << assembly["pushes_call"].size() * 4;
                                                        assembly["text"][text_index++] = push_params.str();
                                                    }                                                
                                                }else{
                                                    assembly["text_funcs"][text_funcs++] = push_call;
                                                    if(x == 0){
                                                        assembly["text_funcs"][text_funcs++] = push_params.str();
                                                        push_params.str("");
                                                        push_params << "\tadd esp, " << assembly["pushes_call"].size() * 4;
                                                        assembly["text_funcs"][text_funcs++] = push_params.str();
                                                    }
                                                }
                                            }

                                            param.str("");
                                            

                                    }
                                }

                                break;
                                
                            }
                        }
                    }  

                return true; 
}

#endif