#include "Token.h"

Token::Token(TokenType type, std::string lexeme, int line, int column){
    this->type = type;
    this->lexeme = lexeme;
    this->line = line;
    this->column = column;
}

// Método para obtener el tipo del token como una cadena legible
std::string Token::getTypeString() const {
    switch (type) {
        // Agrega casos para cada tipo de token según tu enumeración
        // Si encuentro un identificador, número, cadena, fecha, hora, código de identificación o símbolo, retorno su nombre como string
        case TokenType::IDENTIFIER: return "IDENTIFIER";
        case TokenType::NUMBER: return "NUMBER";
        case TokenType::STRING: return "STRING";
        case TokenType::DATE: return "DATE";
        case TokenType::TIME: return "TIME";
        case TokenType::ID_CODE: return "ID_CODE";
        case TokenType::SYMBOL: return "SYMBOL";
        default: return "UNKNOWN";
    }
}

