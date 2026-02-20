#ifndef STRING_UTILS_H
#define STRING_UTILS_H

#include <string>
#include <vector>
#include <sstream>
#include <algorithm>

/**
 * @brief Utilidades de manejo de cadenas de texto.
 *
 * BUENA PRÁCTICA: Las funciones utilitarias reutilizables se agrupan en
 * un namespace para evitar colisiones de nombres y dejar claro su propósito.
 */

namespace StringUtils {

    /**
     * @brief Divide una cadena por un delimitador dado (equivalente a split).
     * @param linea   Cadena a dividir
     * @param delim   Carácter delimitador
     * @return Vector de subcadenas resultantes
     */
    inline std::vector<std::string> split(const std::string& linea, char delim) {
        std::vector<std::string> tokens;
        std::stringstream ss(linea);
        std::string token;
        while (std::getline(ss, token, delim)) {
            tokens.push_back(token);
        }
        return tokens;
    }

    /**
     * @brief Elimina espacios en blanco al inicio y final de una cadena (trim).
     */
    inline std::string trim(const std::string& s) {
        size_t inicio = s.find_first_not_of(" \t\r\n");
        if (inicio == std::string::npos) return "";
        size_t fin = s.find_last_not_of(" \t\r\n");
        return s.substr(inicio, fin - inicio + 1);
    }

    /**
     * @brief Convierte una cadena a entero de forma segura.
     * @return El entero o -1 si la conversión falla.
     */
    inline int toInt(const std::string& s) {
        try {
            return std::stoi(trim(s));
        } catch (...) {
            return -1;
        }
    }

    /**
     * @brief Convierte una cadena a double de forma segura.
     * @return El double o -1.0 si la conversión falla.
     */
    inline double toDouble(const std::string& s) {
        try {
            return std::stod(trim(s));
        } catch (...) {
            return -1.0;
        }
    }

} // namespace StringUtils

#endif // STRING_UTILS_H