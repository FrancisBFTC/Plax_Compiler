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
int data_index = 0;
int dll_count = 0;
int pushes2 = 0;
int i = 0;



int bss = 0;
int index_symbol = 0;
bool is_func = false;
bool reading_func = false;
bool use_all = false;
bool is_comment = false;
bool is_color = false;

// Declarações de Protótipos
string getString(char*);
string getVariable(char*);
string replace_all(string, string, string);
int processOperationConst(string, int, int);
void StoreTypeVars(char*);
void parsingVariables(char*);
bool importCommand(string);
string execFunction(string);
bool Interpret_Commands(FILE*);

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
               // cout << "TYPE:" << assembly["type_vars"][var] << ", VAR: " << var << endl;
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
                    for(; lineImport[i] != '\n'; i++){
                        if(lineImport[i] == '[')
                            reading_func = true;

                        if(lineImport[i] == ']')
                            reading_func = false;

                        if(lineImport[i] == ',' && !reading_func)
                            quant_params++;
                    }
                    ++quant_params;
                    i = j;

                    stringstream param;
                    bool is_var = false;

                    for(; lineImport[i] != '\n'; i++){
                            
                        if(lineImport[i] == '@' && !reading_func)
                        {
                            is_var = true;
                            ++i;
                        }
                            

                        if((lineImport[i] != ',' && lineImport[i] != ' ' && lineImport[i] != '\n') || reading_func)
                        {
                            param << lineImport[i];
                            if(lineImport[i] == '[')
                                reading_func = true;

                            if(lineImport[i] == ']')
                                reading_func = false;
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
                                                    return false;
                                                }
                                            }
                                        }
                                        assembly["pushes_call"][pushes++] = push_params.str();
                                        //assembly["text_funcs"][text_funcs++] = push_params.str();
                                    }
                                }else{
                                    // verificar se é uma função
                                    if(isNumber(param.str()))
                                        push_params << "\tpush " << param.str();
                                    else{
                                        // capturar função
                                        push_params << execFunction(param.str());
                                        if(push_params.str() == "$!"){
                                            cout << "A funcao '" << param.str() << "' nao foi declarada!" << endl;
                                            return false;
                                        }
                                        if(push_params.str() == "$$"){
                                            return false;
                                        }
                                        
                                        push_params << "\tpush eax" << endl;
                                        reading_func = false;
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
                                                    return false;
                                                }
                                            }
                                        }
                                        assembly["pushes_call"][pushes++] = push_params.str();
                                        //assembly["text_funcs"][text_funcs++] = push_params.str();
                                    }
                                }else{
                                    // verificar se é uma função
                                    if(isNumber(param.str()))
                                        push_params << "\tpush " << param.str();
                                    else{
                                        // capturar função
                                        push_params << execFunction(param.str());
                                        if(push_params.str() == "$!"){
                                            cout << "A funcao '" << param.str() << "' nao foi declarada!" << endl;
                                            return false;
                                        }
                                        
                                        push_params << "\tpush eax" << endl;
                                        reading_func = false;
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

    assembly["pushes_call"].clear();    

    return true; 
}

string execFunction(string name){
    stringstream push_name;
    stringstream push_params;

    int i = 0;
    bool is_variable = false;
    bool is_plax_string = false;
    bool no_params = true;
    while(name[i] == ' ' || name[i] == 0x09) i++;

    for(; name[i] != '['; i++)
        push_name << name[i];

    if(assembly["local_funcs"][push_name.str()].is_null())
        return "$!";

    ++i;

    stringstream name_params;
    for(; name[i] != ']'; i++){
        
        while((name[i] == ' ' || name[i] == 0x09) && !is_plax_string) i++;

        no_params = (name[i] == ']') ? true : false;
        if(name[i] == ']')
            break;

        if(name[i] == '@'){
            is_variable = true;
            ++i;
        }

        if(name[i] == '\''){
            if(!is_plax_string)
                ++i;

            is_plax_string = true;
        }

        if(name[i] != ','){
            if(name[i] != '\'')
                name_params << name[i];
        }else{
            // Processar parâmetros do meio     
            string name_var = name_params.str();
            string typevar = "NO";
            if(!assembly["type_vars"][name_var].is_null())
                typevar = assembly["type_vars"][name_var];

            if(is_variable){
                if(!is_func){                
                    if(typevar == "CONST INT" || typevar == "CONST FLOAT" || typevar == "INT" || typevar == "FLOAT"){
                        push_params << "\tpush DWORD[" << name_var << "]";
                    }else{
                        if(typevar == "CONST STRING"){
                            push_params << "\tpush " << name_var;
                        }else{
                            push_params << "\tpush DWORD[_" << name_var << "_]";
                        }
                    }
                    
                }else{
                    if(!assembly["local_funcs"][func_name][name_var].is_null()){
                        string params = assembly["local_funcs"][func_name][name_var];
                        push_params << "\tpush " << params;
                    }else{
                        if(!assembly["local_names"][func_name][name_var].is_null()){
                            string stackname = assembly["local_names"][func_name][name_var];
                            push_params << "\tpush " << stackname;
                        }else{
                            if(!assembly["vars"][name_var].is_null()){
                                if(typevar == "CONST INT" || typevar == "CONST FLOAT" || typevar == "INT" || typevar == "FLOAT"){
                                    push_params << "\tpush DWORD[" << name_var << "]";
                                }else{
                                    if(typevar == "CONST STRING"){
                                        push_params << "\tpush " << name_var;
                                    }else{
                                        push_params << "\tpush DWORD[_" << name_var << "_]";
                                    }
                                }
                            }else{
                                cout << "Erro: A variavel " << name_var << " nao existe!" << endl;
                                return "$$";
                            }
                        }
                    }
                    
                }
                is_variable = false;
            }else{
                if(is_plax_string){
                    stringstream var_conc;
                    name_var = (name_var.find("\\n") != -1) ? replace_all(name_var, "\\n", "',13,10,'") : name_var;
                    name_var = (name_var.find("\\t") != -1) ? replace_all(name_var, "\\t", "',9,'") : name_var;
                    name_var = (name_var.find("\\v") != -1) ? replace_all(name_var, "\\v", "',11,'") : name_var;
                    if(name_var.find("\\x") != -1){
                        string strColor = name_var;
                        int index_start = strColor.find("\\x")+2;
                        strColor = strColor.substr(index_start, 2);

                        stringstream color_inst;
                        color_inst << "\tpush 0x" << strColor << endl;
                        color_inst << "\tpush DWORD[_stdout_]" << endl;
                        color_inst << "\tcall SetConsoleTextAttribute" << endl;
                        color_inst << "\tadd esp, 8" << endl;

                        if(!is_func)
                            assembly["text"][text_index++] = color_inst.str();
                        else
                            assembly["text_funcs"][text_funcs++] = color_inst.str(); 

                        stringstream colorconc;
                        colorconc << "\\x" << strColor;
                        string colorStr = colorconc.str();
                        name_var = replace_all(name_var, colorStr, "");
                    }

                    var_conc << "\t__" << push_name.str() << "_st__" << data_index << " db '" << name_var << "',0";
                    assembly["data"][data_index++] = var_conc.str();
                    var_conc.str("");
                    push_params << "\tpush __" << push_name.str() << "_st__" << data_index-1;
                    
                    is_plax_string = false;
                }else{
                    if(isNumber(name_var)){
                        push_params << "\tpush " << name_var;               
                    }else{
                        /*
                        push_params << execFunction(name_var);
                        if(push_params.str() == "$!"){
                            cout << "A funcao '" << name_var << "' nao foi declarada!" << endl;
                            return "$!";
                        }
                        reading_func = false;
                        */
                        push_params << "\tpush eax" << endl;
                    }
                    
                }
            }

            assembly["pushes_call2"][pushes2++] = push_params.str();
            name_params.str("");
            push_params.str("");

        }
        
    }

    stringstream push_call;

    if(no_params){
        push_call << "\tcall " << push_name.str() << endl;
    }else{
        // Processar parâmetro do final
        string name_var = name_params.str();
        string typevar = "NO";
        if(!assembly["type_vars"][name_var].is_null())
            typevar = assembly["type_vars"][name_var];


        if(is_variable){
            if(!is_func){                
                if(typevar == "CONST INT" || typevar == "CONST FLOAT" || typevar == "INT" || typevar == "FLOAT"){
                    push_params << "\tpush DWORD[" << name_var << "]";
                }else{
                    if(typevar == "CONST STRING"){
                        push_params << "\tpush " << name_var;
                    }else{
                        push_params << "\tpush DWORD[_" << name_var << "_]";
                    }
                }
                assembly["pushes_call2"][pushes2++] = push_params.str();
            }else{
                if(!assembly["local_funcs"][func_name][name_var].is_null()){
                    string params = assembly["local_funcs"][func_name][name_var];
                    push_params << "\tpush " << params;
                }else{
                    if(!assembly["local_names"][func_name][name_var].is_null()){
                        string stackname = assembly["local_names"][func_name][name_var];
                        push_params << "\tpush " << stackname;
                    }else{
                        if(!assembly["vars"][name_var].is_null()){
                            if(typevar == "CONST INT" || typevar == "CONST FLOAT" || typevar == "INT" || typevar == "FLOAT"){
                                push_params << "\tpush DWORD[" << name_var << "]";
                            }else{
                                if(typevar == "CONST STRING"){
                                    push_params << "\tpush " << name_var;
                                }else{
                                    push_params << "\tpush DWORD[_" << name_var << "_]";
                                }
                            }
                        }else{
                            cout << "Erro: A variavel " << name_var << " nao existe!" << endl;
                            return "$$";
                        }
                    }
                }
                assembly["pushes_call2"][pushes2++] = push_params.str();
            }
            is_variable = false;
        }else{
            if(is_plax_string){
                stringstream var_conc;
                name_var = (name_var.find("\\n") != -1) ? replace_all(name_var, "\\n", "',13,10,'") : name_var;
                name_var = (name_var.find("\\t") != -1) ? replace_all(name_var, "\\t", "',9,'") : name_var;
                name_var = (name_var.find("\\v") != -1) ? replace_all(name_var, "\\v", "',11,'") : name_var;
                if(name_var.find("\\x") != -1){
                    string strColor = name_var;
                    int index_start = strColor.find("\\x")+2;
                    strColor = strColor.substr(index_start, 2);

                    stringstream color_inst;
                    color_inst << "\tpush 0x" << strColor << endl;
                    color_inst << "\tpush DWORD[_stdout_]" << endl;
                    color_inst << "\tcall SetConsoleTextAttribute" << endl;
                    color_inst << "\tadd esp, 8" << endl;

                    if(!is_func)
                        assembly["text"][text_index++] = color_inst.str();
                    else
                        assembly["text_funcs"][text_funcs++] = color_inst.str();

                    stringstream colorconc;
                    colorconc << "\\x" << strColor;
                    string colorStr = colorconc.str();
                    name_var = replace_all(name_var, colorStr, "");
                }
                var_conc << "\t__" << push_name.str() << "_st__" << data_index << " db '" << name_var << "',0";
                assembly["data"][data_index++] = var_conc.str();
                var_conc.str("");
                push_params << "\tpush __" << push_name.str() << "_st__" << data_index-1;
                
                assembly["pushes_call2"][pushes2++] = push_params.str();
                is_plax_string = false;
            }else{
                if(isNumber(name_var)){
                    push_params << "\tpush " << name_var;               
                }else{
                    /*
                    push_params << execFunction(name_var);
                    if(push_params.str() == "$!"){
                        cout << "A funcao '" << name_var << "' nao foi declarada!" << endl;
                        return "$!";
                    }
                    reading_func = false;
                    */
                   push_params << "\tpush eax" << endl;
                }
                assembly["pushes_call2"][pushes2++] = push_params.str();
            }
        }

        name_params.str("");
        push_params.str("");

        push_params << "\tcall " << push_name.str();
        push_call << endl;

        for(int x = assembly["pushes_call2"].size()-1; x >= 0 ; x--){
            string push_call_str = assembly["pushes_call2"][x];
            push_call << push_call_str << endl;
            if(x == 0){
                push_call << push_params.str() << endl;
                push_call << "\tadd esp, " << assembly["pushes_call2"].size() * 4 << endl;
            }
        }
    }

    assembly["pushes_call2"].clear();
    pushes2 = 0;

    return push_call.str();
}

bool Interpret_Commands(FILE *file_read){
    char line[1024];
    while((fgets(line, sizeof(line), file_read)) != NULL){

        if(contains(line, "<<<") || contains(line, ">>>")){

            string lineComm = toString(line);
            int j = 0;
            while(lineComm[j] == ' ' || lineComm[j] == 0x09) j++;

            if(lineComm[j] == '<' && lineComm[j+1] == '<' && lineComm[j+1] == '<'){
                is_comment = true;
                fgets(line, sizeof(line), file_read);
            }
            for(; lineComm[j] != '\n'; j++){
                if(lineComm[j] == '>' && lineComm[j+1] == '>' && lineComm[j+1] == '>'){
                    is_comment = false;
                    //fgets(line, sizeof(line), file_read);
                }
            }
            if(contains(line, ">>>") && lineComm.find("<<<") == -1)
                fgets(line, sizeof(line), file_read);
        }

        if(contains(line, "@") && contains(line, ":") && !contains(line, "local") && !is_comment){
                        
            str = getString(line);
            var = getVariable(line);
            StoreTypeVars(line);
            
            bool exist_var = (!assembly["vars"][var].is_null()) ? true : false;
            if(!is_func)
                assembly["vars"][var] = str.c_str();

            parsingVariables(line);

            if(is_constant){
                if(isNumber(str)){
                    if(!assembly["type_vars"][var].is_null())
                        type = assembly["type_vars"][var];
                    //cout << type << ": " << str << ", VAR: @" << var << endl;
                    stringstream var_conc;
                    if(!is_func){
                        var_conc << "\t" << var << " dd " << str;
                        assembly["rodata"][rodata_index++] = var_conc.str();
                    }else{
                        if(!assembly["local_funcs"][func_name][var].is_null()){
                            string param = assembly["local_funcs"][func_name][var];
                            var_conc << "\tmov " << param << ", " << str;
                        }else{
                            if(assembly["vars"][var].is_null()){
                                local_offsets += 4;
                                stringstream stacklocal;
                                stacklocal << "DWORD[ebp - " << local_offsets << "]";
                                var_conc << "\tmov " << stacklocal.str() << ", " << str;
                                assembly["local_names"][func_name][var] = stacklocal.str();
                            }else{
                                var_conc << "\tmov DWORD[" << var << "], " << str;
                            }
                        }
                        assembly["text_funcs"][text_funcs++] = var_conc.str();
                    }
                }else{
                    if(!assembly["type_vars"][var].is_null())
                        type = assembly["type_vars"][var];
                    //cout << type << ": " << "\"" << str << "\", VAR: @" << var << endl;
                    stringstream var_conc;
                    
                    bool is_plax_bool = false;

                    if(type == "CONST BOOL"){
                        is_plax_bool = true;
                        if(str == "true")
                            var_conc << "\t" << var << " db 1";
                        else
                            var_conc << "\t" << var << " db 0";
                    }else{
                        str = (str.find("\\n") != -1) ? replace_all(str, "\\n", "',13,10,'") : str;
                        str = (str.find("\\t") != -1) ? replace_all(str, "\\t", "',9,'") : str;
                        str = (str.find("\\v") != -1) ? replace_all(str, "\\v", "',11,'") : str;
                        if(str.find("\\x") != -1){
                            string strColor = str;
                            int index_start = strColor.find("\\x")+2;
                            strColor = strColor.substr(index_start, 2);

                            stringstream color_inst;
                            color_inst << "\tpush 0x" << strColor << endl;
                            color_inst << "\tpush DWORD[_stdout_]" << endl;
                            color_inst << "\tcall SetConsoleTextAttribute" << endl;
                            color_inst << "\tadd esp, 8" << endl;

                            if(!is_func)
                                assembly["text"][text_index++] = color_inst.str();
                            else
                                assembly["text_funcs"][text_funcs++] = color_inst.str();

                            stringstream colorconc;
                            colorconc << "\\x" << strColor;
                            string colorStr = colorconc.str();
                            str = replace_all(str, colorStr, "");
                        }
                        var_conc << "\t" << var << " db '" << str << "',0";
                    }

                    assembly["rodata"][rodata_index++] = var_conc.str();
                    var_conc.str("");

                    if(!is_plax_bool){
                        var_conc << "\t" << var << ".size equ $ - " << var;
                        assembly["rodata"][rodata_index++] = var_conc.str();
                    }
                }

            }else{
                stringstream var_conc;
            
                if(!exist_var){
                    if(isNumber(str)){
                        if(!assembly["type_vars"][var].is_null())
                                type = assembly["type_vars"][var];
                            
                            //cout << type << ": " << "\"" << str << "\", VAR: @" << var << endl;
                        if(!is_func){
                            var_conc << "\t" << var << " dd " << str;
                            assembly["data"][i++] = var_conc.str();
                        }else{
                            if(!assembly["local_funcs"][func_name][var].is_null()){
                                string param = assembly["local_funcs"][func_name][var];
                                var_conc << "\tmov " << param << ", " << str;
                            }else{
                                local_offsets += 4;
                                stringstream stacklocal;
                                stacklocal << "DWORD[ebp - " << local_offsets << "]";
                                var_conc << "\tmov " << stacklocal.str() << ", " << str;
                                assembly["local_names"][func_name][var] = stacklocal.str();
                            }
                            assembly["text_funcs"][text_funcs++] = var_conc.str();
                        }

                    }else{
                        if(!assembly["type_vars"][var].is_null())
                                type = assembly["type_vars"][var];
                            
                            //cout << type << ": " << "\"" << str << "\", VAR: @" << var << endl;

                        if(str.find("import") != -1){
                            //cout << str << endl;
                            stringstream strbreak;
                            strbreak << str << "\n";
                            bool success = importCommand(strbreak.str());
                            if(!success)
                                return 0;

                            var_conc << "\t_" << var << "_" << " resd 1 ";
                            assembly["bss"][bss++] = var_conc.str();
                            var_conc.str("");
                            var_conc << "\tmov DWORD[_" << var << "_], eax";

                            if(!is_func)
                                assembly["text"][text_index++] = var_conc.str();
                            else
                                assembly["text_funcs"][text_funcs++] = var_conc.str();

                            assembly["realloc"][var] = false;
                        }else{
                            if(str == "NULL")
                                var_conc << "\t" << var << " db 0";
                            else{
                                str = (str.find("\\n") != -1) ? replace_all(str, "\\n", "',13,10,'") : str;
                                str = (str.find("\\t") != -1) ? replace_all(str, "\\t", "',9,'") : str;
                                str = (str.find("\\v") != -1) ? replace_all(str, "\\v", "',11,'") : str;
                                if(str.find("\\x") != -1){
                                    string strColor = str;
                                    int index_start = strColor.find("\\x")+2;
                                    strColor = strColor.substr(index_start, 2);

                                    stringstream color_inst;
                                    color_inst << "\tpush 0x" << strColor << endl;
                                    color_inst << "\tpush DWORD[_stdout_]" << endl;
                                    color_inst << "\tcall SetConsoleTextAttribute" << endl;
                                    color_inst << "\tadd esp, 8" << endl;

                                    if(!is_func)
                                        assembly["text"][text_index++] = color_inst.str();
                                    else
                                        assembly["text_funcs"][text_funcs++] = color_inst.str();

                                    stringstream colorconc;
                                    colorconc << "\\x" << strColor;
                                    string colorStr = colorconc.str();
                                    str = replace_all(str, colorStr, "");
                                }
                                var_conc << "\t" << var << " db '" << str << "',0";
                            }
                            assembly["data"][i] = var_conc.str();

                            var_conc.str("");

                            var_conc << "\t_" << var << "_" << " resd 1 ";
                            assembly["bss"][bss++] = var_conc.str();
                            assembly["realloc"][var] = false;
                        }
                    }
                }else{
                    
                    if(isNumber(str)){
                        if(!assembly["type_vars"][var].is_null())
                                type = assembly["type_vars"][var];
                            
                            //cout << type << ": " << "\"" << str << "\", VAR: @" << var << endl;
                        if(!is_func){
                            var_conc << "\tmov DWORD[" << var << "], " << str;
                            assembly["text"][text_index++] = var_conc.str();
                        }else{
                            string globalword = toString(line);
                            int firstpos = globalword.find("global");
                            int seconpos = globalword.find("@");
                            if(contains(line, "global") && (firstpos < seconpos)){
                                    var_conc << "\tmov DWORD[" << var << "], " << str;
                            }else{
                                if(!assembly["local_funcs"][func_name][var].is_null()){
                                    string param = assembly["local_funcs"][func_name][var];
                                    var_conc << "\tmov " << param << ", " << str;
                                }else{
                                    var_conc << "\tmov DWORD[" << var << "], " << str;
                                }
                            }
                            assembly["text_funcs"][text_funcs++] = var_conc.str();
                        }
                    }else{
                        if(!assembly["type_vars"][var].is_null())
                                type = assembly["type_vars"][var];
                            
                            //cout << type << ": " << "\"" << str << "\", VAR: @" << var << endl;

                        var_conc << "_" << var << "_";

                        stringstream push_inst;
                        if(!is_func){
                            if(!assembly["realloc"][var]){
                                push_inst << "\tpush " << var << "__" << i << ".size";
                                assembly["text"][text_index++] = push_inst.str();
                                assembly["text"][text_index++] = "\tcall malloc";
                                assembly["text"][text_index++] = "\tadd esp, 4";
                            }else{
                                push_inst << "\tpush " << var << "__" << i << ".size";
                                assembly["text"][text_index++] = push_inst.str();
                                push_inst.str("");
                                push_inst << "\tpush DWORD[_" << var << "_]";
                                assembly["text"][text_index++] = push_inst.str();
                                assembly["text"][text_index++] = "\tcall realloc";
                                assembly["text"][text_index++] = "\tadd esp, 8";
                            }

                            stringstream addr_inst;
                            addr_inst << "\tmov [" << var_conc.str() << "], eax";
                            var_conc.str("");
                            str = (str.find("\\n") != -1) ? replace_all(str, "\\n", "',13,10,'") : str;
                            str = (str.find("\\t") != -1) ? replace_all(str, "\\t", "',9,'") : str;
                            str = (str.find("\\v") != -1) ? replace_all(str, "\\v", "',11,'") : str;

                            stringstream color_inst;
                            bool has_color = false;

                             if(str.find("\\x") != -1){
                                string strColor = str;
                                int index_start = strColor.find("\\x")+2;
                                strColor = strColor.substr(index_start, 2);

                                color_inst << "\tpush 0x" << strColor << endl;
                                color_inst << "\tpush DWORD[_stdout_]" << endl;
                                color_inst << "\tcall SetConsoleTextAttribute" << endl;
                                color_inst << "\tadd esp, 8" << endl;

                                stringstream colorconc;
                                colorconc << "\\x" << strColor;
                                string colorStr = colorconc.str();
                                str = replace_all(str, colorStr, "");

                                has_color = true;
                            }
                            var_conc << "\t" << var << "__" << i << " db '" << str << "',0";

                            assembly["data"][i] = var_conc.str();
                            var_conc.str("");

                            var_conc << "\t" << var << "__" << i << ".size equ $ - " << var << "__" << i;
                            assembly["data"][++i] = var_conc.str();
                            var_conc.str("");

                            var_conc << "\tmov esi, " << var << "__" << i-1; 
                            assembly["text"][text_index++] = addr_inst.str();
                            assembly["text"][text_index++] = "\tmov edi, eax";
                            assembly["text"][text_index++] = var_conc.str();
                            var_conc.str("");

                            var_conc << "\tmov ecx, " << var << "__" << i-1 << ".size";
                            assembly["text"][text_index++] = var_conc.str();
                            assembly["text"][text_index++] = "\trep movsb";

                            if(has_color){
                                if(!is_func)
                                    assembly["text"][text_index++] = color_inst.str();
                                else
                                    assembly["text_funcs"][text_funcs++] = color_inst.str();
                            }
                        }else{
                            if(!assembly["realloc"][var]){
                                push_inst << "\tpush " << var << "__" << i << ".size";
                                assembly["text_funcs"][text_funcs++] = push_inst.str();
                                assembly["text_funcs"][text_funcs++] = "\tcall malloc";
                                assembly["text_funcs"][text_funcs++] = "\tadd esp, 4";
                            }else{
                                push_inst << "\tpush " << var << "__" << i << ".size";
                                assembly["text_funcs"][text_funcs++] = push_inst.str();
                                push_inst.str("");
                                push_inst << "\tpush DWORD[_" << var << "_]";
                                assembly["text_funcs"][text_funcs++] = push_inst.str();
                                assembly["text_funcs"][text_funcs++] = "\tcall realloc";
                                assembly["text_funcs"][text_funcs++] = "\tadd esp, 8";
                            }

                            stringstream addr_inst;
                            addr_inst << "\tmov [" << var_conc.str() << "], eax";
                            var_conc.str("");
                            str = (str.find("\\n") != -1) ? replace_all(str, "\\n", "',13,10,'") : str;
                            str = (str.find("\\t") != -1) ? replace_all(str, "\\t", "',9,'") : str;
                            str = (str.find("\\v") != -1) ? replace_all(str, "\\v", "',11,'") : str;
                            stringstream color_inst;
                            bool has_color = false;
                            if(str.find("\\x") != -1){
                                string strColor = str;
                                int index_start = strColor.find("\\x")+2;
                                strColor = strColor.substr(index_start, 2);

                                color_inst << "\tpush 0x" << strColor << endl;
                                color_inst << "\tpush DWORD[_stdout_]" << endl;
                                color_inst << "\tcall SetConsoleTextAttribute" << endl;
                                color_inst << "\tadd esp, 8" << endl;

                                stringstream colorconc;
                                colorconc << "\\x" << strColor;
                                string colorStr = colorconc.str();
                                str = replace_all(str, colorStr, "");

                                has_color = true;
                            }
                            var_conc << "\t" << var << "__" << i << " db '" << str << "',0";

                            assembly["data"][i] = var_conc.str();
                            var_conc.str("");

                            var_conc << "\t" << var << "__" << i << ".size equ $ - " << var << "__" << i;
                            assembly["data"][++i] = var_conc.str();
                            var_conc.str("");

                            var_conc << "\tmov esi, " << var << "__" << i-1; 
                            assembly["text_funcs"][text_funcs++] = addr_inst.str();
                            assembly["text_funcs"][text_funcs++] = "\tmov edi, eax";
                            assembly["text_funcs"][text_funcs++] = var_conc.str();
                            var_conc.str("");

                            var_conc << "\tmov ecx, " << var << "__" << i-1 << ".size";
                            assembly["text_funcs"][text_funcs++] = var_conc.str();
                            assembly["text_funcs"][text_funcs++] = "\trep movsb";

                            if(has_color){
                                if(!is_func)
                                    assembly["text"][text_index++] = color_inst.str();
                                else
                                    assembly["text_funcs"][text_funcs++] = color_inst.str();
                            }
                        }

                        i++;
                    
                        assembly["realloc"][var] = true;
                    }
                }
            }

            data_index = i;
        }

        if(is_func && !is_comment){
            if(contains(line, ")") && index_symbol == 0){
                is_func = false;
                //cout << "FUNC END " << index_symbol << " => " << line << endl;
                assembly["text_funcs"][text_funcs++] = "\tmov esp, ebp";
                assembly["text_funcs"][text_funcs++] = "\tpop ebp";
                assembly["text_funcs"][text_funcs++] = "\tret";
            }else{
                if(contains(line, ")")){
                    //cout << "CONDITION " << index_symbol << " END:" << line << endl;
                    index_symbol -= 1;
                }
            }

            if(contains(line, "(")){
                    index_symbol += 1;
                    //cout << "CONDITION " << index_symbol << ": " << line << endl;
            }

        }

        if(contains(line, "func") && !is_comment){
            
            local_offsets = 0;
            is_func = true;
            string nameFunc = toString(line);

            int index_end = (contains(line, "@")) ? nameFunc.find("@") : nameFunc.find("(");
            int index_start = nameFunc.find("func")+5;

            index_end = index_end - (index_start + 1);
            nameFunc = nameFunc.substr(index_start, index_end);
            nameFunc = toString(EraseSpace((char *) nameFunc.c_str())); 

            func_name = nameFunc;
            // nameFunc contém o nome da função
            // Insere label da função no array de funções Assembly
            stringstream labels;
            labels << nameFunc << ":";
            assembly["text_funcs"][text_funcs++] = labels.str().c_str();
            assembly["text_funcs"][text_funcs++] = "\tpush ebp";
            assembly["text_funcs"][text_funcs++] = "\tmov ebp, esp";

            // Filtra e Armazena parâmetros da função 'nameFunc'
            if(contains(line, "@")){
                int arroba_ind = toString(line).find("@");
                int locals_ind = 0;
                int ebp_add = 4;
                stringstream local_name;
                stringstream ebp_value;
                for(int i = arroba_ind; i < toString(line).length(); i++){

                    if(line[i] == '@') ++i;

                    if(line[i] != ',' && line[i] != ' ' && line[i] != '(')
                        local_name << line[i];

                    if(line[i] == ',' || line[i] == '('){
                        ebp_add += 4;
                        ebp_value << "DWORD[ebp + " << ebp_add << "]";
                        assembly["local_funcs"][nameFunc][local_name.str()] = ebp_value.str();
                        local_name.str("");
                        ebp_value.str("");
                        
                        if(line[i] == '(')
                            break;
                    }
                    
                }
            }

            // cout << "FUNC BEGIN " << index_symbol << " => " << line << endl;

        }

        if(contains(line, "import") && !is_comment){
            bool success = importCommand(toString(line));
            if(!success)
                return false;
        }

        if(contains(line, "return") && !is_comment){
            string lineStr = toString(line);

            int index_end = lineStr.length();
            int index_start = lineStr.find("return")+6;

            index_end = index_end - (index_start + 1);

            lineStr = lineStr.substr(index_start, index_end);

            if(lineStr.find("import") != -1){
                stringstream strbreak;
                strbreak << lineStr << "\n";
                bool success = importCommand(strbreak.str());
                if(!success)
                    return false;

                strbreak.str("");
                strbreak << "\tmov DWORD[_return_funcs_], eax";

                if(!is_func)
                    assembly["text"][text_index++] = strbreak.str();
                else
                    assembly["text_funcs"][text_funcs++] = strbreak.str();
            }

        }

        if(contains(line, "[") && !contains(line, "import") && !contains(line, "const") && !is_comment){
            stringstream getcode;
            reading_func = true;
            getcode << execFunction(toString(line));
            if(getcode.str() == "$!"){
                cout << "A funcao '" << toString(line) << "' nao foi declarada!" << endl;
                return false;
            }
            reading_func = false;

            if(!is_func)
                assembly["text"][text_index++] = getcode.str();
            else
                assembly["text_funcs"][text_funcs++] = getcode.str();
        }

        if(contains(line, "use") && contains(line, "of") && !is_comment){
            string lineUse = toString(line);
            int j = 0;
            while(lineUse[j] == ' ' || lineUse[j] == 0x09) j++;

            if(lineUse[j] == 'u' && lineUse[j+1] == 's' && lineUse[j+2] == 'e'){
                int index_end = lineUse.find("of");
                int index_start = lineUse.find("use")+4;

                index_end = index_end - (index_start + 1);

                string lineOf = lineUse;
                lineUse = lineUse.substr(index_start, index_end);
                lineUse = toString(EraseSpace((char *) lineUse.c_str()));

                index_end = lineOf.length();
                index_start = lineOf.find("of")+3;

                index_end = index_end - (index_start + 1);
                lineOf = lineOf.substr(index_start, index_end);
                lineOf = toString(EraseSpace((char *) lineOf.c_str()));
                
                stringstream of_file;
                if(lineOf.find(".plax") == -1)
                    of_file << lineOf << ".plax";

                lineOf = of_file.str();

                lineOf.replace(lineOf.find("<"), 1, "");
                lineOf.replace(lineOf.find(">"), 1, "");

                FILE *file_inc;

                if((file_inc = fopen((char*) lineOf.c_str(), "r")) == NULL){
                    cout << "Erro: O arquivo '" << lineOf << "' nao existe para ser usado!" << endl;
                    return false;
                }

                if(lineOf == "*" || lineOf == "*.*")
                    use_all = true;
                
                bool finish = Interpret_Commands(file_inc);
                if(!finish)
                    return false;

            }

        }

    }

    return true;
}

#endif