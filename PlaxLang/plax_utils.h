#ifndef __PLAX_UTILS_H__
#define __PLAX_UTILS_H__

#include "c_utils.h"

// Objetos do compilador
json assembly;
FILE* plaxfile;
// Variáveis String
// ----------------------------------------
string str;
string var;
string func_name;
string type;
string tokenfunc;
string token;

// ----------------------------------------

// Variáveis inteiros
// ----------------------------------------
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
int line_index = 0;
int line_number = 1;
int line_temp 	= 0;
int token_len   = 0;
int token_ind 	= 0;
int typetok 	= 0;
int subtype 	= 0;
// ----------------------------------------


// Variáveis booleanas e estados
// ----------------------------------------
bool is_func = false;
bool reading_func = false;
bool use_all = false;
bool is_comment = false;
bool is_color = false;
bool is_constant = false;

// Estados do Parser ----------------------
bool is_quote_open = false;
bool is_use_command = false;
bool is_comment_open = false;
bool is_attrib = false;
bool is_cond_command = false;
bool is_func_command = false;
bool is_call_function = false;
bool is_call_command = false;
bool is_intr_command = false;
bool is_error = false;
// ----------------------------------------

// Declarações de Protótipos
// ----------------------------------------
int get_token_type(string);
int get_token_subtype(int, string);
int get_token(int, string);
int get_subtypes(string);
int iterate_token(string, int, int, const char*[]);
void debug_token_type(int, int, int, string);
void debug_msg(string);

bool get_state(int, int);
bool end_state(int);

void Lexical_Analyzer();
void Tokenize();
void Parser();

void (*func_ptr)();
void variable_proc();
void config_proc();
void functions_proc();
void comment_proc();
void declaration_proc();
void conditional_proc();
void repetition_proc();
void call_proc();
void intr_proc();
void arithmetic_proc();
void attribution_proc();
void standard();

string getString(char*);
string getstring(string, string, int);
string getVariable(char*);
string replace_all(string, string, string);
string execFunction(string);

int processOperationConst(string, int, int);
void process_function_params(char*);
bool process_variables_attrib(char*);
bool process_func_command(char*);
bool process_return_command(char*);
bool process_functions_call(char*);
bool process_use_command(char*);
bool process_import_command(string);
bool Interpret_Commands(FILE*);
void StoreTypeVars(char*);
void parsingVariables(char*);
void setTypeVars(string, int, int, int);
void parsingConstant(char*);

// ----------------------------------------

// Vetores de chars
char line[1024];
const string error_comment  = "Error Syntax: It's missing comment close symbol! In line -> ";
const string error_variable = "Error Syntax: Alone variable(s) no keyword or assignment associated! In line -> ";
const string error_math = "Error Syntax: cannot do math operations with strings! In line -> ";

// Estruturas e Arrays
// ----------------------------------------
#define OPERATORS_SIZE 	19
const char* operators[] = {
	"->",
    "*",
    "/",
    "+",
    "-",
	"%",
	"=",
	">=",
	"<=",
	"<>",
	">",
	"<",
	"~",
    "^",
    "&",
    "|",
	"??",
	"?",
	":",
};

#define PREFIX_SIZE 2
const char* prefix[] = {
	"@",
	"$"
};

#define DELIMITER_SIZE 14
const char* delimiter[] = {
	"<<<",
	">>>",
	"<",
	">",
	"[",
	"]",
	"(",
	")",
	"{",
	"}",
	"'",
	"\"",
	",",
	"."
};

#define DIFF_SYMBOLS 2
const char* diff_symbols[] = {
	"<",
	">",
};

#define SPECIAL_SIZE 5
const char* special[] = {
	"!",
	"#",
	"§",
	";",
	"¨"
};

#define ESCAPE_SIZE 4
const char* escape[] = {
	"\\n",
	"\\t",
	"\\v",
	"\\x"
};

#define COMMAND_SIZE 19
const char* command[] = {
	"use",
	"of",
	"func",
	"return",
	"import",
	"intr",
	"call",
	"const",
	"string",
	"int",
	"word",
	"byte",
	"char",
	"bool",
	"float",
	"if",
	"else",
	"while",
	"for"
};

#define VALUES_SIZE 6
const char* values[] = {
	"true",
	"false",
	"yes",
	"no",
	"NaN",
	"null"
};

const char* types[] = {
    "CONST FLOAT",
    "CONST INT",
    "CONST STRING",
    "CONST BOOL",
    "FLOAT",
    "INT",
    "STRING",
    "BOOL",
	"FUNCTION"
};

const char* tokentypes[] = {
	"UNKNOWN",
	"COMMAND",
	"VALUES",
	"PREFIX",
	"DELIMITER",
	"OPERATOR",
	"SPECIAL",
	"ESCAPE",
	"ENDLINE",
	"ENDFILE"
};

const char* subtypes[] = {
	"VARIABLE",
	"CONFIG",
	"FUNCTIONS",
	"DEFINITION",
	"TEXT",
	"NUMBER",
	"ARITHMETIC",
	"RELATIONAL",
	"LOGICAL",
	"ATTRIBUTION",
	"COMMENTS",
	"SCOPE",
	"STRUCTS",
	"SEPARATOR",
	"CONDITIONAL",
	"REPETITION",
	"INCLUDE",
	"DECLARATION",
	"RETURN",
	"PARAMETERS",
	"CONVERSION",
	"EXTERN_INT",
	"EXTERN_CALL",
	"OTHER KEYWORD"
};

// ----------------------------------------

// Enumerators
// ----------------------------------------
enum TYPE_OF_VARS {
  CONST_FLOAT = 0,
  CONST_INT,
  CONST_STRING,
  CONST_BOOL,
  V_FLOAT,
  V_INT,
  V_STRING,
  V_BOOL,
  FUNCTION
};

enum TYPE_TOKEN {
	UNKNOWN = -1,
	COMMAND = 0,
	VALUES,
	PREFIX,
	DELIMITER,
	OPERATOR,
	SPECIAL,
	ESCAPE,
	ENDLINE,
	ENDFILE
};

enum SUB_TYPES {
	VARIABLE = 0,
	CONFIG,
	FUNCTIONS,
	DEFINITION,
	TEXT,
	NUMBER,
	ARITHMETIC,
	RELATIONAL,
	LOGICAL,
	ATTRIBUTION,
	COMMENTS,
	SCOPE,
	STRUCTS,
	SEPARATOR,
	CONDITIONAL,
	REPETITION,
	INCLUDE,
	DECLARATION,
	RETURN,
	PARAMETERS,
	CONVERSION,
	EXTERNINT,
	EXTERNCALL,
	SINGLEQUOTE,
	OTHER
};

#define OTHER_KEYWORD 16

enum SYMBOL_TYPES {
	BRACKET = 4,
	ATTRIB = 17,
	CONFIG_ATT = 18,
	KEYS = 8,
	QUOTE = 10,
	DOUBLEQUOTE = 11
};

const int tokensizes[] = {
	COMMAND_SIZE, VALUES_SIZE, PREFIX_SIZE,  
	DELIMITER_SIZE, OPERATORS_SIZE,
	SPECIAL_SIZE, ESCAPE_SIZE
};

const int tokentype[] = {
	COMMAND, VALUES, PREFIX, 
	DELIMITER, OPERATOR,
	SPECIAL, ESCAPE
};

#define TOKENPOINTER_SIZE 7
const char** tokenpointer[] = {
	command, values, prefix, 
	delimiter, operators, special, 
	escape
};

int* operations[] = {
	(int*)variable_proc, (int*)config_proc, (int*)functions_proc, (int*)standard,
	(int*)standard,(int*)standard,(int*)arithmetic_proc,(int*)standard,(int*)standard,(int*)attribution_proc,(int*)comment_proc,
	(int*)standard,(int*)standard,(int*)standard,(int*)conditional_proc,(int*)repetition_proc,(int*)standard,(int*)declaration_proc,(int*)standard,
	(int*)standard,(int*)standard,(int*)intr_proc,(int*)call_proc,(int*)standard,(int*)standard,(int*)standard,(int*)standard,
	(int*)standard,(int*)standard,(int*)standard,(int*)standard,(int*)standard,(int*)standard,(int*)standard,(int*)standard,
	(int*)standard,(int*)standard,(int*)standard,(int*)standard,
};

void variable_proc(){
	debug_msg("Processing variable token...");
	is_error = false;
	Tokenize();
	
	if(!is_attrib)
		is_attrib = get_state(OPERATOR, ATTRIBUTION);
	
	if(is_attrib){
		// TODO: PARSER DE VALORES E EXPRESSÕES
		debug_msg("Atributing data...");
		is_attrib = end_state(ENDLINE);
		Parser();
	}else if(is_cond_command){
		// TODO: PARSER DE COMPARAÇÕES E EXPRESSÕES
		debug_msg("Conditions reading...");
		
		//is_cond_command = false;
	}else if(is_func_command){
		// TODO: PARSER DE PARÂMETROS DE FUNÇÕES
		debug_msg("Parameters reading...");
		
		//is_func_command = false;
	}else if(is_call_function){
		// TODO: PARSER DE ARGUMENTOS DE FUNÇÕES
		debug_msg("Arguments reading...");
		
		//is_call_function = false;
	}else if(is_call_command){
		// TODO: PARSER DE CALLS EXTERNOS E REGISTRADORES
		debug_msg("Register args reading for CALL");
		
		//is_call_command = false;
	}else if(is_intr_command){
		// TODO: PARSER DE INTERRUPÇÕES E REGISTRADORES
		debug_msg("Register args reading for INTR");
		
		//is_intr_command = false;
	}else{
		cout << error_variable << line_temp << endl;
		is_error = true;
	}
}

void config_proc(){
	debug_msg("Processing config token...");
	is_error = false;
	Tokenize();
}

void functions_proc(){
	debug_msg("Processing functions token...");
	is_error = false;
	Tokenize();
	is_call_function = get_state(DELIMITER, PARAMETERS);
}

void declaration_proc(){
	debug_msg("Processing func token...");
	is_error = false;
	is_func_command = get_state(COMMAND, DECLARATION);
}

void conditional_proc(){
	debug_msg("Processing IF token...");
	is_error = false;
	is_cond_command = get_state(COMMAND, CONDITIONAL);
}

void repetition_proc(){
	debug_msg("Processing WHILE token...");
	is_error = false;
	is_cond_command = get_state(COMMAND, REPETITION);
}

// -----------------------------------------------------------------------------
// Funções pendentes para subtipos
void call_proc(){
	debug_msg("Processing call token...");
	is_error = false;
	is_call_command = get_state(COMMAND, EXTERNCALL);
}

void intr_proc(){
	debug_msg("Processing intr token...");
	is_error = false;
	is_intr_command = get_state(COMMAND, EXTERNINT);
}

void arithmetic_proc(){
	debug_msg("Processing math token...");
	is_error = false;
	Tokenize();
	Parser();
	is_error = (subtype == SINGLEQUOTE);
	if (is_error)
		cout << error_math << line_temp << endl;
}

void attribution_proc(){
	debug_msg("Processing attrib token...");
	is_error = false;
	Tokenize();
	Parser();
}
// -----------------------------------------------------------------------------

void comment_proc(){
	is_error = false;
	while(typetok != ENDFILE){
		Tokenize();
		if(token == delimiter[1] && subtype == COMMENTS)
			return;
		if(typetok == ENDLINE || line[line_index] == 0)
			if(fgets(line, sizeof(line), plaxfile) == NULL)
				typetok = ENDFILE;
	}
	cout << error_comment << line_temp << endl;
	is_error = true;
}

void standard(){
	//cout << "Standard test..." << endl;
}
// ----------------------------------------

string getString(char *line)
{

	string lineStr = toString(line);

    lineStr = getstring(lineStr, ":", lineStr.length());
	int i = 0;
	for(; lineStr[i] == ' ' || lineStr[i] == 0x09; i++);

	// contains(line, "'")
    if(lineStr[i] == '\'' || lineStr[i] == '\"'){
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
		bool toSpace = lineStr.find("import") == -1 && lineStr.find("[") == -1; 
        return (toSpace) ? spaceclear(lineStr) : lineStr;
    }
}

string getVariable(char *line)
{
    string lineStr = toString(line);
    string variable = substring(lineStr, "@", ":");
    return spaceclear(variable);
}

int processOperationConst(string operation, int counter, int j){
    int result = 0;
    for(j = 0; j < assembly["operations"][counter].size(); j++){
       string element = assembly["operations"][counter][j];

        if(element == operation){
            string number1 = assembly["operations"][counter][j - 1];
            string number2 = assembly["operations"][counter][j + 1];

            switch(operation[0]){
                case '*':
                    result = stoi(number1) * stoi(number2);
                    break;
                case '/':
                    result = stoi(number1) / stoi(number2);
                    break;
                case '+':
                    result = stoi(number1) + stoi(number2);
                    break;
                case '-':
                    result = stoi(number1) - stoi(number2);
                    break;
				case '%':
                    result = stoi(number1) % stoi(number2);
                    break;
				case '=':
					result = stoi(number1) == stoi(number2);
					break;
				case '>':
					if(operation[1] == '=')
						result = stoi(number1) >= stoi(number2);
					else	
						result = stoi(number1) > stoi(number2);
					break;
				case '<':
					if(operation[1] == '=')
						result = stoi(number1) <= stoi(number2);
					else 
						if(operation[1] == '>')	
							result = stoi(number1) != stoi(number2);
						else
							result = stoi(number1) < stoi(number2);
					break;
				case '~':
					result = stoi(number1) * !stoi(number2);
					break;
                case '^':
                    result = stoi(number1) ^ stoi(number2);
                    break;
                case '&':
                    result = stoi(number1) & stoi(number2);
                    break;
                case '|':
                    result = stoi(number1) | stoi(number2);
                    break;
            }

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
            assembly["type_vars"][var] = types[CONST_STRING];
            is_constant = true;
        }else{
            if(contains(line, "const[") && contains(line, "]")){
                assembly["type_vars"][var] = (contains(line, ".")) ? types[CONST_FLOAT] : types[CONST_INT];
                str = str.substr(str.find("[")+1, str.find("]")-str.find("[")-1);

                if(str == "true" || str == "false")
                    assembly["type_vars"][var] = types[CONST_BOOL];

                is_constant = true;
            }else{
				if(!contains(line, "[") && !contains(line, "]")){
					if(contains(line, "'"))
						assembly["type_vars"][var] = types[V_STRING];
					else
						assembly["type_vars"][var] = types[V_INT];
				}else{
					assembly["type_vars"][var] = "RETURN";
				}
            }
        }
    }else{
        assembly["type_vars"][var] = "RETURN";
    }
}

void setTypeVars(string value, int type1, int type2, int type3){
    if(isNumber(value) && value.find(".") != -1)
        assembly["type_vars"][var] = types[type1];
    else
        if(isNumber(value))
            assembly["type_vars"][var] = types[type2];
        else
            assembly["type_vars"][var] = types[type3];
}

// Verificador de variáveis, constantes e outros tipos
void parsingVariables(char *line){

    parsingConstant(line);              // Verificando constantes e expressões matemáticas
    assembly["operations"].clear();
}

// Verificador de constantes
void parsingConstant(char *line){
    if(!contains(line, "const[") && !isNumber(str) && str.find("import") == -1){

        char op;
        int count_op = 0;
        bool const_finded = true;
        bool has_variable = false;
        bool variable = false;

        stringstream var_name;

        for(int i = 0; i < str.length(); i++){
            variable = false;

            if(str[i] == '@'){
                variable = true;
                ++i;
            }

                while(i < str.length() && str[i] != '+' && str[i] != '-' && str[i] != '*' && str[i] != '/'
                                       && str[i] != '^' && str[i] != '%' && str[i] != '&' && str[i] != '|')
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
                        if(type == types[CONST_STRING] || type == types[CONST_INT] || type == types[CONST_FLOAT] || type == types[CONST_BOOL])
                            is_constant = true;

                        if(type.find("CONST") == -1)
                            const_finded = false;
                        else
                            if(const_finded)
                                assembly["type_vars"][var] = type.c_str();

                        assembly["vars"][var] = value.c_str();
                        str = assembly["vars"][var];

                    }else{
                        if(has_variable && const_finded){
                            is_constant = true;
                            setTypeVars(value, CONST_FLOAT, CONST_INT, CONST_STRING);
                        }else{
                            setTypeVars(value, V_FLOAT, V_INT, V_STRING);
                        }
                    }

                    if(assembly["operations"][0].size() > 1){
                        int result = 0;
                        int counter = 0;
                        assembly["operations"][0][count_op++] = value.c_str();
                        int j;

                        for(int i = 0; i < 15; i++)
                            counter = processOperationConst(operators[i], counter, j);

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
}

bool process_import_command(string lineImport){
    if(lineImport.find("import") != -1 && !is_comment){
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
    }
        return true;
}

string execFunction(string name){
    stringstream push_name;
    stringstream push_params;
	stringstream push_call;

    int i = 0;
    bool is_variable = false;
    bool is_plax_string = false;
    bool no_params = true;
    bool is_sub_function = false;
    int bracket = 1;

    while(name[i] == ' ' || name[i] == 0x09) i++;
    for(; name[i] != '['; i++) push_name << name[i];
    no_params = (name[++i] == ']');

	//cout << "FUNCOES DISPONIVEIS : " << assembly["local_funcs"] << endl;
	//cout << "FUNCAO ATUAL : '" << push_name.str() << "'" << endl;
	//system("pause");
	
    if(assembly["local_funcs"][push_name.str()].is_null())
        return "$!";

    //cout << "INICIO: " << name << endl;
    //cout << "FUNCTION NAME: " << push_name.str() << endl;
    //cout << "NAME[I]: " << name[i] << endl;
    //system("PAUSE");

    stringstream name_params;
    int sizename = (name[name.length()-1] == '\n') ? name.length()-1 : name.length();

    for(; i < sizename; i++){

        if(!is_sub_function){
            is_sub_function = (name[i] == '[');
            if(name[i] == '@' && name[i-1] != '{'){
                is_variable = true;
                ++i;
            }
            if(name[i] == '\''){
                is_plax_string = true;
                if(!is_plax_string)
                    ++i;
            }
            while((name[i] == ' ' || name[i] == 0x09)
                  && !is_plax_string)
                    i++;
        }

        if(name[i] == '[')
            ++bracket;
        if(name[i] == ']')
            --bracket;

        //cout << "CHAR : " << name[i] << endl;
        //cout << "BRACKET : " << bracket << endl;
        //system("PAUSE");

        //name[i] != '\'' &&
        if(name[i] != ',' && bracket != 0){
            if(!(is_plax_string && name[i] == '\''))
                name_params << name[i];
        }else{
			if(!no_params){
            // Processar parâmetros do meio
            string name_var = name_params.str();
            //cout << "PROCESSANDO: " << name_var << endl;
            //system("PAUSE");

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
                            if(typevar == "FUNCTION"){
                                push_params << "\tcall " << name_var << "\n";
                                push_params << "\tmov DWORD[_" << name_var << "_], eax\n";
                            }
                            push_params << "\tpush DWORD[_" << name_var << "_]";
                            //cout << "Entrou -> " << name_var << " : " << typevar << endl;
                            //cout << push_params.str() << endl;
                            //system("PAUSE");
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
                                        if(typevar == "FUNCTION"){
                                            push_params << "\tcall " << name_var << "\n";
                                            push_params << "\tmov DWORD[_" << name_var << "_], eax\n";
                                        }
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
					
					// CÓDIGO DE QUERY STRINGS ADICIONADO -----------------------------------------------------------------------------------
					stringstream querystr;
					stringstream strconv;
					stringstream strconv2;
					stringstream label_show;
					stringstream copy_query;
					int stack = 4;
					int countst = 0;
					if(name_var.find("{") != -1){
						bool is_querystr = false;
						assembly["text"][text_index++] = "\txor ebx, ebx";
						copy_query << "\tmov edi, eax" << endl;

						for(int j = 0; j < name_var.length(); j++){
							if(name_var[j] == '{'){
								j += 2;
								is_querystr = true;
							}
							if(name_var[j] == '}'){
								is_querystr = false;
								strconv << "{@" << querystr.str() << "}";

								stringstream staticstr;
								staticstr << "__" << push_name.str() << "_st__" << data_index;
								stringstream ebxconc;
								if(countst == 0){
									strconv2 << "'\n\t" << staticstr.str() << ".size equ $ - " << staticstr.str();
									ebxconc << "\tadd ebx, " << staticstr.str() << ".size";
									assembly["text"][text_index++] = ebxconc.str();
									copy_query << "\tmov esi, " << staticstr.str() << endl;
									copy_query << "\tmov ecx, " << staticstr.str() << ".size" << endl;
								}else{
									strconv2 << "'\n\t" << staticstr.str() << "_" << countst-1 << ".size equ $ - " << staticstr.str() << "_" << countst-1;
									ebxconc << "\tadd ebx, " << staticstr.str() << "_" << countst-1 << ".size";
									assembly["text"][text_index++] = ebxconc.str();
									copy_query << "\tmov esi, " << staticstr.str() << "_" << countst-1 << endl;
									copy_query << "\tmov ecx, " << staticstr.str() << "_" << countst-1 << ".size" << endl;
								}

								copy_query << "\trep movsb" << endl;

								ebxconc.str("");
								string typequery = assembly["type_vars"][querystr.str()];
								if(typequery == "INT" || typequery == "CONST STRING" || typequery == "CONST INT"){
									strconv2 << "\n\tdd " << querystr.str() << " \n\t" << staticstr.str() << "_" << countst << " db '";
									ebxconc << "\tpush " << querystr.str() << endl;
									ebxconc << "\tcall strlen" << endl;
									ebxconc << "\tadd esp, 4" << endl;
									ebxconc << "\tadd ebx, eax" << endl;
									ebxconc << "\tmov [ebp + " << stack << "], eax";
									assembly["text"][text_index++] = ebxconc.str();

									copy_query << "\tmov esi, " << querystr.str() << endl;
									copy_query << "\tmov ecx, [ebp + " << stack << "]" << endl;
									copy_query << "\trep movsb" << endl;
									stack += 4;
								}else{
									if(typequery == "STRING"){
										strconv2 << "\n\tdd _" << querystr.str() << "_ \n\t" << staticstr.str() << "_" << countst << " db '";
										ebxconc << "\tpush DWORD[_" << querystr.str() << "_]" << endl;
										ebxconc << "\tcall strlen" << endl;
										ebxconc << "\tadd esp, 4" << endl;
										ebxconc << "\tadd ebx, eax" << endl;
										ebxconc << "\tmov [ebp + " << stack << "], eax";
										assembly["text"][text_index++] = ebxconc.str();

										copy_query << "\tmov esi, DWORD[_" << querystr.str() << "_]" << endl;
										copy_query << "\tmov ecx, [ebp + " << stack << "]" << endl;
										copy_query << "\trep movsb" << endl;
										stack += 4;
									}
								}

								name_var.replace(name_var.find(strconv.str()), strconv.str().length(), strconv2.str());
								strconv2.str("");
								strconv.str("");
								querystr.str("");
								countst++;
							}
							if(is_querystr){
								querystr << name_var[j];
							}
						}

						stringstream lastaddr;
						lastaddr << "__" << push_name.str() << "_st__" << data_index << "_" << countst-1;
						stringstream ebxlast;
						ebxlast << "\tadd ebx, " << lastaddr.str() << ".size";
						assembly["text"][text_index++] = ebxlast.str();

						// apenas fora de uma função por enquanto
						stringstream push_inst;
						push_inst << "\tpush ebx ";
						assembly["text"][text_index++] = push_inst.str();
						assembly["text"][text_index++] = "\tcall malloc";
						assembly["text"][text_index++] = "\tadd esp, 4";
						label_show << "__" << push_name.str() << "_st__" << data_index << "_";
						push_inst.str("");
						push_inst << "\t" << label_show.str() << " resd 1";
						assembly["bss"][bss++] = push_inst.str();
						push_inst.str("");
						push_inst << "\tmov [" << label_show.str() << "], eax";
						assembly["text"][text_index++] = push_inst.str();
					}
					// CÓDIGO DE QUERY STRINGS ADICIONADO -----------------------------------------------------------------------------------
					
					
					
                    stringstream var_conc;
					bool contain_color = false;
					stringstream color_inst;
		
                    name_var = (name_var.find("\\n") != -1) ? replace_all(name_var, "\\n", "',13,10,'") : name_var;
                    name_var = (name_var.find("\\t") != -1) ? replace_all(name_var, "\\t", "',9,'") : name_var;
                    name_var = (name_var.find("\\v") != -1) ? replace_all(name_var, "\\v", "',11,'") : name_var;
			
                    if(name_var.find("\\x") != -1){
						contain_color = true;		// VARIÁVEL DE QUERY STRINGS/COLOR ADICIONADO
                        string strColor = name_var;
                        int index_start = strColor.find("\\x")+2;
                        strColor = strColor.substr(index_start, 2);

                        color_inst << "\tpush 0x" << strColor << endl;
                        color_inst << "\tpush DWORD[_stdout_]" << endl;
                        color_inst << "\tcall SetConsoleTextAttribute" << endl;
                        color_inst << "\tadd esp, 8" << endl;

                        stringstream colorconc;
                        colorconc << "\\x" << strColor;
                        string colorStr = colorconc.str();
                        name_var = replace_all(name_var, colorStr, "");
                    }

                    var_conc << "\t__" << push_name.str() << "_st__" << data_index << " db '" << name_var << "',0";
			
					// CÓDIGO DE QUERY STRINGS ADICIONADO -----------------------------------------------------------------------------------
					if(countst > 0){
						stringstream lastaddr;
						lastaddr << "__" << push_name.str() << "_st__" << data_index << "_" << countst-1;
						var_conc << "\n\t" << lastaddr.str() << ".size equ $ - __" << push_name.str() << "_st__" << data_index << "_" << countst-1;
						copy_query << "\tmov esi, " << lastaddr.str() << endl;
						copy_query << "\tmov ecx, " << lastaddr.str() << ".size" << endl;
						copy_query << "\trep movsb" << endl;
						assembly["text"][text_index++] = copy_query.str();
						push_params << "\tpush DWORD[" << label_show.str() << "]";
					}else{
						push_params << "\tpush __" << push_name.str() << "_st__" << data_index;
					}
					// CÓDIGO DE QUERY STRINGS ADICIONADO -----------------------------------------------------------------------------------
				
                    assembly["data"][data_index] = var_conc.str();
					//cout << "CODIGO: " << var_conc.str() << endl;
					//cout << "DATA_INDEX : " << data_index << endl;
					//cout << "ASSEMBLY DATA: " << assembly["data"][data_index] << endl;
                    var_conc.str("");
					
					// CÓDIGO DE QUERY STRINGS ADICIONADO -----------------------------------------------------------------------------------
					if(contain_color){
						if(!is_func)
							assembly["text"][text_index++] = color_inst.str();
						else
							assembly["text_funcs"][text_funcs++] = color_inst.str();
					}
					// CÓDIGO DE QUERY STRINGS ADICIONADO -----------------------------------------------------------------------------------
				
					// COMENTAR NOVAMENTE CASO EXCLUIR O CÓDIGO DE QUERY STRINGS
                    //push_params << "\tpush __" << push_name.str() << "_st__" << data_index;
					
					// CÓDIGO DE QUERY STRINGS ADICIONADO -----------------------------------------------------------------------------------
					//assembly["pushes_call2"][pushes2++] = push_params.str();
					// CÓDIGO DE QUERY STRINGS ADICIONADO -----------------------------------------------------------------------------------
					
					++data_index;
                    is_plax_string = false;
                }else{
                    if(isNumber(name_var)){
                        push_params << "\tpush " << name_var;
                    }else{
                        push_params << execFunction(name_var);

                        is_sub_function = false;

						//cout << "PROCESSOU: " << name_var << endl;
						//cout << "PROCESSOU i: " << name[i] << endl;
						//cout << "SUB_FUNCTION: " << is_sub_function << endl;
                        //system("PAUSE");

                        if(push_params.str() == "$!"){
                            cout << "A funcao '" << name_var << "' nao foi declarada!" << endl;
                            return "$!";
                        }


                        reading_func = false;

                        push_params << "\tpush eax" << endl;
                    }

                }
            }
		
			}else{
				push_call << "\tcall " << push_name.str() << endl;
			}
			
			assembly["pushes_call2"][pushes2++] = push_params.str();
            name_params.str("");
            push_params.str("");
        }

    }

    name_params.str("");
    push_params.str("");

	if(!no_params){
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

bool Interpret_Commands(FILE *file){
	plaxfile = file;
	typetok = 0;
	subtype = 0;
    while((fgets(line, sizeof(line), file)) != NULL){

		Lexical_Analyzer();
		if(is_error)
			return false;
		/*
        // IGNORADOR DE COMENTÁRIOS (MACRO)
        ignore_comments(line, file)

        // LEXER DE VARIÁVEIS : INT, STRING, FLOAT E BOOL
        if(!process_variables_attrib(line))
            return false;

        // LEXER DE DECLARAÇÃO DE FUNÇÕES
        if(!process_func_command(line))
            return false;

        // LEXER DE IMPORTAÇÃO DE DLLS E FUNÇÕES EXTERNAS
        if(!process_import_command(toString(line)))
            return false;

        // LEXER DE RETORNO DE FUNÇÕES
        if(!process_return_command(line))
            return false;

        // LEXER DE CHAMADA DE FUNÇÕES
        if(!process_functions_call(line))
            return false;

        // LEXER DE USO DA LIB PLAX
        if(!process_use_command(line))
            return false;
		*/
    }

    return true;
}

void Lexical_Analyzer(){
	typetok = 0, subtype = 0;
	while(typetok != ENDLINE && typetok != ENDFILE){
		Tokenize();
		Parser();
		if(is_error)
			return;
	}
}

void Tokenize(){
	typetok = get_token_type(toString(line));
	subtype = get_token(typetok, toString(line));
}

void Parser(){
	if(typetok != ENDLINE && typetok != ENDFILE){
		func_ptr = (void(*)())operations[subtype];
		func_ptr();
	}
}

int get_token_type(string line){
	
	int x = line_index;

	// SKIPAR OS ESPAÇOS E TABS
	for(; line[x] == ' ' || line[x] == 0x09; x++);
	line_index = x;
	
	// PRIMEIRO VERIFICA SE É FIM DE LINHA OU ARQUIVO
	if(line[x] == '\n')
		return ENDLINE;
	if(line[x] == 0 || line[x] == EOF)
		return ENDFILE;
	
	// ITERA PELOS TIPOS PRINCIPAIS DE TOKENS
	for(int i = 0; i < TOKENPOINTER_SIZE; i++){
		int type = iterate_token(line, x, i, tokenpointer[i]);
		if(type != UNKNOWN)
			return type;
	}
	
	// RETORNA TOKEN DESCONHECIDO CASO FALHE NAS ANÁLISES
	return UNKNOWN;
}

int iterate_token(string line, int x, int y, const char* tokens[]){
	for(int i = 0; i < tokensizes[y]; i++){
		token_len = strlen(tokens[i]);
		token_ind = i;
		string tokstring = line.substr(x, token_len);
		bool isRealToken = 	(line[x + token_len] == ' '  || line[x + token_len] == '[' ||
							 line[x + token_len] == '\n' || line[x + token_len] == '\t') || token_len == 1 &&
							!(((line[x] == diff_symbols[0][0] || line[x] == diff_symbols[1][0]) && 
							(line[x+1] == '=' || line[x+1] == '>') && y == 3));
		if(tokstring == tokens[i] && isRealToken)
			return tokentype[y];
	}
	return UNKNOWN;
}

// ESTA FUNÇÃO RETORNA UM INTEIRO DE SUB-TIPO DO TOKEN
int get_token(int type, string line){
	line_temp = line_number;
	// SE FOR FIM DE LINHA, INTERROMPA
	if(type == ENDLINE){
		line_index = 0; 
		line_number++;
		return type;
	}
	if(type == ENDFILE)
		return type;
	
	// ATRIBUI O TOKEN E IDENTIFICA SUB-CATEGORIA
	int subtype = get_token_subtype(type, line);

	// DEPURA AS INFORMAÇÕES NO TERMINAL
	//debug_token_type(type, subtype, line_index, token);
	
	return subtype;
}

int get_token_subtype(int type, string line){
	
	// VARIÁVEIS PARA TOKENS QUE IDENTIFICA FINAL DA KEYWORD
	char singlequote   = delimiter[QUOTE][0];
	char doublequote   = delimiter[QUOTE+1][0];
	char bracket_open  = delimiter[BRACKET][0];
	char bracket_close = delimiter[BRACKET+1][0];
	char keys_open 	   = delimiter[KEYS][0];
	char keys_close    = delimiter[KEYS+1][0];
	char inc_close     = delimiter[3][0];
	int index = line_index;
	stringstream symbol;
	
	// VERIFICAR O TOKEN DESCONHECIDO PODENDO SER:
	// TEXTO, NÚMERO, FUNÇÃO OU DEFINIÇÃO
	if(type == UNKNOWN){
		for(int i = 0; (line[index] != ' ' || is_quote_open) && line[index] != bracket_open && line[index] != bracket_close && 
						line[index] != '\n' && line[index] != inc_close && line[index] != '\t'; index++){
			if((is_quote_open && (line[index] == singlequote || line[index] == doublequote)) || line[index] == keys_open){
				token = symbol.str();
				line_index = index;
				return TEXT;
			}
			symbol << line[index];
		}
		token = symbol.str();
		line_index = index;
		if(isNumber(token))
			return NUMBER;
		if(line[index] == bracket_open)
			return FUNCTIONS;
		while(line[index] == ' ' || line[index] == '\n' || line[index] == '\t'){
			if(line[++index] == bracket_open)
				return FUNCTIONS;
		}
		return DEFINITION;
	}else{
		// VERIFICAR PREFIXOS, PODENDO SER: VARIÁVEIS, CONFIGURAÇÕES,
		// OU, SÍMBOLOS DE DELIMITAÇÃO, OPERAÇÕES E PALAVRAS-CHAVE
		const char** tokens = tokenpointer[type];
		bool isPrefix = (tokens[token_ind] == prefix[VARIABLE]) || (tokens[token_ind] == prefix[CONFIG]);
		if(isPrefix){
			int i = 0;
			for(++index; line[index] != ' ' && line[index] != '\n' && line[index] != '\t' &&
								   (line[index] >= 'A' && line[index] <= 'Z'  || line[index] >= 'a'  && 
									line[index] <= 'z' || line[index] >= '0'  && line[index] <= '9'); index++){
				symbol << line[index];
			}
			token = symbol.str();
			line_index = index;
			if(tokens[token_ind] == prefix[VARIABLE])
				return VARIABLE;
			return CONFIG;
		}else{
			index += token_len;
			symbol << line.substr(line_index, index-line_index);
			token = symbol.str();
			bool is_quote = (token[0] == singlequote || token[0] == doublequote);
			is_quote_open = (is_quote) ? is_quote && !is_quote_open : is_quote_open;
			line_index = index;
			return get_subtypes(token);
		}	
	}
}

int get_subtypes(string token){
	bool is_use = (token == command[1] || token == delimiter[3] && is_use_command);
	if((token == delimiter[2] || token == delimiter[3]) && is_use_command){
		is_use_command = (is_use) ? is_use && !is_use_command : is_use_command;
		return INCLUDE;
	}
	is_use_command = (is_use) ? is_use && !is_use_command : is_use_command;
	for(int i = 1; i <= 5; i++)
		if(token == operators[i])
			return ARITHMETIC;
	for(int i = 6; i <= 11; i++)
		if(token == operators[i])
			return RELATIONAL;
	for(int i = 12; i <= 17; i++)	
		if(token == operators[i])
			return LOGICAL;
	for(int i = 7; i <= 14; i++)	
		if(token == command[i])
			return CONVERSION;
	if(token == operators[0] || token == operators[18])
		return ATTRIBUTION;
	if(token == delimiter[0] || token == delimiter[1])
		return COMMENTS;
	if(token == delimiter[6] || token == delimiter[7])
		return SCOPE;
	if(token == delimiter[8] || token == delimiter[9])
		return STRUCTS;
	if(token == delimiter[12])
		return SEPARATOR;
	if(token == command[15] || token == command[16])
		return CONDITIONAL;
	if(token == command[17] || token == command[18])
		return REPETITION;
	if(token == command[0] || token == command[1])
		return INCLUDE;
	if(token == command[2])
		return DECLARATION;
	if(token == command[3])
		return RETURN;
	if(token == delimiter[4] || token == delimiter[5])
		return PARAMETERS;
	if(token == command[5])
		return EXTERNINT;
	if(token == command[6])
		return EXTERNCALL;
	if(token == delimiter[10])
		return SINGLEQUOTE;
	return OTHER;	
}

bool get_state(int type, int subtok){
	return (typetok == type && subtype == subtok); 
}

bool end_state(int condition){
	return !(typetok == condition);
}

void debug_token_type(int type_token, int subtype, int line_index, string token){
	cout << "TYPE TOKEN  : " << tokentypes[type_token+1] << endl;
	cout << "SUB TYPE    : " << subtypes[subtype] << endl;
	cout << "TOKEN       : " << token << endl;
	cout << "POSITION    : " << line_index << endl;
	cout << "LINE NUMBER : " << line_number << endl; 
	system("pause");
}

void debug_msg(string msg){
	cout << msg << endl;
}

bool process_variables_attrib(char *line){ // begin function

    bool attrib = toString(line).find("@") < toString(line).find(":");

	// SE É UMA VARIÁVEL
    if(contains(line, "@") && contains(line, ":") && attrib && !contains(line, "local") && !contains(line, "func") && !is_comment){ // begin if 1
			
        str = getString(line); 
        var = getVariable(line);
        StoreTypeVars(line);
		//cout << "ATTRIB: " << var << " = " << str << " -> " << assembly["type_vars"][var] << endl;
		//system("pause");

        bool exist_var = (!assembly["vars"][var].is_null()) ? true : false;
        if(!is_func)
            assembly["vars"][var] = str.c_str();

        parsingVariables(line);

		// SE É UMA VARIÁVEL CONSTANTE
        if(is_constant){
			// SE É UMA VARIÁVEL CONSTANTE INTEIRA
            if(isNumber(str) && assembly["type_vars"][var] == "INT"){
                if(!assembly["type_vars"][var].is_null())
                    type = assembly["type_vars"][var];

                stringstream var_conc;
				// SE NÃO ESTÁ DENTRO DE UMA FUNÇÃO
                if(!is_func){
                    var_conc << "\t" << var << " dd " << str;
                    assembly["rodata"][rodata_index++] = var_conc.str();
                }else{	// SE ESTÁ DENTRO DE UMA FUNÇÃO
						
					// SE A VARIÁVEL DE PARÂMETRO DA FUNÇÃO EXISTE
                    if(!assembly["local_funcs"][func_name][var].is_null()){
                        string param = assembly["local_funcs"][func_name][var];
                        var_conc << "\tmov " << param << ", " << str;
                    }else{ // SE O PARÂMETRO DA FUNÇÃO NÃO EXISTE
							
						// SE A VARIÁVEL GLOBAL NÃO EXISTE
                        if(assembly["vars"][var].is_null()){
                            local_offsets += 4;
                            stringstream stacklocal;
                            stacklocal << "DWORD[ebp - " << local_offsets << "]";
                            var_conc << "\tmov " << stacklocal.str() << ", " << str;
                            assembly["local_names"][func_name][var] = stacklocal.str();
                        }else{ // SE A VARIÁVEL GLOBAL EXISTE
                            var_conc << "\tmov DWORD[" << var << "], " << str;
                        }
                    } // SE ESTÁ DENTRO DE UMA FUNÇÃO
                    assembly["text_funcs"][text_funcs++] = var_conc.str();
                }
				
			// SE NÃO É UMA VARIÁVEL CONSTANTE INTEIRA
            }else{
                if(!assembly["type_vars"][var].is_null())
                    type = assembly["type_vars"][var];
				
                stringstream var_conc;

                bool is_plax_bool = false;

				// SE FOR DO TIPO BOOLEANO CONSTANTE
                if(type == "CONST BOOL"){
                    is_plax_bool = true;
                    if(str == "true")
                        var_conc << "\t" << var << " db 1";
                    else
                        var_conc << "\t" << var << " db 0";
				// SE NÃO FOR BOOLEANO CONSTANTE
                }else{
                    str = (str.find("\\n") != -1) ? replace_all(str, "\\n", "',13,10,'") : str;
                    str = (str.find("\\t") != -1) ? replace_all(str, "\\t", "',9,'") : str;
                    str = (str.find("\\v") != -1) ? replace_all(str, "\\v", "',11,'") : str;
					
					// SE É UM CÓDIGO DE CORES PARA STRINGS
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
                    } // SE É UM CÓDIGO DE CORES PARA STRINGS
                    var_conc << "\t" << var << " db '" << str << "',0";
                }

                assembly["rodata"][rodata_index++] = var_conc.str();
                var_conc.str("");

                if(!is_plax_bool){
                    var_conc << "\t" << var << ".size equ $ - " << var;
                    assembly["rodata"][rodata_index++] = var_conc.str();
                }
            }

		// SE NÃO É UMA CONSTANTE
        }else{ 
            stringstream var_conc;

			// SE A VARIÁVEL NÃO EXISTE OU NÃO FOI ALOCADA
            if(!exist_var){
				// SE É UM NÚMERO (INTEIRO)
                if(isNumber(str)){
                    if(!assembly["type_vars"][var].is_null())
                        type = assembly["type_vars"][var];

					// SE NÃO ESTÁ DENTRO DE UMA FUNÇÃO
                    if(!is_func){
                        var_conc << "\t" << var << " dd " << str;
                        assembly["data"][i++] = var_conc.str();
                    }else{	// SE ESTÁ DENTRO DE UMA FUNÇÃO
					
						// SE A VARIÁVEL DE PARÂMETRO DA FUNÇÃO EXISTE
                        if(!assembly["local_funcs"][func_name][var].is_null()){
                            string param = assembly["local_funcs"][func_name][var];
                            var_conc << "\tmov " << param << ", " << str;
                        }else{ // SE O PARÂMETRO DA FUNÇÃO NÃO EXISTE
						
							// SE A VARIÁVEL GLOBAL NÃO EXISTE
                            local_offsets += 4;
                            stringstream stacklocal;
                            stacklocal << "DWORD[ebp - " << local_offsets << "]";
                            var_conc << "\tmov " << stacklocal.str() << ", " << str;
                            assembly["local_names"][func_name][var] = stacklocal.str();
                        }
                        assembly["text_funcs"][text_funcs++] = var_conc.str();
                    }

				// SE NÃO É UM NÚMERO
                }else{
                    if(!assembly["type_vars"][var].is_null())
                        type = assembly["type_vars"][var];

					// SE FOR UMA IMPORTAÇÃO EXTERNA
                    if(str.find("import") != -1){
                        stringstream strbreak;
                        strbreak << str << "\n";
                        bool success = process_import_command(strbreak.str());    //strbreak.str()
                        if(!success)
                            return false;

                        var_conc << "\t_" << var << "_" << " resd 1 ";
                        assembly["bss"][bss++] = var_conc.str();
                        var_conc.str("");
                        var_conc << "\tmov DWORD[_" << var << "_], eax";

                        if(!is_func)
                            assembly["text"][text_index++] = var_conc.str();
                        else
                            assembly["text_funcs"][text_funcs++] = var_conc.str();

                        assembly["realloc"][var] = false;
							
					// SE NÃO FOR UMA IMPORTAÇÃO EXTERNA
                    }else{
						// SE NÃO FOR UMA FUNÇÃO
                        if(str.find("[") == -1 && str.find("]") == -1){
                            if(str == "NULL")	// SE É NULL
                                var_conc << "\t" << var << " db 0";
                            else{
                                str = (str.find("\\n") != -1) ? replace_all(str, "\\n", "',13,10,'") : str;
                                str = (str.find("\\t") != -1) ? replace_all(str, "\\t", "',9,'") : str;
                                str = (str.find("\\v") != -1) ? replace_all(str, "\\v", "',11,'") : str;
								
								// SE CONTÉM CÓDIGO PARA CORES DE STRINGS
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
                                }	// SE CONTÉM CÓDIGO PARA CORES DE STRINGS
								
                                var_conc << "\t" << var << " db '" << str << "',0";
                            } // SE NÃO É NULL

                                assembly["data"][i] = var_conc.str();
                                var_conc.str("");
								
						// SE FOR UMA FUNÇÃO
                        }else{
							//cout << "EXECUTANDO : " << str << endl;
							//system("pause");
							
                            var_conc << execFunction(str);
							
							//cout << "EXECUTOU : " << str << endl;
							//system("pause");
							
                            if(var_conc.str() == "$!"){
                                cout << "A funcao '" << str << "' nao foi declarada!" << endl;
                                return false;
                            }
							
							//cout << "PASSOU : " << str << endl;
							//system("pause");
                            if(var_conc.str() == "$$"){
                                cout << "Error: Erro ao executar a funcao!" << endl;
                                return false;
                            }
                            var_conc << "\tmov DWORD[_" << var << "_], eax";
                            if(!is_func)
                                assembly["text"][text_index++] = var_conc.str();
                            else
                                assembly["text_funcs"][text_funcs++] = var_conc.str();

                            var_conc.str("");
                        } // SE FOR UMA FUNÇÃO

                        var_conc << "\t_" << var << "_" << " resd 1 ";
                        assembly["bss"][bss++] = var_conc.str();
                        assembly["realloc"][var] = false;
                    } // SE NÃO FOR UMA IMPORTAÇÃO EXTERNA
                } // SE NÃO É UM NÚMERO
					
			// SE EXISTE VARIÁVEL
            }else{

					// SE A VARIÁVEL EXISTENTE FOR UM NÚMERO INTEIRO 
                    if(isNumber(str) && assembly["type_vars"][var] == "INT"){
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
						
					// SE A VARIÁVEL EXISTENTE NÃO FOR UM NÚMERO INTEIRO
                    }else{
                        if(!assembly["type_vars"][var].is_null())
                                type = assembly["type_vars"][var];

                            //cout << type << ": " << "\"" << str << "\", VAR: @" << var << endl;

                        var_conc << "_" << var << "_";

                        stringstream push_inst;
						
						// SE NÃO ESTÁ DENTRO DE UMA FUNÇÃO
                        if(!is_func){
                            if(!assembly["realloc"][var]){ // SE VARIÁVEL NÃO FOI ALOCADA
                                push_inst << "\tpush " << var << "__" << i << ".size";
                                assembly["text"][text_index++] = push_inst.str();
                                assembly["text"][text_index++] = "\tcall malloc";
                                assembly["text"][text_index++] = "\tadd esp, 4";
                            }else{	// SE A VARIÁVEL JÁ FOI ALOCADA
                                push_inst << "\tpush " << var << "__" << i << ".size";
                                assembly["text"][text_index++] = push_inst.str();
                                push_inst.str("");
                                push_inst << "\tpush DWORD[_" << var << "_]";
                                assembly["text"][text_index++] = push_inst.str();
                                assembly["text"][text_index++] = "\tcall realloc";
                                assembly["text"][text_index++] = "\tadd esp, 8";
                            } // SE A VARIÁVEL JÁ FOI ALOCADA

                            stringstream addr_inst;
                            addr_inst << "\tmov [" << var_conc.str() << "], eax";
                            var_conc.str("");
                            str = (str.find("\\n") != -1) ? replace_all(str, "\\n", "',13,10,'") : str;
                            str = (str.find("\\t") != -1) ? replace_all(str, "\\t", "',9,'") : str;
                            str = (str.find("\\v") != -1) ? replace_all(str, "\\v", "',11,'") : str;

                            stringstream color_inst;
                            bool has_color = false;

							// SE CONTÉM CÓDIGO PARA CORES DE STRINGS 
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
                            } // SE CONTÉM CÓDIGO PARA CORES DE STRINGS
							
							//cout << "DATA INDEX NO VARIABLE : " << data_index << endl;
							//system("pause");
							//i = data_index;
                            var_conc << "\t" << var << "__" << i << " db '" << str << "',0" << "\n"
									 << "\t" << var << "__" << i << ".size equ $ - " << var << "__" << i;
                            assembly["data"][i] = var_conc.str();
                            var_conc.str("");

                            var_conc << "\tmov esi, " << var << "__" << i;
                            assembly["text"][text_index++] = addr_inst.str();
                            assembly["text"][text_index++] = "\tmov edi, eax";
                            assembly["text"][text_index++] = var_conc.str();
                            var_conc.str("");

                            var_conc << "\tmov ecx, " << var << "__" << i << ".size";
                            assembly["text"][text_index++] = var_conc.str();
                            assembly["text"][text_index++] = "\trep movsb";

							++i;
							data_index = i;
							
                            if(has_color){
                                if(!is_func)
                                    assembly["text"][text_index++] = color_inst.str();
                                else
                                    assembly["text_funcs"][text_funcs++] = color_inst.str();
                            }
							
						// SE ESTÁ DENTRO DE UMA FUNÇÃO
                        }else{
                            if(!assembly["realloc"][var]){ // SE A VARIÁVEL NÃO FOI ALOCADA
                                push_inst << "\tpush " << var << "__" << i << ".size";
                                assembly["text_funcs"][text_funcs++] = push_inst.str();
                                assembly["text_funcs"][text_funcs++] = "\tcall malloc";
                                assembly["text_funcs"][text_funcs++] = "\tadd esp, 4";
                            }else{	// SE A VARIÁVEL JÁ FOI ALOCADA
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
							// SE CONTÉM CÓDIGO PARA CORES DE STRINGS
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
                            } // SE CONTÉM CÓDIGO PARA CORES DE STRINGS
							
							//i = data_index;
                            var_conc << "\t" << var << "__" << i << " db '" << str << "',0" << "\n"
									 << "\t" << var << "__" << i << ".size equ $ - " << var << "__" << i;
                            assembly["data"][i] = var_conc.str();
                            var_conc.str("");

                            var_conc << "\tmov esi, " << var << "__" << i;
                            assembly["text_funcs"][text_funcs++] = addr_inst.str();
                            assembly["text_funcs"][text_funcs++] = "\tmov edi, eax";
                            assembly["text_funcs"][text_funcs++] = var_conc.str();
                            var_conc.str("");

                            var_conc << "\tmov ecx, " << var << "__" << i << ".size";
                            assembly["text_funcs"][text_funcs++] = var_conc.str();
                            assembly["text_funcs"][text_funcs++] = "\trep movsb";
							
							++i;
							data_index = i;

                            if(has_color){
                                if(!is_func)
                                    assembly["text"][text_index++] = color_inst.str();
                                else
                                    assembly["text_funcs"][text_funcs++] = color_inst.str();
                            }
							
							
                        } // SE ESTÁ DENTRO DE UMA FUNÇÃO

                        //i++;

                        assembly["realloc"][var] = true;
                    } // SE A VARIÁVEL EXISTENTE NÃO FOR UM NÚMERO INTEIRO
					
					
			} // SE EXISTE VARIÁVEL
		
		} // SE NÃO É UMA CONSTANTE
				
    } // SE É UMA VARIÁVEL

	return true;
} // FIM DA FUNÇÃO

// Comando de declaração de funções
bool process_func_command(char *line){
    if(is_func && !is_comment){
        if(contains(line, ")") && index_symbol == 0){
            is_func = false;
            assembly["text_funcs"][text_funcs++] = "\tmov esp, ebp";
            assembly["text_funcs"][text_funcs++] = "\tpop ebp";
            assembly["text_funcs"][text_funcs++] = "\tret";
        }else{
            if(contains(line, ")"))
                --index_symbol;
        }

        if(contains(line, "("))
            ++index_symbol;
    }

    if(contains(line, "func") && !is_comment){
        local_offsets = 0;
        is_func = true;

        stringstream lineconc;
        string line1;
        for(int i = 1; i < strlen(line); i++)
            lineconc << line[i];

        line1 = lineconc.str();

        string nameFunc = (line1.find(":") != -1) ? line1 : toString(line);

        if(nameFunc.find("(") == -1)
            return false;

        tokenfunc = substring(nameFunc, "func", "@");
        if(tokenfunc == "")
            tokenfunc = substring(nameFunc, "func", "(");

        nameFunc = spaceclear(tokenfunc);


        // Funções anônimas
        if(nameFunc.length() < 1 || nameFunc[0] == '(' || nameFunc[0] == '@'){
            tokenfunc = substring(toString(line), "@", ":");
            if(tokenfunc == "")
                return false;

                nameFunc = spaceclear(tokenfunc);

                stringstream func_variable;
                assembly["type_vars"][nameFunc] = "FUNCTION";
                func_variable << "\t_" << nameFunc << "_ resd 1";
                assembly["bss"][bss++] = func_variable.str();
        }

        func_name = nameFunc;

        stringstream labels;
        labels << nameFunc << ":";
        assembly["text_funcs"][text_funcs++] = labels.str().c_str();
        assembly["text_funcs"][text_funcs++] = "\tpush ebp";
        assembly["text_funcs"][text_funcs++] = "\tmov ebp, esp";

        process_function_params((char*) line1.c_str());
    }
    return true;
}

// Filtra e Armazena parâmetros da função 'func_name'
void process_function_params(char *line){

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
                assembly["local_funcs"][func_name][local_name.str()] = ebp_value.str();
                local_name.str("");
                ebp_value.str("");

                if(line[i] == '(')
                    break;
            }
        }
    }else{
		assembly["local_funcs"][func_name][""] = "";
	}
}

// Comando de retorno de funções
bool process_return_command(char *line){
    if(contains(line, "return") && !is_comment){
        string lineStr = toString(line);

        lineStr = getstring(lineStr, "return", lineStr.length());

        stringstream strbreak;

        if(lineStr.find("import") != -1){
            strbreak << lineStr << "\n";
            if(!process_import_command(strbreak.str()))   //strbreak.str()
                return false;

            strbreak.str("");
            strbreak << "\tmov DWORD[_return_funcs_], eax";
        }else{
            stringstream lineStr2;
            lineStr2 << lineStr;
            string lineStrNoSpace = spaceclear(lineStr2.str());
            if(isNumber(lineStrNoSpace))
                strbreak << "\tmov eax, " << lineStrNoSpace.c_str();
            else{
                if(lineStrNoSpace[0] == '@'){
                    lineStrNoSpace = replace_all(lineStrNoSpace, "@", "");
                    if(!assembly["vars"][lineStrNoSpace].is_null())
                        if(assembly["type_vars"][lineStrNoSpace] == types[V_STRING])
                            strbreak << "\tmov eax, DWORD[_" << lineStrNoSpace << "_]";
                        else
                            strbreak << "\tmov eax, DWORD[" << lineStrNoSpace << "]";
                    else{
                        if(!assembly["local_funcs"][func_name][lineStrNoSpace].is_null()){
                            string var_param = assembly["local_funcs"][func_name][lineStrNoSpace];
                            strbreak << "\tmov eax, " << var_param;
                        }else{
                            cout << "A variável no retorno de função não existe!" << endl;
                            return false;
                        }
                    }
                }else{
                    stringstream return_name_str;
                    stringstream return_string;
                    return_name_str << "\t__Return_" << func_name << "_" << rodata_index;
                    return_string << return_name_str.str() << " db " << lineStr.c_str() << ",0\n";
                    assembly["rodata"][rodata_index++] = return_string.str();
                    strbreak << "\tmov eax, " << return_name_str.str();
                }
            }
        }

        if(!is_func)
            assembly["text"][text_index++] = strbreak.str();
        else
            assembly["text_funcs"][text_funcs++] = strbreak.str();
    }
    return true;
}

// Executa funções chamadas no PLAX
bool process_functions_call(char *line){
    if(contains(line, "[") && !contains(line, "import") && !contains(line, "const") 
		&& !contains(line, ":") && !is_comment){
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
    return true;
}

// Comando para incluir arquivos ou usar recursos/apis
bool process_use_command(char *line){
    if(contains(line, "use") && contains(line, "of") && !is_comment){
        string lineUse = toString(line);
        int j = 0;
        while(lineUse[j] == ' ' || lineUse[j] == 0x09) j++;

        if(lineUse[j] == 'u' && lineUse[j+1] == 's' && lineUse[j+2] == 'e'){

            string lineOf = lineUse;
            lineUse = spaceclear(substring(lineUse, "use", "of"));
            lineOf = spaceclear(getstring(lineOf, "of", lineOf.length()));

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

            if(lineUse == "*" || lineUse == "*.*")
                use_all = true;

            bool finish = Interpret_Commands(file_inc);
            if(!finish)
                return false;
        }
    }
    return true;
}

#endif
