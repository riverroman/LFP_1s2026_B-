#pragma once
#include "Token.h"
#include "ErrorManager.h"
#include <vector>
#include <string>

// Nodo del arbol de derivacion
struct NodoArbol {
    std::string etiqueta;          // nombre de la produccion o lexema
    bool        esTerminal;        // true = hoja (token), false = no-terminal
    std::vector<NodoArbol*> hijos;

    NodoArbol(std::string e, bool term)
        : etiqueta(std::move(e)), esTerminal(term) {}
    ~NodoArbol() { for (auto* h : hijos) delete h; }
};

// ============================================================
//  Parser descendente recursivo para TaskScript
//
//  Gramatica implementada:
//  <programa>  ::= TABLERO CADENA '{' <columnas> '}' ';'
//  <columnas>  ::= <columna> <columnas> | <columna>
//  <columna>   ::= COLUMNA CADENA '{' <tareas> '}' ';'
//  <tareas>    ::= <tarea> ',' <tareas> | <tarea>
//  <tarea>     ::= tarea ':' CADENA '[' <atributos> ']'
//  <atributos> ::= <atributo> ',' <atributos> | <atributo>
//  <atributo>  ::= prioridad ':' <prioridad>
//               |  responsable ':' CADENA
//               |  fecha_limite ':' FECHA
//  <prioridad> ::= ALTA | MEDIA | BAJA
// ============================================================
class SyntaxAnalyzer {
public:
    SyntaxAnalyzer(const std::vector<Token>& tokens, ErrorManager& errMgr);

    // Punto de entrada -- retorna el nodo raiz del arbol o nullptr si falla
    NodoArbol* parse();

private:
 
    // Analisis lexico -> tokens
    // Analisis sintactico -> arbol de derivacion (NodoArbol) pero recibe los tokens del analis lexico

    const std::vector<Token>& tokens_;
    ErrorManager&             errMgr_;
    size_t                    pos_;

    // --- Primitivas del parser ---
    const Token& actual() const;
    const Token& ver(int offset = 0) const;
    Token        consumir(TokenType esperado, const std::string& contexto);
    // match verifica el tipo del token actual sin consumirlo, retorna true si coincide
    // TABLERO "Mi Tablero" { ... } -> match(TABLERO) en el primer token, match(CADENA) en el segundo, etc.
    bool         match(TokenType t) const;
    void         sincronizar(const std::vector<TokenType>& seguimiento);

    // --- Una funcion por produccion de la gramatica ---
    NodoArbol* parsePrograma();
    NodoArbol* parseColumnas();
    NodoArbol* parseColumna();
    NodoArbol* parseTareas();
    NodoArbol* parseTarea();
    NodoArbol* parseAtributos();
    NodoArbol* parseAtributo();
    NodoArbol* parsePrioridad();
    
    // Crea un nodo hoja (terminal)
    NodoArbol* hoja(const Token& t);
};