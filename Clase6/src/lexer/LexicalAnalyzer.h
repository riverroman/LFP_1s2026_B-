#ifndef LEXICA_ANALYZER_H
#define LEXICA_ANALYZER_H

#include <string>
#include <vector>
#include "../model/Token.h"
#include "../errors/ErrorManager.h"

class LexicalAnalyzer {
    // input seria la entrada de texto a analizar
private:
    std::string input;
    int pos;
    int line;
    int column;
    // tokens es el vector donde se almacenan los tokens encontrados
    char peek();
    // advance mueve el puntero de lectura al siguiente caracter y devuelve el caracter actual
    char advance();

public:
    // Lexico -> Tokens
    std::vector<Token> tokens;
    // Lexico -> Errores
    std::vector<LexicalError> errors;
    LexicalAnalyzer(std::string text);
    void analyze();
};

#endif