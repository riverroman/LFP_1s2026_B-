#include "SyntaxAnalyzer.h"
#include <stdexcept>

SyntaxAnalyzer::SyntaxAnalyzer(const std::vector<Token>& tokens, ErrorManager& errMgr)
    : tokens_(tokens), errMgr_(errMgr), pos_(0) {}

// ----------------------------------------------------------------
// Primitivas
// ----------------------------------------------------------------
const Token& SyntaxAnalyzer::actual() const {
    return tokens_[pos_];
}

const Token& SyntaxAnalyzer::ver(int offset) const {
    size_t idx = pos_ + static_cast<size_t>(offset);
    if (idx >= tokens_.size()) return tokens_.back(); // FIN_ARCHIVO
    return tokens_[idx];
}

bool SyntaxAnalyzer::match(TokenType t) const {
    return actual().type == t;
}

// Consume el token actual si es del tipo esperado.
// Si no, registra error y retorna un token dummy para continuar (recuperacion).
Token SyntaxAnalyzer::consumir(TokenType esperado, const std::string& contexto) {
    if (match(esperado)) {
        Token t = actual();
        pos_++;
        return t;
    }
    // Error sintactico: se esperaba 'esperado' pero se encontro otro
    std::string desc = "Se esperaba '" + tokenTypeToString(esperado) +
                       "' en " + contexto +
                       ", se encontro '" + actual().lexema + "'";
    errMgr_.agregarError(TipoError::SINTACTICO, actual().lexema, desc,
                          actual().linea, actual().columna);
    // NO avanzamos: el token sigue disponible para la recuperacion
    return Token(esperado, "", actual().linea, actual().columna);
}

// Recuperacion de errores: avanza hasta encontrar un token del conjunto
// de seguimiento (tokens que pueden iniciar la proxima construccion valida)
void SyntaxAnalyzer::sincronizar(const std::vector<TokenType>& seguimiento) {
    while (!match(TokenType::FIN_ARCHIVO)) {
        for (auto t : seguimiento) {
            if (match(t)) return;
        }
        pos_++;
    }
}

NodoArbol* SyntaxAnalyzer::hoja(const Token& t) {
    return new NodoArbol(t.lexema, true);
}

// ----------------------------------------------------------------
// <programa> ::= TABLERO CADENA '{' <columnas> '}' ';'
// ----------------------------------------------------------------
NodoArbol* SyntaxAnalyzer::parsePrograma() {

    // programa es el nodo raiz del arbol, no es terminal (no es un token)
    // hijo 1: token TABLERO
    // hijo 2: token CADENA (nombre del tablero)
    // hijo 3: token LLAVE_ABRE
    // hijo 4: nodo <columnas>
    // hijo 5: token LLAVE_CIERRA
    // hijo 6: token PUNTO_COMA

    NodoArbol* nodo = new NodoArbol("<programa>", false);

    nodo->hijos.push_back(hoja(consumir(TokenType::TABLERO, "<programa>")));
    nodo->hijos.push_back(hoja(consumir(TokenType::CADENA,  "<programa> - nombre tablero")));
    nodo->hijos.push_back(hoja(consumir(TokenType::LLAVE_ABRE, "<programa>")));

    if (match(TokenType::COLUMNA)) {
        nodo->hijos.push_back(parseColumnas());
    } else {
        errMgr_.agregarError(TipoError::SINTACTICO, actual().lexema,
            "Se esperaba al menos una COLUMNA dentro del TABLERO",
            actual().linea, actual().columna);
    }

    nodo->hijos.push_back(hoja(consumir(TokenType::LLAVE_CIERRA, "<programa>")));
    nodo->hijos.push_back(hoja(consumir(TokenType::PUNTO_COMA,   "<programa>")));

    return nodo;
}

// ----------------------------------------------------------------
// <columnas> ::= <columna> <columnas> | <columna>
// ----------------------------------------------------------------
NodoArbol* SyntaxAnalyzer::parseColumnas() {
    
}

// ----------------------------------------------------------------
// <columna> ::= COLUMNA CADENA '{' <tareas> '}' ';'
// ----------------------------------------------------------------
NodoArbol* SyntaxAnalyzer::parseColumna() {
    
}

// ----------------------------------------------------------------
// <tareas> ::= <tarea> ',' <tareas> | <tarea>
// ----------------------------------------------------------------
NodoArbol* SyntaxAnalyzer::parseTareas() {
   
}

// ----------------------------------------------------------------
// <tarea> ::= tarea ':' CADENA '[' <atributos> ']'
// ----------------------------------------------------------------
NodoArbol* SyntaxAnalyzer::parseTarea() {
    
}

// ----------------------------------------------------------------
// <atributos> ::= <atributo> ',' <atributos> | <atributo>
// ----------------------------------------------------------------
NodoArbol* SyntaxAnalyzer::parseAtributos() {
    
}

// ----------------------------------------------------------------
// <atributo> ::= prioridad ':' <prioridad>
//             |  responsable ':' CADENA
//             |  fecha_limite ':' FECHA
// ----------------------------------------------------------------
NodoArbol* SyntaxAnalyzer::parseAtributo() {
   
}

// ----------------------------------------------------------------
// <prioridad> ::= ALTA | MEDIA | BAJA
// ----------------------------------------------------------------
NodoArbol* SyntaxAnalyzer::parsePrioridad() {
    
}

// ----------------------------------------------------------------
// Punto de entrada publico
// ----------------------------------------------------------------
NodoArbol* SyntaxAnalyzer::parse() {
    if (tokens_.empty()) return nullptr;
    return parsePrograma();
}