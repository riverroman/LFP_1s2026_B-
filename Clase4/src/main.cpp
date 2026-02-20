#include <iostream>
#include <string>
#include <vector>
#include <limits>

#include "models/Estudiante.h"
#include "models/Curso.h"
#include "models/Nota.h"
#include "services/FileReader.h"
#include "services/ReportGenerator.h"

// ─────────────────────────────────────────────────────────────────────────────
// Constantes de rutas (BUENA PRÁCTICA: centralizar rutas en un solo lugar)
// ─────────────────────────────────────────────────────────────────────────────
const std::string RUTA_DATA     = "data/";
const std::string RUTA_REPORTES = "reportes/";

// ─────────────────────────────────────────────────────────────────────────────
// Prototipos de funciones del menú
// ─────────────────────────────────────────────────────────────────────────────
void mostrarMenu();
void limpiarPantalla();
void pausar();
void cargarEstudiantes(std::vector<Estudiante>& estudiantes);
void cargarCursos(std::vector<Curso>& cursos);
void cargarNotas(std::vector<Nota>& notas);
bool verificarDatosCargados(const std::vector<Estudiante>& e,
                             const std::vector<Curso>& c,
                             const std::vector<Nota>& n);

// ─────────────────────────────────────────────────────────────────────────────
// FUNCIÓN PRINCIPAL
// ─────────────────────────────────────────────────────────────────────────────
int main() {
    // Estado del sistema: colecciones en memoria
    std::vector<Estudiante> estudiantes;
    std::vector<Curso>      cursos;
    std::vector<Nota>       notas;

    int opcion = 0;

    do {
        limpiarPantalla();
        mostrarMenu();

        std::cout << "  Seleccione una opcion: ";
        std::cin >> opcion;

        // Limpiar el buffer de entrada (BUENA PRÁCTICA: siempre limpiar cin)
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        limpiarPantalla();

        switch (opcion) {
            case 1:
                cargarEstudiantes(estudiantes);
                break;

            case 2:
                cargarCursos(cursos);
                break;

            case 3:
                cargarNotas(notas);
                break;

            case 4:
                if (!verificarDatosCargados(estudiantes, cursos, notas)) break;
                ReportGenerator::generarReporte1_EstadisticasPorCurso(
                    cursos, notas, RUTA_REPORTES + "reporte1_estadisticas_curso.html");
                break;

            case 5:
                if (!verificarDatosCargados(estudiantes, cursos, notas)) break;
                ReportGenerator::generarReporte2_RendimientoEstudiante(
                    estudiantes, cursos, notas, RUTA_REPORTES + "reporte2_rendimiento_estudiante.html");
                break;

            case 6:
                if (!verificarDatosCargados(estudiantes, cursos, notas)) break;
                ReportGenerator::generarReporte3_Top10Estudiantes(
                    estudiantes, notas, RUTA_REPORTES + "reporte3_top10.html");
                break;

            case 7:
                if (!verificarDatosCargados(estudiantes, cursos, notas)) break;
                ReportGenerator::generarReporte4_IndiceReprobacion(
                    cursos, notas, RUTA_REPORTES + "reporte4_reprobacion.html");
                break;

            case 8:
                if (!verificarDatosCargados(estudiantes, cursos, notas)) break;
                ReportGenerator::generarReporte5_AnalisisPorCarrera(
                    estudiantes, cursos, notas, RUTA_REPORTES + "reporte5_por_carrera.html");
                break;

            case 9:
                std::cout << "\n  Hasta pronto. Cerrando el sistema...\n\n";
                break;

            default:
                std::cout << "\n  [!] Opcion no valida. Intente de nuevo.\n";
                break;
        }

        if (opcion != 9) pausar();

    } while (opcion != 9);

    return 0; // BUENA PRÁCTICA: siempre retornar 0 al salir correctamente
}

// ─────────────────────────────────────────────────────────────────────────────
// IMPLEMENTACIONES DE FUNCIONES AUXILIARES
// ─────────────────────────────────────────────────────────────────────────────

void mostrarMenu() {
    std::cout << "\n";
    std::cout << "  ==========================================\n";
    std::cout << "     SISTEMA DE ANALISIS ACADEMICO\n";
    std::cout << "     LFP - USAC / FIUSAC  2026\n";
    std::cout << "  ==========================================\n\n";
    std::cout << "  --- Carga de Datos ---\n";
    std::cout << "   1. Cargar archivo de estudiantes\n";
    std::cout << "   2. Cargar archivo de cursos\n";
    std::cout << "   3. Cargar archivo de notas\n\n";
    std::cout << "  --- Generacion de Reportes ---\n";
    std::cout << "   4. Estadisticas Generales por Curso\n";
    std::cout << "   5. Rendimiento por Estudiante\n";
    std::cout << "   6. Top 10 Mejores Estudiantes\n";
    std::cout << "   7. Cursos con Mayor Indice de Reprobacion\n";
    std::cout << "   8. Analisis por Carrera\n\n";
    std::cout << "  --- Sistema ---\n";
    std::cout << "   9. Salir\n\n";
    std::cout << "  ==========================================\n";
}

void limpiarPantalla() {
    // Funciona en Linux/Mac. En Windows se usaría "cls"
    system("clear");
}

void pausar() {
    std::cout << "\n  Presione [Enter] para continuar...";
    std::cin.get();
}

void cargarEstudiantes(std::vector<Estudiante>& estudiantes) {
    std::string ruta;
    std::cout << "\n  Ingrese la ruta del archivo de estudiantes\n";
    std::cout << "  [Enter para usar '" << RUTA_DATA << "estudiantes.lfp']: ";
    std::getline(std::cin, ruta);

    if (ruta.empty()) ruta = RUTA_DATA + "estudiantes.lfp";

    estudiantes = FileReader::leerEstudiantes(ruta);
}

void cargarCursos(std::vector<Curso>& cursos) {
    std::string ruta;
    std::cout << "\n  Ingrese la ruta del archivo de cursos\n";
    std::cout << "  [Enter para usar '" << RUTA_DATA << "cursos.lfp']: ";
    std::getline(std::cin, ruta);

    if (ruta.empty()) ruta = RUTA_DATA + "cursos.lfp";

    cursos = FileReader::leerCursos(ruta);
}

void cargarNotas(std::vector<Nota>& notas) {
    std::string ruta;
    std::cout << "\n  Ingrese la ruta del archivo de notas\n";
    std::cout << "  [Enter para usar '" << RUTA_DATA << "notas.lfp']: ";
    std::getline(std::cin, ruta);

    if (ruta.empty()) ruta = RUTA_DATA + "notas.lfp";

    notas = FileReader::leerNotas(ruta);
}

bool verificarDatosCargados(const std::vector<Estudiante>& e,
                              const std::vector<Curso>& c,
                              const std::vector<Nota>& n) {
    bool ok = true;
    if (e.empty()) { std::cout << "  [!] Debe cargar primero el archivo de estudiantes (opcion 1).\n"; ok = false; }
    if (c.empty()) { std::cout << "  [!] Debe cargar primero el archivo de cursos (opcion 2).\n"; ok = false; }
    if (n.empty()) { std::cout << "  [!] Debe cargar primero el archivo de notas (opcion 3).\n"; ok = false; }
    return ok;
}