#pragma once
#include <string>

// ============================================================
//  Tipos de tokens del lenguaje TaskScript
//  Cada valor corresponde a un estado de aceptacion del AFD
// ============================================================
enum class TokenType {
    // Palabras reservadas
    TABLERO,
    COLUMNA,
    TAREA,
    PRIORIDAD,
    RESPONSABLE,
    FECHA_LIMITE,

    // Enumeraciones de prioridad
    ALTA,
    MEDIA,
    BAJA,

    // Literales
    CADENA,       // "texto entre comillas"
    FECHA,        // AAAA-MM-DD
    ENTERO,       // 123

    // Delimitadores
    LLAVE_ABRE,   // {
    LLAVE_CIERRA, // }
    CORCHETE_ABRE,   // [
    CORCHETE_CIERRA, // ]
    DOS_PUNTOS,   // :
    COMA,         // ,
    PUNTO_COMA,   // ;

    // Especiales
    FIN_ARCHIVO,
    DESCONOCIDO   // token invalido (error lexico)
};

// Convierte TokenType a string legible (util para reportes)
inline std::string tokenTypeToString(TokenType token) {
    switch (token) {
        case TokenType::TABLERO:          return "TABLERO";
        case TokenType::COLUMNA:          return "COLUMNA";
        case TokenType::TAREA:            return "TAREA";
        case TokenType::PRIORIDAD:        return "PRIORIDAD";
        case TokenType::RESPONSABLE:      return "RESPONSABLE";
        case TokenType::FECHA_LIMITE:     return "FECHA_LIMITE";
        case TokenType::ALTA:             return "ALTA";
        case TokenType::MEDIA:            return "MEDIA";
        case TokenType::BAJA:             return "BAJA";
        case TokenType::CADENA:           return "CADENA";
        case TokenType::FECHA:            return "FECHA";
        case TokenType::ENTERO:           return "ENTERO";
        case TokenType::LLAVE_ABRE:       return "LLAVE_ABRE";
        case TokenType::LLAVE_CIERRA:     return "LLAVE_CIERRA";
        case TokenType::CORCHETE_ABRE:    return "CORCHETE_ABRE";
        case TokenType::CORCHETE_CIERRA:  return "CORCHETE_CIERRA";
        case TokenType::DOS_PUNTOS:       return "DOS_PUNTOS";
        case TokenType::COMA:             return "COMA";
        case TokenType::PUNTO_COMA:       return "PUNTO_COMA";
        case TokenType::FIN_ARCHIVO:      return "FIN_ARCHIVO";
        case TokenType::DESCONOCIDO:      return "DESCONOCIDO";
        default:                           return "?";
    }
}

struct Token {
    TokenType   type;
    std::string lexema;
    int         linea;
    int         columna;

    Token(TokenType t, std::string l, int ln, int col)
        : type(t), lexema(std::move(l)), linea(ln), columna(col) {}
};