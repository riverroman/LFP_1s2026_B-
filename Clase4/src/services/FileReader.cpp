#include "FileReader.h"
#include "../utils/StringUtils.h"
#include <fstream>
#include <iostream>

// ─────────────────────────────────────────────────────────────────────────────
// BUENA PRACTICA: Cada función valida la apertura del archivo y reporta
// el error de forma clara. Nunca asumimos que el archivo existe.
// ─────────────────────────────────────────────────────────────────────────────

std::vector<Estudiante> FileReader::leerEstudiantes(const std::string& ruta) {
    std::vector<Estudiante> lista;
    std::ifstream archivo(ruta);

    if (!archivo.is_open()) {
        std::cerr << "[ERROR] No se pudo abrir el archivo: " << ruta << std::endl;
        return lista; // Retorna vector vacío en caso de error
    }

    std::string linea;
    int numeroLinea = 0;

    while (std::getline(archivo, linea)) {
        numeroLinea++;
        linea = StringUtils::trim(linea);

        // Ignorar líneas vacías
        if (linea.empty()) continue;

        std::vector<std::string> campos = StringUtils::split(linea, ',');

        // Validar que la línea tenga exactamente 5 campos
        if (campos.size() != 5) {
            std::cerr << "[ADVERTENCIA] Línea " << numeroLinea
                      << " malformada en " << ruta
                      << " (se esperaban 5 campos, se encontraron "
                      << campos.size() << ")" << std::endl;
            continue;
        }

        Estudiante e(
            StringUtils::toInt(campos[0]),    // carnet
            StringUtils::trim(campos[1]),      // nombre
            StringUtils::trim(campos[2]),      // apellido
            StringUtils::trim(campos[3]),      // carrera
            StringUtils::toInt(campos[4])      // semestre
        );

        // Validación básica de datos
        if (e.carnet <= 0 || e.nombre.empty() || e.semestre <= 0) {
            std::cerr << "[ADVERTENCIA] Datos inválidos en línea " << numeroLinea << std::endl;
            continue;
        }

        lista.push_back(e);
    }

    archivo.close();
    std::cout << "[OK] Se cargaron " << lista.size()
              << " estudiantes desde: " << ruta << std::endl;
    return lista;
}

// ─────────────────────────────────────────────────────────────────────────────

std::vector<Curso> FileReader::leerCursos(const std::string& ruta) {
    std::vector<Curso> lista;
    std::ifstream archivo(ruta);

    if (!archivo.is_open()) {
        std::cerr << "[ERROR] No se pudo abrir el archivo: " << ruta << std::endl;
        return lista;
    }

    std::string linea;
    int numeroLinea = 0;

    while (std::getline(archivo, linea)) {
        numeroLinea++;
        linea = StringUtils::trim(linea);
        if (linea.empty()) continue;

        std::vector<std::string> campos = StringUtils::split(linea, ',');

        if (campos.size() != 5) {
            std::cerr << "[ADVERTENCIA] Línea " << numeroLinea
                      << " malformada en " << ruta << std::endl;
            continue;
        }

        Curso c(
            StringUtils::toInt(campos[0]),    // codigo
            StringUtils::trim(campos[1]),      // nombre
            StringUtils::toInt(campos[2]),     // creditos
            StringUtils::toInt(campos[3]),     // semestre
            StringUtils::trim(campos[4])       // carrera
        );

        if (c.codigo <= 0 || c.nombre.empty()) {
            std::cerr << "[ADVERTENCIA] Datos inválidos en línea " << numeroLinea << std::endl;
            continue;
        }

        lista.push_back(c);
    }

    archivo.close();
    std::cout << "[OK] Se cargaron " << lista.size()
              << " cursos desde: " << ruta << std::endl;
    return lista;
}

// ─────────────────────────────────────────────────────────────────────────────

std::vector<Nota> FileReader::leerNotas(const std::string& ruta) {
    std::vector<Nota> lista;
    std::ifstream archivo(ruta);

    if (!archivo.is_open()) {
        std::cerr << "[ERROR] No se pudo abrir el archivo: " << ruta << std::endl;
        return lista;
    }

    std::string linea;
    int numeroLinea = 0;

    while (std::getline(archivo, linea)) {
        numeroLinea++;
        linea = StringUtils::trim(linea);
        if (linea.empty()) continue;

        std::vector<std::string> campos = StringUtils::split(linea, ',');

        if (campos.size() != 5) {
            std::cerr << "[ADVERTENCIA] Línea " << numeroLinea
                      << " malformada en " << ruta << std::endl;
            continue;
        }

        Nota n(
            StringUtils::toInt(campos[0]),      // carnet
            StringUtils::toInt(campos[1]),      // codigo_curso
            StringUtils::toDouble(campos[2]),   // nota
            StringUtils::trim(campos[3]),        // ciclo
            StringUtils::toInt(campos[4])       // anio
        );

        // Validar rango de nota (0 - 100)
        if (n.nota < 0.0 || n.nota > 100.0) {
            std::cerr << "[ADVERTENCIA] Nota fuera de rango en línea " << numeroLinea << std::endl;
            continue;
        }

        lista.push_back(n);
    }

    archivo.close();
    std::cout << "[OK] Se cargaron " << lista.size()
              << " notas desde: " << ruta << std::endl;
    return lista;
}