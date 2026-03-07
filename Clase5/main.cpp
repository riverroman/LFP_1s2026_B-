#include <cctype>   // Libreria para funciones como isspace;
#include <iostream> // Libreria para entraada y salida de datos
#include <string>   // Libreria para usar cadenas de texto
#include <vector>   // Libreria para usar vectores

using namespace std;

enum TipoToken {
  IDENTIFICADOR,
  NUMERO,
  RESERVADA,
  OPERADOR,
  PARENTESIS,
  PUNTOYCOMA,
  DESCONOCIDO // Manejar los errores lexicos
};

// @ -> desconocido

struct Token {
  TipoToken tipo;
  string valor;
};

bool esReservada(const string &palabra) {
  return palabra == "int" || palabra == "if" || palabra == "else" ||
         palabra == "while";
}

vector<Token> analizar(const string &entrada) {
  vector<Token> tokens;

  int i = 0; // Indice de la entrada que recorrera caracter por caracter

  while (i < entrada.length()) {
    // Elimine los espacios en blanco
    if (isspace(entrada[i])) { // Si el caracter actual es un espacio en blanco
      i++;                     // Se avanza al siguiente caracter
      continue;                // Ignorar espacios en blanco
    }
    // Si el caracter es una letra (inicio de identificador o palabra
    // reservada)

    if (isalpha(entrada[i])) {
      string
          buffer; // Buffer para almacenar el identificador o palabra reservada

      while (isalnum(entrada[i])) {
        buffer += entrada[i]; // Agregar el caracter al buffer
        i++;
      }

      if (esReservada(buffer)) {
        tokens.push_back({RESERVADA, buffer}); // Agregar token de palabra
                                               // reservada
      } else {
        tokens.push_back({IDENTIFICADOR, buffer}); // Agregar token de
                                                   // identificador
      }
      continue;
      // reservada  identificador
      // let      edad       = 0
    }

    if (isdigit(entrada[i])) {
      string buffer; // Buffer para almacenar el numero

      while (isdigit(entrada[i])) {
        buffer += entrada[i]; // Agregar el caracter al buffer
        i++;
      }

      tokens.push_back({NUMERO, buffer}); // Agregar token de numero
      continue;
    }

    // Se dectecta operador doble ==
    // ==
    if (entrada[i] == '=' && entrada[i + 1] == '=') {
      tokens.push_back({OPERADOR, "=="});
      continue;
    }

    // Operadores
    //    [i]
    // 4  +  4
    if (string("+-*/=").find(entrada[i]) !=
        string::npos) { // Si es un operador simple
      tokens.push_back({OPERADOR, string(1, entrada[i])});
      i++;
      continue;
    }

    if (entrada[i] == '(' || entrada[i] == ')') {
      tokens.push_back({PARENTESIS, string(1, entrada[i])});
      i++;
      continue;
    }
    //                                  [i]
    //  system.out.println("Hola Mundo");
    if (entrada[i] == ';') {
      tokens.push_back({PUNTOYCOMA, ";"});
      i++;
      continue;
    }

    tokens.push_back({DESCONOCIDO, string(1, entrada[i])}); // Agregar token
                                                            // desconocido
    i++;
  }
  return tokens;
}

string tipoToString(TipoToken tipo) {
  switch (tipo) {
  case IDENTIFICADOR:
    return "IDENTIFICADOR";
  case NUMERO:
    return "NUMERO";
  case RESERVADA:
    return "RESERVADA";
  case OPERADOR:
    return "OPERADOR";
  case PARENTESIS:
    return "PARENTESIS";
  case PUNTOYCOMA:
    return "PUNTOYCOMA";
  default:
    return "DESCONOCIDO";
  }
}

int main() {
  string codigo = "int x = 10; if (x == 10) { x = x + 1; }";
  vector<Token> tokens = analizar(codigo);
  for (const auto &token : tokens) {
    cout << tipoToString(token.tipo) << ": " << token.valor << endl;
  }
  return 0;
}
