#pragma once
#include <string>

// enum clase para definir los tipos de tokens
enum class TokenType{
    IDENTIFIER, // para nombres de variables, funciones, etc.
    NUMBER,     // para números enteros o decimales
    STRING,     // para cadenas de texto
    DATE,       // para fechas en formato específico
    TIME,       // para horas en formato específico
    ID_CODE,    // para códigos de identificación
    SYMBOL,     // para símbolos como operadores, paréntesis, etc
};

// clase Token para representar un token con su tipo y valor
class Token {
    //   10 -> TokenType::NUMBER, "10" , 1, 5
public:
    TokenType type; // tipo del token
    std::string lexeme;   // valor del token
    int line;          // número de línea donde se encuentra el token
    int column;        // número de columna donde se encuentra el token

    Token(TokenType type, std::string lexeme, int line, int column);

    std::string getTypeString() const; // método para obtener el tipo del token como string

};