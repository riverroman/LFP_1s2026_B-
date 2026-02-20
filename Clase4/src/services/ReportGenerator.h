#ifndef REPORT_GENERATOR_H
#define REPORT_GENERATOR_H

#include <string>
#include <vector>
#include "../models/Estudiante.h"
#include "../models/Curso.h"
#include "../models/Nota.h"

/**
 * @brief Servicio que genera los reportes HTML del sistema académico.
 *
 * BUENA PRÁCTICA: El generador de reportes NO calcula estadísticas por sí mismo;
 * recibe los datos ya procesados. Esto respeta la separación de capas:
 *   - FileReader  → lectura de datos
 *   - Statistics  → cálculos
 *   - ReportGenerator → presentación/salida
 */
class ReportGenerator {
public:
    /**
     * @brief Reporte 1: Estadísticas generales por curso.
     */
    static void generarReporte1_EstadisticasPorCurso(
        const std::vector<Curso>&       cursos,
        const std::vector<Nota>&        notas,
        const std::string&              rutaSalida
    );

    /**
     * @brief Reporte 2: Rendimiento por estudiante.
     */
    static void generarReporte2_RendimientoEstudiante(
        const std::vector<Estudiante>&  estudiantes,
        const std::vector<Curso>&       cursos,
        const std::vector<Nota>&        notas,
        const std::string&              rutaSalida
    );

    /**
     * @brief Reporte 3: Top 10 mejores estudiantes.
     */
    static void generarReporte3_Top10Estudiantes(
        const std::vector<Estudiante>&  estudiantes,
        const std::vector<Nota>&        notas,
        const std::string&              rutaSalida
    );

    /**
     * @brief Reporte 4: Cursos con mayor índice de reprobación.
     */
    static void generarReporte4_IndiceReprobacion(
        const std::vector<Curso>&       cursos,
        const std::vector<Nota>&        notas,
        const std::string&              rutaSalida
    );

    /**
     * @brief Reporte 5: Análisis por carrera.
     */
    static void generarReporte5_AnalisisPorCarrera(
        const std::vector<Estudiante>&  estudiantes,
        const std::vector<Curso>&       cursos,
        const std::vector<Nota>&        notas,
        const std::string&              rutaSalida
    );

private:
    ReportGenerator() = delete;

    // Helpers privados para construir el HTML base
    static std::string htmlCabecera(const std::string& titulo);
    static std::string htmlPie();
    static std::string formatDouble(double valor, int decimales = 2);
};

#endif // REPORT_GENERATOR_H