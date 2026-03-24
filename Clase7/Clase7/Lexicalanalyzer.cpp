#include "Lexicalanalyzer.h"
#include <cctype> // para isalpha, isdigit, etc.
//   |
//   v
//   kfadjkfjadkfjakld
LexicalAnalyzer::LexicalAnalyzer(std::string text){
    input = text;
    pos = 0;
    line = 1;
    column = 1;
}

// El método analyze es el corazón del analizador léxico, donde se realiza el proceso de tokenización
char LexicalAnalyzer::peek(){
    // Verificamos si hemos llegado al final de la entrada de texto
    if((size_t)pos >= input.size()){
        return '\0'; // fin de la entrada
    }
    // Retorna el caracter actual sin avanzar el puntero
    return input[pos];
}

char LexicalAnalyzer::advance(){
    char c = input[pos++]; // Obtiene el caracter actual y luego avanza el puntero

    if(c == '\n'){
        line++;
        column = 1; // Reinicia la columna al inicio de la nueva línea
    }else{
        column++;
    }
    return c; // Retorna el caracter actual
}

void LexicalAnalyzer::analyze() {

    // Estado inicial
    int state = 0;
    std::string lexeme = "";
    // Bucle principal del autómata
    while (true) {
        // Obtener el siguiente carácter sin avanzar
        // peek() es una función que devuelve el carácter actual sin mover el puntero de lectura
        char c = peek();

        // Lógica del autómata basada en el estado actual
        switch (state) {

        // q0 estado inicial
        case 0:
            // Ignorar espacios en blanco
            if (isspace(c)) {
                advance();
                continue;
            }
            // Identificar el tipo de token basado en el primer carácter
            if (isalpha(c)) {
                // Si es una letra, comenzamos a construir un identificador
                lexeme += advance();
                // Cambiamos al estado 1 para continuar leyendo el identificador
                state = 1;
                // Salimos del switch para continuar con el siguiente carácter
                break;
            }

            // Si es un dígito, comenzamos a construir un número
            if (isdigit(c)) {
                // Agregamos el dígito al lexema y cambiamos al estado 2 para continuar leyendo el número
                lexeme += advance();
                // Cambiamos al estado 2 para continuar leyendo el número
                state = 2;
                // Salimos del switch para continuar con el siguiente carácter
                break;
            }

            // Si es una comilla, comenzamos a construir una cadena
            if (c == '"') {
                // Agregamos la comilla al lexema y cambiamos al estado 3 para continuar leyendo la cadena
                lexeme += advance();
                // Cambiamos al estado 3 para continuar leyendo la cadena
                state = 3;
                break;
            }

            // Si es un símbolo reconocido, lo agregamos como token
            if (c == ':' || c == '[' || c == ']') {
                // Agregamos el símbolo al lexema y lo guardamos como token
                lexeme += advance();
                // Agregamos el token a la lista de tokens con su tipo, lexema, línea y columna
                tokens.push_back(
                    Token(TokenType::SYMBOL, lexeme, line, column)
                    );
                // Reiniciamos el lexema y volvemos al estado inicial
                lexeme = "";
                break;
            }
            // Si el carácter no es reconocido y no es el fin del input, lo tratamos como un error
            if (c == '\0')
                return;
            // Si el carácter no es reconocido, lo tratamos como un error
            // error
            lexeme += advance();

            // Agregamos el error a la lista de errores con su lexema, línea, columna y mensaje descriptivo
            errors.push_back(
                LexicalError(lexeme, line, column, "Caracter no reconocido")
                );
            // Reiniciamos el lexema para el próximo token o error
            lexeme = "";

            break;

        // identificador
        case 1:
            // Si el siguiente carácter es alfanumérico, lo agregamos al lexema del identificador
            if (isalnum(c)) {
                lexeme += advance();
            } else {
                // Si el siguiente carácter no es alfanumérico, finalizamos el token de identificador y lo agregamos a la lista de tokens con su tipo, lexema, línea y columna
                tokens.push_back(
                    Token(TokenType::IDENTIFIER, lexeme, line, column)
                    );

                lexeme = "";
                state = 0;
            }

            break;

        // número
        case 2:
            // Si el siguiente carácter es un dígito, lo agregamos al lexema del número
            if (isdigit(c)) {
                lexeme += advance();
            } else {
                // Si el siguiente carácter no es un dígito, finalizamos el token de número y lo agregamos a la lista de tokens con su tipo, lexema, línea y columna
                tokens.push_back(
                    Token(TokenType::NUMBER, lexeme, line, column)
                    );
                // Reiniciamos el lexema y volvemos al estado inicial para procesar el siguiente token
                lexeme = "";
                state = 0;
            }
            break;
        // string
        case 3:
            // Si el siguiente carácter es una comilla, finalizamos el token de cadena y lo agregamos a la lista de tokens con su tipo, lexema, línea y columna
            if (c == '"') {
                lexeme += advance();
                // Agregamos el token de cadena a la lista de tokens con su tipo, lexema, línea y columna
                tokens.push_back(
                    Token(TokenType::STRING, lexeme, line, column)
                    );

                // Reiniciamos el lexema y volvemos al estado inicial para procesar el siguiente token
                lexeme = "";
                // Volvemos al estado inicial para procesar el siguiente token
                // Se reinicia el estado a 0 para procesar el siguiente token o error
                state = 0;
            } else if (c == '\0') {
                // Si llegamos al final del input sin cerrar la cadena, registramos un error de cadena no cerrada con su lexema, línea, columna y mensaje descriptivo
                errors.push_back(
                    LexicalError(lexeme, line, column, "Cadena no cerrada")
                    );
                return;
            } else {
                // Si el siguiente carácter no es una comilla ni el fin del input, lo agregamos al lexema de la cadena y continuamos leyendo
                lexeme += advance();
            }
            // Continuamos en el estado 3 para seguir leyendo la cadena hasta encontrar la comilla de cierre o el fin del input
            break;
        }
    }
}