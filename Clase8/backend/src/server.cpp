//  server.cpp  -- Servidor HTTP minimalista para TaskScript
//  Expone: POST /analyze  con body { "source": "..." }
//  Retorna JSON con tokens, errores y codigo DOT del arbol
//
//  Compilacion:
//    g++ -std=c++17 -O2 -o taskscript_server \
//        server.cpp LexicalAnalyzer.cpp SyntaxAnalyzer.cpp ErrorManager.cpp
//
//  NOTA: No usa librerias HTTP externas -- usa sockets POSIX directamente.
//        Para produccion se recomienda usar cpp-httplib (header-only).

#include "LexicalAnalyzer.h"
#include "SyntaxAnalyzer.h"
#include "ErrorManager.h"

#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <cstring>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

// ----------------------------------------------------------------
// Utilidades JSON minimas (sin dependencias)
// ----------------------------------------------------------------
static std::string escJson(const std::string& s) {
    std::string out;
    for (char c : s) {
        switch (c) {
            case '"':  out += "\\\""; break;
            case '\\': out += "\\\\"; break;
            case '\n': out += "\\n";  break;
            case '\r': out += "\\r";  break;
            case '\t': out += "\\t";  break;
            default:   out += c;
        }
    }
    return out;
}

// ----------------------------------------------------------------
// Generador de codigo DOT (arbol de derivacion) -- ESQUELETO
// El estudiante debe completar la generacion recursiva
// ----------------------------------------------------------------
static int dotCounter = 0;

static void dotNodo(std::ostringstream& out, const NodoArbol* nodo, int padre) {
    int id = ++dotCounter;
    std::string color = nodo->esTerminal ? "#D6EAF8" : "#2E75B6";
    std::string fcolor = nodo->esTerminal ? "black" : "white";
    out << "  n" << id << " [label=\"" << escJson(nodo->etiqueta)
        << "\", fillcolor=\"" << color
        << "\", fontcolor=\"" << fcolor << "\"];\n";
    if (padre >= 0) {
        out << "  n" << padre << " -> n" << id << ";\n";
    }
    for (auto* hijo : nodo->hijos) {
        dotNodo(out, hijo, id);
    }
}

static std::string generarDot(const NodoArbol* raiz) {
    if (!raiz) return "";
    dotCounter = 0;
    std::ostringstream out;
    out << "digraph ArbolDerivacion {\n";
    out << "  rankdir=TB;\n";
    out << "  node [shape=box, style=filled, fontname=\"Arial\"];\n";
    dotNodo(out, raiz, -1);
    out << "}\n";
    return out.str();
}

// ----------------------------------------------------------------
// Construye la respuesta JSON completa
// ----------------------------------------------------------------
static std::string analizarYResponder(const std::string& source) {
    ErrorManager errMgr;


   /// ==== 1. USO DEL ANALISIS LEXICO

    // 1. Analisis lexico
    LexicalAnalyzer lexer(source);
    std::vector<Token> tokens = lexer.tokenizeAll();

    // Registrar errores lexicos
    for (auto& t : tokens) {
        if (t.type == TokenType::DESCONOCIDO) {
            errMgr.agregarError(TipoError::LEXICO, t.lexema,
                "Caracter o secuencia no reconocida: '" + t.lexema + "'",
                t.linea, t.columna);
        }
    }

    //   ==== 2. USO DEL ANALISIS SINTACTICO

    // 2. Analisis sintactico
    SyntaxAnalyzer parser(tokens, errMgr);
    NodoArbol* raiz = parser.parse();

    // 3. Construir JSON de respuesta
    std::ostringstream json;
    json << "{\n";

    // Tokens
    json << "  \"tokens\": [\n";
    for (size_t i = 0; i < tokens.size(); i++) {
        const auto& t = tokens[i];
        if (t.type == TokenType::FIN_ARCHIVO) break;
        json << "    {\"numero\":" << (i+1)
             << ",\"lexema\":\"" << escJson(t.lexema) << "\""
             << ",\"tipo\":\"" << tokenTypeToString(t.type) << "\""
             << ",\"linea\":" << t.linea
             << ",\"columna\":" << t.columna << "}";
        if (i + 1 < tokens.size() - 1) json << ",";
        json << "\n";
    }
    json << "  ],\n";

    // Errores
    json << "  \"errores\": [\n";
    const auto& errores = errMgr.getErrores();
    for (size_t i = 0; i < errores.size(); i++) {
        const auto& e = errores[i];
        std::string tipo = (e.tipo == TipoError::LEXICO) ? "Lexico" : "Sintactico";
        json << "    {\"numero\":" << e.numero
             << ",\"lexema\":\"" << escJson(e.lexema) << "\""
             << ",\"tipo\":\"" << tipo << "\""
             << ",\"descripcion\":\"" << escJson(e.descripcion) << "\""
             << ",\"linea\":" << e.linea
             << ",\"columna\":" << e.columna << "}";
        if (i + 1 < errores.size()) json << ",";
        json << "\n";
    }
    json << "  ],\n";

    // Arbol DOT
    std::string dot = generarDot(raiz);
    json << "  \"dot\": \"" << escJson(dot) << "\",\n";
    json << "  \"exitoso\": " << (errMgr.tieneErrores() ? "false" : "true") << "\n";
    json << "}\n";

    delete raiz;
    return json.str();
}

// ----------------------------------------------------------------
// Extraer body de una peticion HTTP POST simple
// ----------------------------------------------------------------
static std::string extraerBody(const std::string& request) {
    size_t pos = request.find("\r\n\r\n");
    if (pos == std::string::npos) return "";
    return request.substr(pos + 4);
}

// Extrae el valor de "source" del JSON {"source":"..."}
// Implementacion minima sin parser JSON completo
static std::string extraerSource(const std::string& body) {
    size_t pos = body.find("\"source\"");
    if (pos == std::string::npos) return "";
    pos = body.find(':', pos);
    if (pos == std::string::npos) return "";
    pos = body.find('"', pos);
    if (pos == std::string::npos) return "";
    pos++; // saltar la comilla de apertura
    std::string result;
    while (pos < body.size() && body[pos] != '"') {
        if (body[pos] == '\\' && pos + 1 < body.size()) {
            char next = body[++pos];
            switch (next) {
                case 'n':  result += '\n'; break;
                case 't':  result += '\t'; break;
                case '"':  result += '"';  break;
                case '\\': result += '\\'; break;
                default:   result += next; break;
            }
        } else {
            result += body[pos];
        }
        pos++;
    }
    return result;
}

// ----------------------------------------------------------------
// Servidor HTTP en puerto 8080
// ----------------------------------------------------------------
int main() {
    int serverFd = socket(AF_INET, SOCK_STREAM, 0);
    if (serverFd < 0) { perror("socket"); return 1; }

    int opt = 1;
    setsockopt(serverFd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    sockaddr_in addr{};
    addr.sin_family      = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port        = htons(8080);

    if (bind(serverFd, (sockaddr*)&addr, sizeof(addr)) < 0) {
        perror("bind"); return 1;
    }
    listen(serverFd, 10);
    std::cout << "TaskScript server escuchando en http://localhost:8080\n";

    while (true) {
        int clientFd = accept(serverFd, nullptr, nullptr);
        if (clientFd < 0) continue;

        // Leer peticion
        char buf[65536];
        ssize_t n = recv(clientFd, buf, sizeof(buf) - 1, 0);
        if (n <= 0) { close(clientFd); continue; }
        buf[n] = '\0';
        std::string request(buf);

        std::string responseBody;
        int statusCode = 200;

        // Cabeceras CORS para el frontend React
        std::string corsHeaders =
            "Access-Control-Allow-Origin: *\r\n"
            "Access-Control-Allow-Methods: POST, OPTIONS\r\n"
            "Access-Control-Allow-Headers: Content-Type\r\n";

        if (request.find("OPTIONS") == 0) {
            // Preflight CORS
            std::string resp = "HTTP/1.1 204 No Content\r\n" + corsHeaders + "\r\n";
            send(clientFd, resp.c_str(), resp.size(), 0);
            close(clientFd);
            continue;
        }

        if (request.find("POST /analyze") != std::string::npos) {
            std::string body   = extraerBody(request);
            std::string source = extraerSource(body);
            responseBody = analizarYResponder(source);
        } else {
            responseBody = "{\"error\": \"Endpoint no encontrado\"}";
            statusCode = 404;
        }

        std::string header =
            "HTTP/1.1 " + std::to_string(statusCode) + " OK\r\n" +
            corsHeaders +
            "Content-Type: application/json\r\n"
            "Content-Length: " + std::to_string(responseBody.size()) + "\r\n"
            "\r\n";
            
        std::string full = header + responseBody;
        send(clientFd, full.c_str(), full.size(), 0);
        close(clientFd);
    }
    close(serverFd);
    return 0;
}