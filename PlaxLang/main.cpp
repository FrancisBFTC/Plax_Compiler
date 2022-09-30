// Inclusões de Bibliotecas de C e C++
#include "plax_utils.h"


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
            int text_funcs = 0;
            int rodata_index = 0;

            assembly["global"][0] = "main";
            assembly["extern"][ext_index++] = "ExitProcess";
            assembly["extern"][ext_index++] = "malloc";
            assembly["extern"][ext_index++] = "realloc";
            assembly["text"][text_index++] = "main:";

            int i = 0;
            int bss = 0;

            int index_symbol = 0;
            bool is_func = false;
            
            while((fgets(line, sizeof(line), file_read)) != NULL){
                if(contains(line, "@") && contains(line, ":") && !contains(line, "local")){
                    
                    str = getString(line);
                    var = getVariable(line);
                    StoreTypeVars(line);
                    
                    bool exist_var = (!assembly["vars"][var].is_null()) ? true : false;
                    if(!is_func)
                        assembly["vars"][var] = str.c_str();

                    parsingVariables(line);

                    if(is_constant){
                        if(isNumber(str)){
                            string type = assembly["type_vars"][var];
                            cout << type << ": " << str << ", VAR: @" << var << endl;
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
                                        var_conc << "\tmov DWORD[ebp - " << local_offsets << "], " << str;
                                    }else{
                                        var_conc << "\tmov DWORD[" << var << "], " << str;
                                    }
                                }
                                assembly["text_funcs"][text_funcs++] = var_conc.str();
                            }
                        }else{
                            string type = assembly["type_vars"][var];
                            cout << type << ": " << "\"" << str << "\", VAR: @" << var << endl;
                            stringstream var_conc;
                            
                            bool is_plax_bool = false;

                            if(type == "CONST BOOL"){
                                is_plax_bool = true;
                                if(str == "true")
                                    var_conc << "\t" << var << " db 1";
                                else
                                    var_conc << "\t" << var << " db 0";
                            }else
                                var_conc << "\t" << var << " db '" << str << "'";

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
                                if(!is_func){
                                    var_conc << "\t" << var << " dd " << str;
                                    assembly["data"][i++] = var_conc.str();
                                }else{
                                    if(!assembly["local_funcs"][func_name][var].is_null()){
                                        string param = assembly["local_funcs"][func_name][var];
                                        var_conc << "\tmov " << param << ", " << str;
                                    }else{
                                        local_offsets += 4;
                                        var_conc << "\tmov DWORD[ebp - " << local_offsets << "], " << str;
                                    }
                                    assembly["text_funcs"][text_funcs++] = var_conc.str();
                                }

                            }else{
                                if(str == "NULL")
                                    var_conc << "\t" << var << " db 0";
                                else
                                    var_conc << "\t" << var << " db '" << str << "'";

                                assembly["data"][i] = var_conc.str();

                                var_conc.str("");

                                var_conc << "\t_" << var << "_" << " resd 1 ";
                                assembly["bss"][bss++] = var_conc.str();
                                assembly["realloc"][var] = false;
                            }
                        }else{
                            
                            if(isNumber(str)){
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
                                    var_conc << "\t" << var << "__" << i << " db '" << str << "'";

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
                                }

                                i++;
                            
                                assembly["realloc"][var] = true;
                            }
                        }
                    }

                    
                }

                if(is_func){
                    if(contains(line, ")") && index_symbol == 0){
                        is_func = false;
                        cout << "FUNC END " << index_symbol << " => " << line << endl;
                        assembly["text_funcs"][text_funcs++] = "\tret";
                    }else{
                        if(contains(line, ")")){
                            cout << "CONDITION " << index_symbol << " END:" << line << endl;
                            index_symbol -= 1;
                        }
                    }

                    if(contains(line, "(")){
                          index_symbol += 1;
                          cout << "CONDITION " << index_symbol << ": " << line << endl;
                    }

                }

                if(contains(line, "func")){
                    
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

                    cout << endl << assembly["local_funcs"] << endl;
                    cout << "FUNC BEGIN " << index_symbol << " => " << line << endl;

                }
            }

            

            assembly["text"][text_index++] = "\tpush 0";
            assembly["text"][text_index++] = "\tcall ExitProcess";

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

            for(int i = 0; i < assembly["bss"].size(); i++){
                if(i == 0)
                    output << "section .bss" << endl << endl;
                
                code = assembly["bss"][i];
                output << code << endl;
            }

            output << endl;

            for(int i = 0; i < assembly["rodata"].size(); i++){
                if(i == 0)
                    output << "section .rodata" << endl << endl;
                
                code = assembly["rodata"][i];
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


            for(int i = 0; i < assembly["text_funcs"].size(); i++){
                code = assembly["text_funcs"][i];
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

            //system("del *.asm");
            system("del *.obj");

            system(argv[2]);

        }
    }

    return 0;
}