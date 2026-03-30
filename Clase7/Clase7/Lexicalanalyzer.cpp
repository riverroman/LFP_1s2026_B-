#include "Lexicalanalyzer.h"
#include <cctype>

LexicalAnalyzer::LexicalAnalyzer(std::string text){
    input = text;
    pos = 0;
    line = 1;
    column = 1;
}

char LexicalAnalyzer::peek(){
    if((size_t)pos >= input.size()) return '\0';
    return input[pos];
}

char LexicalAnalyzer::advance(){
    char c = input[pos++];
    if(c == '\n'){ line++; column = 1; }
    else { column++; }
    return c;
}

// Clasifica el lexema como RESERVED, ENUM_VAL o IDENTIFIER
static TokenType classifyWord(const std::string& w) {
    // Palabras reservadas
    if (w=="HOSPITAL"||w=="PACIENTES"||w=="MEDICOS"||
        w=="CITAS"||w=="DIAGNOSTICOS"||
        w=="paciente"||w=="medico"||w=="cita"||
        w=="diagnostico"||w=="con")
        return TokenType::RESERVED;
    // Enumeraciones
    if (w=="CARDIOLOGIA"||w=="NEUROLOGIA"||w=="PEDIATRIA"||
        w=="CIRUGIA"||w=="MEDICINA_GENERAL"||w=="ONCOLOGIA"||
        w=="DIARIA"||w=="CADA_8_HORAS"||w=="CADA_12_HORAS"||w=="SEMANAL")
        return TokenType::ENUM_VAL;
    return TokenType::IDENTIFIER;
}

void LexicalAnalyzer::analyze() {
    int state = 0;
    std::string lexeme = "";

    while (true) {
        char c = peek();

        switch (state) {

        // ── q0: Estado inicial ──────────────────────────────
        case 0:
            if (isspace(c)) { advance(); break; }

            if (c == '\0') return;

            // Letras → identificador / reservada / enum / ID_CODE
            if (isalpha(c) || c == '_') {
                lexeme += advance();
                state = 1;
                break;
            }

            // Dígitos → número / fecha / hora
            if (isdigit(c)) {
                lexeme += advance();
                state = 2;
                break;
            }

            // String
            if (c == '"') {
                lexeme += advance();
                state = 3;
                break;
            }

            // Símbolos simples reconocidos
            if (c==':'||c=='['||c==']'||c=='{'||c=='}'||c==','||c==';') {
                lexeme += advance();
                tokens.push_back(Token(TokenType::SYMBOL, lexeme, line, column));
                lexeme = "";
                break;
            }

            // Carácter no reconocido
            lexeme += advance();
            errors.push_back(LexicalError(lexeme, line, column, "Caracter no reconocido"));
            lexeme = "";
            break;

        // ── q1: Leyendo palabra (IDENTIFIER / RESERVED / ENUM / ID_CODE) ──
        case 1:
            if (isalnum(c) || c == '_') {
                lexeme += advance();
            } else if (c == '-') {
                // Puede ser ID_CODE tipo MED-001 o PAC-001
                lexeme += advance();
                state = 20; // estado ID_CODE dígitos
            } else {
                // Fin de palabra — clasificar
                tokens.push_back(Token(classifyWord(lexeme), lexeme, line, column));
                lexeme = "";
                state = 0;
            }
            break;

        // ── q2: Leyendo número / inicio de fecha ───────────────
        case 2:
            if (isdigit(c)) {
                lexeme += advance();
            } else if (c == '-' && lexeme.size() == 4) {
                // Parece fecha: AAAA-
                lexeme += advance();
                state = 10; // leer MM
            } else if (c == ':' && lexeme.size() <= 2) {
                // Parece hora: HH:
                lexeme += advance();
                state = 15; // leer MM de hora
            } else {
                tokens.push_back(Token(TokenType::NUMBER, lexeme, line, column));
                lexeme = "";
                state = 0;
            }
            break;

        // ── q3: Leyendo string ──────────────────────────────────
        case 3:
            if (c == '"') {
                lexeme += advance();
                tokens.push_back(Token(TokenType::STRING, lexeme, line, column));
                lexeme = "";
                state = 0;
            } else if (c == '\0') {
                errors.push_back(LexicalError(lexeme, line, column, "Cadena no cerrada"));
                return;
            } else {
                lexeme += advance();
            }
            break;

        // ── Estados fecha: AAAA-MM-DD ───────────────────────────
        // q10: leyendo MM (mes)
        case 10:
            if (isdigit(c)) {
                lexeme += advance();
            } else if (c == '-' && lexeme.size() >= 7) {
                // Tenemos AAAA-MM, ahora el -
                lexeme += advance();
                state = 11; // leer DD
            } else {
                errors.push_back(LexicalError(lexeme, line, column, "Fecha inválida, formato esperado: AAAA-MM-DD"));
                lexeme = "";
                state = 0;
            }
            break;

        // q11: leyendo DD (día)
        case 11:
            if (isdigit(c)) {
                lexeme += advance();
            } else {
                // Validar mes y día básico
                // lexeme tiene forma "2025-04-10"
                tokens.push_back(Token(TokenType::DATE, lexeme, line, column));
                lexeme = "";
                state = 0;
            }
            break;

        // ── Estados hora: HH:MM ─────────────────────────────────
        // q15: leyendo MM de hora
        case 15:
            if (isdigit(c)) {
                lexeme += advance();
            } else {
                // Tenemos HH:MM completo
                tokens.push_back(Token(TokenType::TIME, lexeme, line, column));
                lexeme = "";
                state = 0;
            }
            break;

        // ── Estados ID_CODE: MED-001 / PAC-001 ─────────────────
        // q20: leyendo dígitos después del guión
        case 20:
            if (isdigit(c)) {
                lexeme += advance();
            } else {
                tokens.push_back(Token(TokenType::ID_CODE, lexeme, line, column));
                lexeme = "";
                state = 0;
            }
            break;

        } // fin switch
    } // fin while
}