#ifndef STATISTICS_H
#define STATISTICS_H

#include <vector>

/**
 * @brief Servicio de cálculos estadísticos sobre colecciones de datos.
 *
 * BUENA PRÁCTICA: Funciones puras (sin estado, sin efectos secundarios).
 * Reciben datos y retornan resultados. Son fácilmente testeables de forma
 * aislada, lo que es fundamental en proyectos de software real.
 */
namespace Statistics {

    /**
     * @brief Calcula la media (promedio) aritmética.
     * @param datos Vector de valores numéricos
     * @return Media aritmética, o 0.0 si el vector está vacío.
     */
    double media(std::vector<double> datos);

    /**
     * @brief Calcula la mediana (valor central al ordenar los datos).
     * @param datos Vector de valores (se ordena internamente, no modifica el original)
     * @return Mediana del conjunto de datos.
     */
    double mediana(std::vector<double> datos);

    /**
     * @brief Calcula la desviación estándar muestral.
     * @param datos Vector de valores numéricos
     * @return Desviación estándar.
     */
    double desviacionEstandar(std::vector<double> datos);

    /**
     * @brief Calcula el valor en un percentil dado.
     * @param datos     Vector de valores
     * @param percentil Percentil deseado (0-100)
     * @return Valor correspondiente al percentil.
     */
    double percentil(std::vector<double> datos, double percentil);

    /**
     * @brief Retorna el valor máximo del vector.
     */
    double maximo(const std::vector<double>& datos);

    /**
     * @brief Retorna el valor mínimo del vector.
     */
    double minimo(const std::vector<double>& datos);

} // namespace Statistics

#endif // STATISTICS_H