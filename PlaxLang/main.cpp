// Inclusões de Bibliotecas de C e C++
#include "plax_utils.h"


int main(int argc, char** argv) {

    if(argc > 1){
        if(argv[1] != NULL){
            FILE *file_read;

            if((file_read = fopen(argv[1], "r")) == NULL){
                cout << "Erro em abrir o arquivo" << endl;
                return 1;
            }

            assembly["global"][0] = "main";
            assembly["extern"][ext_index++] = "ExitProcess";
            assembly["extern"][ext_index++] = "malloc";
            assembly["extern"][ext_index++] = "realloc";
            assembly["text"][text_index++] = "main:";

            assembly["dlls"][dll_count++] = "kernel32.dll";
            assembly["dlls"][dll_count++] = "msvcrt.dll";
            assembly["bss"][bss++] = "\t_return_funcs_ resd 1";

            i = 0;
            
            bool finish = Interpret_Commands(file_read);
            if(!finish)
                return 0;


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
            golink << "GoLink /console /entry main " << file.str() << ".obj /fo " << argv[2];

            for(int i = 0; i < assembly["dlls"].size(); i++)
            {
                string dll_link = assembly["dlls"][i];
                golink << " " << dll_link;
            }
            string golink_cmd = golink.str();
            system(golink_cmd.c_str());

            //cout << golink_cmd << endl;

            system("del *.asm");
            system("del *.obj");

            system(argv[2]);

        }
    }

    return 0;
}