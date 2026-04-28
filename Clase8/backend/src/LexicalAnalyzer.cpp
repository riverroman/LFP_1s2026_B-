#include "LexicalAnalyzer.h"
#include <stdexcept>

// ============================================================
//  DIAGRAMA DEL AFD (estados relevantes):
//
//  q0  = estado inicial (decision)
//  q1  = leyendo identificador/palabra reservada
//  q2  = leyendo cadena (dentro de comillas)
//  q3  = leyendo digitos (podria ser ENTERO o FECHA)
//  q4  = leyendo digitos de FECHA parte 2 (despues de primer -)
//  q5  = leyendo digitos de FECHA parte 3 (despues de segundo -)
//  q_accept_* = estados de aceptacion (retornan token)
//  q_err = estado de error lexico
//
//  Transiciones principales desde q0:
//    letra       -> q1 (identificador)
//    '"'         -> q2 (cadena)
//    digito      -> q3 (numero/fecha)
//    '{' '}' '[' ']' ':' ',' ';' -> q_accept directamente
//    espacio/tab/\n -> volver a q0 (ignorar)
//    '/' '/'     -> ignorar linea (comentario)
//    cualquier otro -> q_err
// ============================================================

LexicalAnalyzer::LexicalAnalyzer(const std::string& source)
    : src_(source), pos_(0), linea_(1), columna_(1) {}

// ----------------------------------------------------------------
// Lectura de caracter con seguimiento de posicion
// ----------------------------------------------------------------
char LexicalAnalyzer::peek(int offset) const {
    size_t idx = pos_ + static_cast<size_t>(offset);
    if (idx >= src_.size()) return '\0';
    return src_[idx];
}
// ===================================================================
// # comentarios y funciones auxiliares para el AFD
// if(c == "#"){
//      advance(); // consume '#'
// }
//=====================================================================
char LexicalAnalyzer::advance() {
    char c = src_[pos_++];
    if (c == '\n') { linea_++; columna_ = 1; }
    else           { columna_++; }
    return c;
}

// ----------------------------------------------------------------
// Clasificadores de caracteres -- SIN isalpha/isdigit
// Se usan rangos ASCII directamente (requisito del proyecto)
// ----------------------------------------------------------------
bool LexicalAnalyzer::esLetra(char c) {
    // a-z o A-Z o guion bajo
    return (c >= 'a' && c <= 'z') ||
           (c >= 'A' && c <= 'Z') ||
           (c == '_');
}

bool LexicalAnalyzer::esDigito(char c) {
    return (c >= '0' && c <= '9');
}

bool LexicalAnalyzer::esLetraODigito(char c) {
    return esLetra(c) || esDigito(c);
}

// ----------------------------------------------------------------
// Saltar espacios en blanco y comentarios de linea (//)
// ----------------------------------------------------------------
void LexicalAnalyzer::skipWhitespaceAndComments() {
    while (pos_ < src_.size()) {
        char c = peek();
        // Espacios en blanco
        if (c == ' ' || c == '\t' || c == '\r' || c == '\n') {
            advance();
        }
        // Comentario de linea //
        else if (c == '/' && peek(1) == '/') {
            while (pos_ < src_.size() && peek() != '\n') advance();
        }
        else {
            break;
        }
    }
}

// ----------------------------------------------------------------
// ESTADO q1: leer identificador o palabra reservada
// Acepta: [letra|_] ([letra|_|digito])*
// ----------------------------------------------------------------
Token LexicalAnalyzer::leerIdentificadorOPalabraReservada(int linInicio, int colInicio) {
    std::string lexema;
    // El primer caracter ya fue inspeccionado (es letra o _)
    while (pos_ < src_.size() && esLetraODigito(peek())) {
        lexema += advance();
    }
    TokenType tipo = palabraReservada(lexema);
    return Token(tipo, lexema, linInicio, colInicio);
}

// ----------------------------------------------------------------
// ESTADO q2: leer cadena de texto entre comillas dobles
// Acepta: '"' [cualquier caracter excepto '"' y \n]* '"'
// ----------------------------------------------------------------
Token LexicalAnalyzer::leerCadena(int linInicio, int colInicio) {
    advance(); // consume '"' de apertura
    std::string lexema = "\"";
    while (pos_ < src_.size()) {
        char c = peek();
        if (c == '"') {
            lexema += advance(); // consume '"' de cierre
            return Token(TokenType::CADENA, lexema, linInicio, colInicio);
        }
        if (c == '\n' || c == '\0') {
            // Error: cadena no cerrada antes de fin de linea
            return Token(TokenType::DESCONOCIDO,
                         lexema + " [sin cerrar]", linInicio, colInicio);
        }
        lexema += advance();
    }
    // Llegamos al EOF sin cerrar
    return Token(TokenType::DESCONOCIDO,
                 lexema + " [sin cerrar]", linInicio, colInicio);
}

// ----------------------------------------------------------------
// ESTADO q3/q4/q5: leer entero o fecha (AAAA-MM-DD)
//
// AFD de fecha:
//   q3: [digito]+ -> si sigue '-' y tenemos 4 digitos -> q4
//                    si no                            -> q_accept ENTERO
//   q4: [digito][digito] '-'                          -> q5
//   q5: [digito][digito]                              -> q_accept FECHA
//
// Nota: si el patron no completa la fecha, se retorna ENTERO
//       con los digitos leidos hasta ese punto.
// ----------------------------------------------------------------
Token LexicalAnalyzer::leerNumeroOFecha(int linInicio, int colInicio) {
    std::string lexema;

    // ---- q3: leer digitos iniciales ----
    while (pos_ < src_.size() && esDigito(peek())) {
        lexema += advance();
    }

    // ¿Podria ser fecha? Necesitamos exactamente 4 digitos seguidos de '-'
    if (lexema.size() == 4 && peek() == '-') {
        std::string tentativa = lexema;
        tentativa += advance(); // consume '-'

        // ---- q4: leer 2 digitos del mes ----
        int contMes = 0;
        while (pos_ < src_.size() && esDigito(peek()) && contMes < 2) {
            tentativa += advance();
            contMes++;
        }

        if (contMes == 2 && peek() == '-') {
            tentativa += advance(); // consume segundo '-'

            // ---- q5: leer 2 digitos del dia ----
            int contDia = 0;
            while (pos_ < src_.size() && esDigito(peek()) && contDia < 2) {
                tentativa += advance();
                contDia++;
            }

            if (contDia == 2 && !esDigito(peek())) {
                // q_accept FECHA: patron AAAA-MM-DD completo
                return Token(TokenType::FECHA, tentativa, linInicio, colInicio);
            }
            // Si no se completo, caemos a ENTERO con solo los primeros digitos
            // (los caracteres ya consumidos quedan como perdidos -- simplificacion
            //  para la base; el estudiante puede mejorar esto)
        }
        // Si la parte del mes no es valida, retornamos solo el entero de 4 digitos
        // Los caracteres extra ya fueron consumidos -- esta es la parte a mejorar
    }

    return Token(TokenType::ENTERO, lexema, linInicio, colInicio);
}

// ----------------------------------------------------------------
// Tabla de palabras reservadas
// Retorna el TokenType si es reservada, o DESCONOCIDO si no lo es
// (los identificadores generales no existen en TaskScript -- todo
//  es una palabra reservada o un literal)
// ----------------------------------------------------------------
TokenType LexicalAnalyzer::palabraReservada(const std::string& lex) {
    if (lex == "TABLERO")      return TokenType::TABLERO;
    if (lex == "COLUMNA")      return TokenType::COLUMNA;
    if (lex == "tarea")        return TokenType::TAREA;
    if (lex == "prioridad")    return TokenType::PRIORIDAD;
    if (lex == "responsable")  return TokenType::RESPONSABLE;
    if (lex == "fecha_limite") return TokenType::FECHA_LIMITE;
    if (lex == "ALTA")         return TokenType::ALTA;
    if (lex == "MEDIA")        return TokenType::MEDIA;
    if (lex == "BAJA")         return TokenType::BAJA;
    // Identificador no reconocido en este lenguaje -> error lexico
    return TokenType::DESCONOCIDO;
}

// ----------------------------------------------------------------
// nextToken() -- funcion principal del AFD
// Implementa la funcion de transicion de estados delta(q, c)
// ----------------------------------------------------------------
Token LexicalAnalyzer::nextToken() {
    skipWhitespaceAndComments();

    if (pos_ >= src_.size()) {
        return Token(TokenType::FIN_ARCHIVO, "", linea_, columna_);
    }

    int linInicio = linea_;
    int colInicio = columna_;
    char c = peek();

    // ---- Transiciones desde q0 ----

    // q0 --letra--> q1
    if (esLetra(c)) {
        return leerIdentificadorOPalabraReservada(linInicio, colInicio);
    }

    // q0 --'"'--> q2
    if (c == '"') {
        return leerCadena(linInicio, colInicio);
    }

    // q0 --digito--> q3
    if (esDigito(c)) {
        return leerNumeroOFecha(linInicio, colInicio);
    }

    // q0 --delimitador--> q_accept (aceptacion directa de un caracter)
    advance(); // consume el caracter
    switch (c) {
        case '{': return Token(TokenType::LLAVE_ABRE,       "{", linInicio, colInicio);
        case '}': return Token(TokenType::LLAVE_CIERRA,     "}", linInicio, colInicio);
        case '[': return Token(TokenType::CORCHETE_ABRE,    "[", linInicio, colInicio);
        case ']': return Token(TokenType::CORCHETE_CIERRA,  "]", linInicio, colInicio);
        case ':': return Token(TokenType::DOS_PUNTOS,       ":", linInicio, colInicio);
        case ',': return Token(TokenType::COMA,             ",", linInicio, colInicio);
        case ';': return Token(TokenType::PUNTO_COMA,       ";", linInicio, colInicio);
        default:  break;
    }

    // q0 --otro--> q_err  (caracter no reconocido)
    std::string lex(1, c);
    return Token(TokenType::DESCONOCIDO, lex, linInicio, colInicio);
}

// ----------------------------------------------------------------
// tokenizeAll() -- conveniencia: consume todo el fuente
// ----------------------------------------------------------------
std::vector<Token> LexicalAnalyzer::tokenizeAll() {
    std::vector<Token> tokens;
    while (true) {
        Token t = nextToken();
        tokens.push_back(t);
        if (t.type == TokenType::FIN_ARCHIVO) break;
    }
    return tokens;
}