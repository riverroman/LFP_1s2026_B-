#pragma once
#include "Token.h"
#include <string>
#include <vector>

// ============================================================
//  Analizador Lexico basado en AFD manual
//  PROHIBIDO std::regex -- toda la logica es por estados
// ============================================================
class LexicalAnalyzer {
public:
    explicit LexicalAnalyzer(const std::string& source);

    // Retorna el siguiente token del flujo de entrada.
    // Cuando llega al fin retorna Token(FIN_ARCHIVO, "", ...)
    Token nextToken();

    // Tokeniza todo el fuente de una vez (comodidad para el servidor)
    std::vector<Token> tokenizeAll();

    int getLine()   const { return linea_; }
    int getColumn() const { return columna_; }

private:
    std::string src_;
    size_t pos_;
    int linea_;
    int columna_;

    // Primitivas de lectura
    char peek(int offset = 0) const;
    char advance();
    // Comentarios
    void skipWhitespaceAndComments();

    // Clasificadores de caracteres (SIN isalpha/isdigit de <cctype>)
    // Se implementan verificando rangos ASCII directamente
    static bool esLetra(char c);
    static bool esDigito(char c);
    static bool esLetraODigito(char c);

    // Reconocedores de cada clase de token
    Token leerIdentificadorOPalabraReservada(int linInicio, int colInicio);
    Token leerCadena(int linInicio, int colInicio);
    Token leerNumeroOFecha(int linInicio, int colInicio);

    // Tabla de palabras reservadas -> tipo de token
    static TokenType palabraReservada(const std::string& lex);
};