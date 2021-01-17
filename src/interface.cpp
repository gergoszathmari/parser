extern "C"{
    #include "interface.h"
    #include "y.tab.h"
    #include <string.h>


    int isTypedef = 0;
    int isExtern = 0;
    int isEnum = 0;
    int isStructDeclaration = 0;
    int isDeclaration = 0;
    int errorAt[2] = {0,0};
    int nextId = 0;

}
#include <iostream>
#include <set>

std::set<std::string> typenames;

void clearTypes(){
    typenames.clear();
}

void printTypenames(){
    std::cout << "\n\n";
    for(auto& t : typenames){
        std::cout << '|' << t << "|\n";
    }
}

int sym_type(const char * tok){

    if(isExtern){
        isExtern = 0;
        return TYPEDEF_NAME;
    }
    
    if(typenames.find(tok) != typenames.end()){
        return TYPEDEF_NAME;
    }

    if(isTypedef && !isStructDeclaration){
        
        typenames.emplace(std::string(tok));
        isTypedef = 0;
        if(nextId){
            //std::cout << '|' << tok << "|\n";
            nextId = 0;
            return IDENTIFIER;
        }
        return TYPEDEF_NAME;
    }
    
    return IDENTIFIER;
}