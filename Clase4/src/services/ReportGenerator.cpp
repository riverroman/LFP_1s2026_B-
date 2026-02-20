#include "ReportGenerator.h"
#include "Statistics.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <algorithm>
#include <map>
#include <set>

// ─────────────────────────────────────────────────────────────────────────────
// HELPERS PRIVADOS
// ─────────────────────────────────────────────────────────────────────────────

std::string ReportGenerator::htmlCabecera(const std::string& titulo) {
    return R"(<!DOCTYPE html>
<html lang="es">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>)" + titulo + R"(</title>
    <style>
        * { box-sizing: border-box; margin: 0; padding: 0; }
        body { font-family: 'Segoe UI', Arial, sans-serif; background: #f0f4f8; color: #333; }
        header { background: linear-gradient(135deg, #1a237e, #283593);
                 color: white; padding: 30px 40px; }
        header h1 { font-size: 1.8em; margin-bottom: 6px; }
        header p  { opacity: 0.8; font-size: 0.95em; }
        .contenedor { max-width: 1100px; margin: 30px auto; padding: 0 20px; }
        .tarjeta { background: white; border-radius: 10px; padding: 25px;
                   box-shadow: 0 2px 12px rgba(0,0,0,0.08); margin-bottom: 25px; }
        table { width: 100%; border-collapse: collapse; font-size: 0.92em; }
        th { background: #1a237e; color: white; padding: 12px 15px; text-align: left; }
        td { padding: 10px 15px; border-bottom: 1px solid #e8eaf0; }
        tr:hover td { background: #f5f7ff; }
        tr:nth-child(even) td { background: #fafbff; }
        .badge-verde  { background:#e8f5e9; color:#2e7d32; padding:3px 10px;
                        border-radius:20px; font-size:0.85em; font-weight:600; }
        .badge-rojo   { background:#ffebee; color:#c62828; padding:3px 10px;
                        border-radius:20px; font-size:0.85em; font-weight:600; }
        .badge-azul   { background:#e3f2fd; color:#1565c0; padding:3px 10px;
                        border-radius:20px; font-size:0.85em; font-weight:600; }
        .stat-grid { display:grid; grid-template-columns:repeat(auto-fit,minmax(160px,1fr));
                     gap:15px; margin-bottom:20px; }
        .stat-box { background:#f5f7ff; border-left:4px solid #1a237e;
                    padding:15px; border-radius:6px; }
        .stat-box .valor { font-size:1.6em; font-weight:700; color:#1a237e; }
        .stat-box .etiq  { font-size:0.8em; color:#666; margin-top:4px; }
        footer { text-align:center; padding:20px; color:#888; font-size:0.85em; }
    </style>
</head>
<body>
<header>
    <h1>)" + titulo + R"(</h1>
    <p>Sistema de Análisis de Rendimiento Académico &mdash; USAC / FIUSAC</p>
</header>
<div class="contenedor">
)";
}

std::string ReportGenerator::htmlPie() {
    return R"(</div>
<footer>Generado automáticamente por el Sistema de Análisis Académico &bull; LFP 2026</footer>
</body></html>)";
}

std::string ReportGenerator::formatDouble(double valor, int decimales) {
    std::ostringstream oss;
    oss << std::fixed << std::setprecision(decimales) << valor;
    return oss.str();
}

// ─────────────────────────────────────────────────────────────────────────────
// REPORTE 1: Estadísticas generales por curso
// ─────────────────────────────────────────────────────────────────────────────

void ReportGenerator::generarReporte1_EstadisticasPorCurso(
    const std::vector<Curso>& cursos,
    const std::vector<Nota>&  notas,
    const std::string&        rutaSalida)
{
    std::ofstream archivo(rutaSalida);
    if (!archivo.is_open()) {
        std::cerr << "[ERROR] No se pudo crear: " << rutaSalida << std::endl;
        return;
    }

    archivo << htmlCabecera("Reporte 1 – Estadísticas por Curso");
    archivo << "<div class='tarjeta'>\n";
    archivo << "<h2 style='margin-bottom:20px;color:#1a237e;'>Estadísticas Generales por Curso</h2>\n";
    archivo << "<table>\n<thead><tr>"
            << "<th>Código</th><th>Nombre del Curso</th><th>Carrera</th>"
            << "<th>Estudiantes</th><th>Promedio</th><th>Máxima</th>"
            << "<th>Mínima</th><th>Desv. Estándar</th><th>Mediana</th>"
            << "<th>P25</th><th>P75</th>"
            << "</tr></thead>\n<tbody>\n";

    for (const Curso& curso : cursos) {
        // Recopilar notas de este curso
        std::vector<double> notasCurso;
        for (const Nota& n : notas) {
            if (n.codigoCurso == curso.codigo) {
                notasCurso.push_back(n.nota);
            }
        }

        if (notasCurso.empty()) continue; // Ignorar cursos sin notas

        archivo << "<tr>"
                << "<td><span class='badge-azul'>" << curso.codigo << "</span></td>"
                << "<td>" << curso.nombre << "</td>"
                << "<td>" << curso.carrera << "</td>"
                << "<td style='text-align:center;'>" << notasCurso.size() << "</td>"
                << "<td>" << formatDouble(Statistics::media(notasCurso)) << "</td>"
                << "<td style='color:#2e7d32;font-weight:600;'>" << formatDouble(Statistics::maximo(notasCurso)) << "</td>"
                << "<td style='color:#c62828;font-weight:600;'>" << formatDouble(Statistics::minimo(notasCurso)) << "</td>"
                << "<td>" << formatDouble(Statistics::desviacionEstandar(notasCurso)) << "</td>"
                << "<td>" << formatDouble(Statistics::mediana(notasCurso)) << "</td>"
                << "<td>" << formatDouble(Statistics::percentil(notasCurso, 25)) << "</td>"
                << "<td>" << formatDouble(Statistics::percentil(notasCurso, 75)) << "</td>"
                << "</tr>\n";
    }

    archivo << "</tbody></table></div>\n";
    archivo << htmlPie();
    archivo.close();
    std::cout << "[OK] Reporte 1 generado: " << rutaSalida << std::endl;
}

// ─────────────────────────────────────────────────────────────────────────────
// REPORTE 2: Rendimiento por estudiante
// ─────────────────────────────────────────────────────────────────────────────

void ReportGenerator::generarReporte2_RendimientoEstudiante(
    const std::vector<Estudiante>& estudiantes,
    const std::vector<Curso>&      cursos,
    const std::vector<Nota>&       notas,
    const std::string&             rutaSalida)
{
    std::ofstream archivo(rutaSalida);
    if (!archivo.is_open()) {
        std::cerr << "[ERROR] No se pudo crear: " << rutaSalida << std::endl;
        return;
    }

    archivo << htmlCabecera("Reporte 2 – Rendimiento por Estudiante");
    archivo << "<div class='tarjeta'>\n";
    archivo << "<h2 style='margin-bottom:20px;color:#1a237e;'>Rendimiento por Estudiante</h2>\n";
    archivo << "<table>\n<thead><tr>"
            << "<th>Carnet</th><th>Nombre Completo</th><th>Carrera</th>"
            << "<th>Semestre</th><th>Promedio</th><th>Aprobados</th>"
            << "<th>Reprobados</th><th>Créditos Acum.</th>"
            << "</tr></thead>\n<tbody>\n";

    for (const Estudiante& est : estudiantes) {
        std::vector<double> notasEst;
        int aprobados  = 0;
        int reprobados = 0;
        int creditos   = 0;

        for (const Nota& n : notas) {
            if (n.carnet == est.carnet) {
                notasEst.push_back(n.nota);
                if (n.estaAprobado()) {
                    aprobados++;
                    // Buscar créditos del curso aprobado
                    for (const Curso& c : cursos) {
                        if (c.codigo == n.codigoCurso) {
                            creditos += c.creditos;
                            break;
                        }
                    }
                } else {
                    reprobados++;
                }
            }
        }

        if (notasEst.empty()) continue;

        double promedio = Statistics::media(notasEst);
        std::string claseProm = (promedio >= 61) ? "badge-verde" : "badge-rojo";

        archivo << "<tr>"
                << "<td>" << est.carnet << "</td>"
                << "<td>" << est.nombreCompleto() << "</td>"
                << "<td>" << est.carrera << "</td>"
                << "<td style='text-align:center;'>" << est.semestre << "</td>"
                << "<td><span class='" << claseProm << "'>" << formatDouble(promedio) << "</span></td>"
                << "<td style='color:#2e7d32;text-align:center;'>" << aprobados << "</td>"
                << "<td style='color:#c62828;text-align:center;'>" << reprobados << "</td>"
                << "<td style='text-align:center;'>" << creditos << "</td>"
                << "</tr>\n";
    }

    archivo << "</tbody></table></div>\n";
    archivo << htmlPie();
    archivo.close();
    std::cout << "[OK] Reporte 2 generado: " << rutaSalida << std::endl;
}

// ─────────────────────────────────────────────────────────────────────────────
// REPORTE 3: Top 10 mejores estudiantes
// ─────────────────────────────────────────────────────────────────────────────

void ReportGenerator::generarReporte3_Top10Estudiantes(
    const std::vector<Estudiante>& estudiantes,
    const std::vector<Nota>&       notas,
    const std::string&             rutaSalida)
{
    // Calcular promedio de cada estudiante
    struct EstudiantePromedio {
        const Estudiante* est;
        double promedio;
    };

    std::vector<EstudiantePromedio> lista;

    for (const Estudiante& est : estudiantes) {
        std::vector<double> notasEst;
        for (const Nota& n : notas) {
            if (n.carnet == est.carnet) notasEst.push_back(n.nota);
        }
        if (!notasEst.empty()) {
            lista.push_back({&est, Statistics::media(notasEst)});
        }
    }

    // Ordenar descendentemente por promedio
    std::sort(lista.begin(), lista.end(),
        [](const EstudiantePromedio& a, const EstudiantePromedio& b) {
            return a.promedio > b.promedio;
        });

    std::ofstream archivo(rutaSalida);
    if (!archivo.is_open()) {
        std::cerr << "[ERROR] No se pudo crear: " << rutaSalida << std::endl;
        return;
    }

    archivo << htmlCabecera("Reporte 3 – Top 10 Mejores Estudiantes");
    archivo << "<div class='tarjeta'>\n";
    archivo << "<h2 style='margin-bottom:20px;color:#1a237e;'>🏆 Top 10 Mejores Estudiantes</h2>\n";
    archivo << "<table>\n<thead><tr>"
            << "<th>Posición</th><th>Carnet</th><th>Nombre Completo</th>"
            << "<th>Carrera</th><th>Semestre</th><th>Promedio General</th>"
            << "</tr></thead>\n<tbody>\n";

    std::string medallas[] = {"🥇", "🥈", "🥉"};
    int limite = std::min((int)lista.size(), 10);

    for (int i = 0; i < limite; i++) {
        std::string pos = (i < 3)
            ? medallas[i]
            : "#" + std::to_string(i + 1);

        archivo << "<tr>"
                << "<td style='text-align:center;font-size:1.3em;'>" << pos << "</td>"
                << "<td>" << lista[i].est->carnet << "</td>"
                << "<td><strong>" << lista[i].est->nombreCompleto() << "</strong></td>"
                << "<td>" << lista[i].est->carrera << "</td>"
                << "<td style='text-align:center;'>" << lista[i].est->semestre << "</td>"
                << "<td><span class='badge-verde'>" << formatDouble(lista[i].promedio) << "</span></td>"
                << "</tr>\n";
    }

    archivo << "</tbody></table></div>\n";
    archivo << htmlPie();
    archivo.close();
    std::cout << "[OK] Reporte 3 generado: " << rutaSalida << std::endl;
}

// ─────────────────────────────────────────────────────────────────────────────
// REPORTE 4: Cursos con mayor índice de reprobación
// ─────────────────────────────────────────────────────────────────────────────

void ReportGenerator::generarReporte4_IndiceReprobacion(
    const std::vector<Curso>& cursos,
    const std::vector<Nota>&  notas,
    const std::string&        rutaSalida)
{
    struct InfoReprobacion {
        const Curso* curso;
        int total;
        int aprobados;
        int reprobados;
        double porcentaje;
    };

    std::vector<InfoReprobacion> lista;

    for (const Curso& curso : cursos) {
        int total = 0, aprobados = 0, reprobados = 0;
        for (const Nota& n : notas) {
            if (n.codigoCurso == curso.codigo) {
                total++;
                if (n.estaAprobado()) aprobados++;
                else reprobados++;
            }
        }
        if (total > 0) {
            double pct = (static_cast<double>(reprobados) / total) * 100.0;
            lista.push_back({&curso, total, aprobados, reprobados, pct});
        }
    }

    // Ordenar por porcentaje de reprobación descendente
    std::sort(lista.begin(), lista.end(),
        [](const InfoReprobacion& a, const InfoReprobacion& b) {
            return a.porcentaje > b.porcentaje;
        });

    std::ofstream archivo(rutaSalida);
    if (!archivo.is_open()) {
        std::cerr << "[ERROR] No se pudo crear: " << rutaSalida << std::endl;
        return;
    }

    archivo << htmlCabecera("Reporte 4 – Índice de Reprobación");
    archivo << "<div class='tarjeta'>\n";
    archivo << "<h2 style='margin-bottom:20px;color:#1a237e;'>Cursos por Índice de Reprobación</h2>\n";
    archivo << "<table>\n<thead><tr>"
            << "<th>Código</th><th>Nombre del Curso</th><th>Carrera</th>"
            << "<th>Total</th><th>Aprobados</th><th>Reprobados</th><th>% Reprobación</th>"
            << "</tr></thead>\n<tbody>\n";

    for (const InfoReprobacion& info : lista) {
        std::string colorPct = (info.porcentaje >= 50) ? "#c62828" : "#e65100";
        archivo << "<tr>"
                << "<td><span class='badge-azul'>" << info.curso->codigo << "</span></td>"
                << "<td>" << info.curso->nombre << "</td>"
                << "<td>" << info.curso->carrera << "</td>"
                << "<td style='text-align:center;'>" << info.total << "</td>"
                << "<td style='color:#2e7d32;text-align:center;'>" << info.aprobados << "</td>"
                << "<td style='color:#c62828;text-align:center;'>" << info.reprobados << "</td>"
                << "<td><span class='badge-rojo'>" << formatDouble(info.porcentaje) << "%</span></td>"
                << "</tr>\n";
    }

    archivo << "</tbody></table></div>\n";
    archivo << htmlPie();
    archivo.close();
    std::cout << "[OK] Reporte 4 generado: " << rutaSalida << std::endl;
}

// ─────────────────────────────────────────────────────────────────────────────
// REPORTE 5: Análisis por carrera
// ─────────────────────────────────────────────────────────────────────────────

void ReportGenerator::generarReporte5_AnalisisPorCarrera(
    const std::vector<Estudiante>& estudiantes,
    const std::vector<Curso>&      cursos,
    const std::vector<Nota>&       notas,
    const std::string&             rutaSalida)
{
    // Identificar todas las carreras únicas
    std::set<std::string> carreras;
    for (const Estudiante& e : estudiantes) carreras.insert(e.carrera);

    std::ofstream archivo(rutaSalida);
    if (!archivo.is_open()) {
        std::cerr << "[ERROR] No se pudo crear: " << rutaSalida << std::endl;
        return;
    }

    archivo << htmlCabecera("Reporte 5 – Análisis por Carrera");

    for (const std::string& carrera : carreras) {
        // Estudiantes de la carrera
        std::vector<const Estudiante*> estCarrera;
        for (const Estudiante& e : estudiantes) {
            if (e.carrera == carrera) estCarrera.push_back(&e);
        }

        // Cursos de la carrera
        int numCursos = 0;
        for (const Curso& c : cursos) {
            if (c.carrera == carrera) numCursos++;
        }

        // Notas de estudiantes de esta carrera
        std::vector<double> todasNotas;
        std::map<int, int> dist; // semestre → cantidad
        for (const Estudiante* e : estCarrera) {
            dist[e->semestre]++;
            for (const Nota& n : notas) {
                if (n.carnet == e->carnet) todasNotas.push_back(n.nota);
            }
        }

        double promedio = Statistics::media(todasNotas);

        archivo << "<div class='tarjeta'>\n";
        archivo << "<h2 style='color:#1a237e;margin-bottom:18px;'>📚 " << carrera << "</h2>\n";
        archivo << "<div class='stat-grid'>\n";
        archivo << "<div class='stat-box'><div class='valor'>" << estCarrera.size()
                << "</div><div class='etiq'>Estudiantes</div></div>\n";
        archivo << "<div class='stat-box'><div class='valor'>" << numCursos
                << "</div><div class='etiq'>Cursos disponibles</div></div>\n";
        archivo << "<div class='stat-box'><div class='valor'>" << formatDouble(promedio)
                << "</div><div class='etiq'>Promedio general</div></div>\n";
        archivo << "<div class='stat-box'><div class='valor'>" << todasNotas.size()
                << "</div><div class='etiq'>Total calificaciones</div></div>\n";
        archivo << "</div>\n"; // stat-grid

        // Distribución por semestre
        archivo << "<h3 style='margin:15px 0 10px;color:#555;'>Distribución por Semestre</h3>\n";
        archivo << "<table><thead><tr><th>Semestre</th><th>Cantidad de Estudiantes</th></tr></thead><tbody>\n";
        for (auto& [sem, cant] : dist) {
            archivo << "<tr><td>Semestre " << sem << "</td>"
                    << "<td>" << cant << "</td></tr>\n";
        }
        archivo << "</tbody></table></div>\n";
    }

    archivo << htmlPie();
    archivo.close();
    std::cout << "[OK] Reporte 5 generado: " << rutaSalida << std::endl;
}