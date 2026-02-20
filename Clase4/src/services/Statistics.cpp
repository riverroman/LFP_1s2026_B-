#include "Statistics.h"
#include <algorithm>
#include <cmath>
#include <numeric>
#include <stdexcept>

namespace Statistics {

// ─────────────────────────────────────────────────────────────────────────────
// BUENA PRÁCTICA: Validar siempre que el vector no esté vacío antes de operar.
// Retornar 0.0 en casos degenerados evita divisiones por cero y crashes.
// ─────────────────────────────────────────────────────────────────────────────

double media(std::vector<double> datos) {
    if (datos.empty()) return 0.0;

    // std::accumulate es la forma idiomática de C++ para sumar un vector
    double suma = std::accumulate(datos.begin(), datos.end(), 0.0);
    return suma / static_cast<double>(datos.size());
}

// ─────────────────────────────────────────────────────────────────────────────

double mediana(std::vector<double> datos) {
    if (datos.empty()) return 0.0;

    // Ordenar una COPIA local (no modificamos el vector original del llamador)
    std::sort(datos.begin(), datos.end());

    size_t n = datos.size();
    if (n % 2 == 0) {
        // Si es par, la mediana es el promedio de los dos centrales
        return (datos[n / 2 - 1] + datos[n / 2]) / 2.0;
    } else {
        // Si es impar, la mediana es el elemento central
        return datos[n / 2];
    }
}

// ─────────────────────────────────────────────────────────────────────────────

double desviacionEstandar(std::vector<double> datos) {
    if (datos.size() < 2) return 0.0;

    double m = media(datos);
    double sumaCuadrados = 0.0;

    for (double val : datos) {
        double diferencia = val - m;
        sumaCuadrados += diferencia * diferencia;
    }

    // Usamos n-1 (desviación estándar muestral / Bessel's correction)
    return std::sqrt(sumaCuadrados / static_cast<double>(datos.size() - 1));
}

// ─────────────────────────────────────────────────────────────────────────────

double percentil(std::vector<double> datos, double p) {
    if (datos.empty()) return 0.0;
    if (p <= 0.0)  return *std::min_element(datos.begin(), datos.end());
    if (p >= 100.0) return *std::max_element(datos.begin(), datos.end());

    std::sort(datos.begin(), datos.end());

    // Método de interpolación lineal (percentil continuo)
    double indice = (p / 100.0) * static_cast<double>(datos.size() - 1);
    size_t inferior = static_cast<size_t>(std::floor(indice));
    size_t superior = static_cast<size_t>(std::ceil(indice));

    if (inferior == superior) return datos[inferior];

    double fraccion = indice - static_cast<double>(inferior);
    return datos[inferior] * (1.0 - fraccion) + datos[superior] * fraccion;
}

// ─────────────────────────────────────────────────────────────────────────────

double maximo(const std::vector<double>& datos) {
    if (datos.empty()) return 0.0;
    return *std::max_element(datos.begin(), datos.end());
}

double minimo(const std::vector<double>& datos) {
    if (datos.empty()) return 0.0;
    return *std::min_element(datos.begin(), datos.end());
}

} 