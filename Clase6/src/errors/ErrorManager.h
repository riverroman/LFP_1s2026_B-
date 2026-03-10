#ifndef LEXICAL_ERROR_H
#define LEXICAL_ERROR_H
#include <string>
using namespace std;

class LexicalError{

    //  lexema          line      column       description
    //  "@"              5         10          "Caracter no reconocido"

public:
    string lexeme; // Capturar el lexema que encontre 
    int line; // Capturar la linea donde se encuentra el error
    int column; // Capturar la columna donde se encuentra el error
    string description; // Capturar la descripcion del error
    
    LexicalError(string lex, int l, int c, string desc){
        lexeme = lex;
        line = l;
        column = c;
        description = desc;
    }
};

#endif