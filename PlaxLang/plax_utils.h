#ifndef __PLAX_UTILS_H__
#define __PLAX_UTILS_H__

#include "c_utils.h"

// Variáveis e Objetos
json assembly;
bool is_constant;
string str;
string var;
string func_name;
int local_offsets = 0;

// Declarações de Protótipos
string getString(char*);
string getVariable(char*);
int processOperationConst(string, int, int);
void StoreTypeVars(char*);
void parsingVariables(char*);

string getString(char *line)
{

	string lineStr = toString(line);

    int index_end = lineStr.find("(");
    int index_start = lineStr.find(":")+1;

    index_end = index_end - (index_start + 1);

    lineStr = lineStr.substr(index_start, index_end);
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
        }
    }
}

void parsingVariables(char *line){
    stringstream var_name;
    bool variable = false;
    char op;
    int count_op = 0;
    bool const_finded = true;
    bool has_variable = false;

    if(!contains(line, "const[") && !isNumber(str)){
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
                                assembly["type_vars"][var] = "CONST INT";
                        }else{
                            if(isNumber(value) && value.find(".") != -1)
                                assembly["type_vars"][var] = "FLOAT";
                            else
                                assembly["type_vars"][var] = "INT";
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

#endif