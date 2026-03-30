#include "ReportGenerator.h"
#include "ErrorManager.h"
#include "Token.h"
#include <fstream>
#include <map>
#include <vector>
#include <sstream>
#include <iomanip>
#include <set>

using namespace std;

void ReportGenerator::generateTokenReport(
    const vector<Token>& tokens,
    const string& filename
    ) {

    ofstream file(filename);

    file << "<html>";
    file << "<head>";

    file << "<style>";
    file << "table { border-collapse: collapse; width: 100%; }";
    file << "th, td { border:1px solid black; padding:8px; text-align:left; }";
    file << "th { background:#2E7D52; color:white; }";
    file << "</style>";

    file << "</head>";
    file << "<body>";

    file << "<h2>Token Report</h2>";

    file << "<table>";

    file << "<tr>";
    file << "<th>No</th>";
    file << "<th>Lexeme</th>";
    file << "<th>Type</th>";
    file << "<th>Line</th>";
    file << "<th>Column</th>";
    file << "</tr>";

    int counter = 1;

    for (const Token& token : tokens) {
        file << "<tr>";
        file << "<td>" << counter++ << "</td>";
        file << "<td>" << token.lexeme << "</td>";
        file << "<td>" << token.getTypeString() << "</td>";
        file << "<td>" << token.line << "</td>";
        file << "<td>" << token.column << "</td>";

        file << "</tr>";
    }

    file << "</table>";

    file << "</body>";
    file << "</html>";

    file.close();
}

void ReportGenerator::generateErrorReport(
    const vector<LexicalError>& errors,
    const string& filename
    ) {

    ofstream file(filename);

    file << "<html>";
    file << "<head>";

    file << "<style>";
    file << "table { border-collapse: collapse; width: 100%; }";
    file << "th, td { border:1px solid black; padding:8px; text-align:left; }";
    file << "th { background:#E53935; color:white; }";
    file << "</style>";

    file << "</head>";
    file << "<body>";

    file << "<h2>Error Report</h2>";

    file << "<table>";

    file << "<tr>";
    file << "<th>No</th>";
    file << "<th>Lexeme</th>";
    file << "<th>Description</th>";
    file << "<th>Line</th>";
    file << "<th>Column</th>";
    file << "</tr>";

    int counter = 1;

    for (const LexicalError& error : errors) {

        file << "<tr>";

        file << "<td>" << counter++ << "</td>";
        file << "<td>" << error.lexeme << "</td>";
        file << "<td>" << error.description << "</td>";
        file << "<td>" << error.line << "</td>";
        file << "<td>" << error.column << "</td>";

        file << "</tr>";
    }

    file << "</table>";

    file << "</body>";
    file << "</html>";

    file.close();
}


void ReportGenerator::generateStatisticsReport(
    const std::vector<Token>& tokens,
    const std::string& filename
    ) {
    // ── Estructuras de datos para cruzar información ────────────
    struct Paciente {
        std::string nombre;
        int edad = 0;
    };
    struct Medico {
        std::string nombre;
        std::string codigo;
        std::string especialidad;
    };
    struct Cita {
        std::string paciente;
        std::string medico;
        std::string fecha;
        std::string hora;
    };
    struct Diagnostico {
        std::string paciente;
        std::string medicamento;
        std::string dosis;
    };

    std::vector<Paciente>    pacientes;
    std::vector<Medico>      medicos;
    std::vector<Cita>        citas;
    std::vector<Diagnostico> diagnosticos;

    // ── Parser semántico sobre los tokens ───────────────────────
    // Recorremos buscando patrones: RESERVED seguido de atributos
    int n = (int)tokens.size();

    for (int i = 0; i < n; i++) {
        const Token& t = tokens[i];

        // ── paciente: "Nombre" [edad: N, tipo_sangre: "X", habitacion: N]
        if (t.type == TokenType::RESERVED && t.lexeme == "paciente") {
            Paciente p;
            // siguiente STRING es el nombre
            for (int j = i+1; j < n && j < i+4; j++) {
                if (tokens[j].type == TokenType::STRING) {
                    p.nombre = tokens[j].lexeme;
                    p.nombre = p.nombre.substr(1, p.nombre.size()-2); // quitar comillas
                    break;
                }
            }
            // buscar edad dentro del bloque [...]
            for (int j = i+1; j < n; j++) {
                if (tokens[j].type == TokenType::SYMBOL && tokens[j].lexeme == "]") break;
                if (tokens[j].type == TokenType::IDENTIFIER && tokens[j].lexeme == "edad") {
                    // edad : NUMBER
                    if (j+2 < n && tokens[j+2].type == TokenType::NUMBER) {
                        try { p.edad = std::stoi(tokens[j+2].lexeme); } catch(...) {}
                    }
                }
            }
            if (!p.nombre.empty()) pacientes.push_back(p);
        }

        // ── medico: "Nombre" [especialidad: ENUM, codigo: "MED-001"]
        if (t.type == TokenType::RESERVED && t.lexeme == "medico") {
            Medico m;
            for (int j = i+1; j < n && j < i+4; j++) {
                if (tokens[j].type == TokenType::STRING) {
                    m.nombre = tokens[j].lexeme;
                    m.nombre = m.nombre.substr(1, m.nombre.size()-2);
                    break;
                }
            }
            for (int j = i+1; j < n; j++) {
                if (tokens[j].type == TokenType::SYMBOL && tokens[j].lexeme == "]") break;
                if (tokens[j].type == TokenType::IDENTIFIER && tokens[j].lexeme == "especialidad") {
                    if (j+2 < n && tokens[j+2].type == TokenType::ENUM_VAL)
                        m.especialidad = tokens[j+2].lexeme;
                }
                if (tokens[j].type == TokenType::IDENTIFIER && tokens[j].lexeme == "codigo") {
                    if (j+2 < n && tokens[j+2].type == TokenType::STRING) {
                        m.codigo = tokens[j+2].lexeme;
                        m.codigo = m.codigo.substr(1, m.codigo.size()-2);
                    }
                }
            }
            if (!m.nombre.empty()) medicos.push_back(m);
        }

        // ── cita: "Paciente" con "Medico" [fecha: DATE, hora: TIME]
        if (t.type == TokenType::RESERVED && t.lexeme == "cita") {
            Cita c;
            // primer STRING = paciente
            for (int j = i+1; j < n && j < i+4; j++) {
                if (tokens[j].type == TokenType::STRING) {
                    c.paciente = tokens[j].lexeme;
                    c.paciente = c.paciente.substr(1, c.paciente.size()-2);
                    // segundo STRING = medico (después de "con")
                    for (int k = j+1; k < n && k < j+4; k++) {
                        if (tokens[k].type == TokenType::STRING) {
                            c.medico = tokens[k].lexeme;
                            c.medico = c.medico.substr(1, c.medico.size()-2);
                            break;
                        }
                    }
                    break;
                }
            }
            for (int j = i+1; j < n; j++) {
                if (tokens[j].type == TokenType::SYMBOL && tokens[j].lexeme == "]") break;
                if (tokens[j].type == TokenType::IDENTIFIER && tokens[j].lexeme == "fecha") {
                    if (j+2 < n && tokens[j+2].type == TokenType::DATE)
                        c.fecha = tokens[j+2].lexeme;
                }
                if (tokens[j].type == TokenType::IDENTIFIER && tokens[j].lexeme == "hora") {
                    if (j+2 < n && tokens[j+2].type == TokenType::TIME)
                        c.hora = tokens[j+2].lexeme;
                }
            }
            if (!c.paciente.empty()) citas.push_back(c);
        }

        // ── diagnostico: "Paciente" [condicion: "X", medicamento: "Y", dosis: ENUM]
        if (t.type == TokenType::RESERVED && t.lexeme == "diagnostico") {
            Diagnostico d;
            for (int j = i+1; j < n && j < i+4; j++) {
                if (tokens[j].type == TokenType::STRING) {
                    d.paciente = tokens[j].lexeme;
                    d.paciente = d.paciente.substr(1, d.paciente.size()-2);
                    break;
                }
            }
            for (int j = i+1; j < n; j++) {
                if (tokens[j].type == TokenType::SYMBOL && tokens[j].lexeme == "]") break;
                if (tokens[j].type == TokenType::IDENTIFIER && tokens[j].lexeme == "medicamento") {
                    if (j+2 < n && tokens[j+2].type == TokenType::STRING) {
                        d.medicamento = tokens[j+2].lexeme;
                        d.medicamento = d.medicamento.substr(1, d.medicamento.size()-2);
                    }
                }
                if (tokens[j].type == TokenType::IDENTIFIER && tokens[j].lexeme == "dosis") {
                    if (j+2 < n && tokens[j+2].type == TokenType::ENUM_VAL)
                        d.dosis = tokens[j+2].lexeme;
                }
            }
            if (!d.paciente.empty()) diagnosticos.push_back(d);
        }
    }

    // ── Calcular estadísticas ───────────────────────────────────

    // Promedio de edad
    double promedioEdad = 0;
    if (!pacientes.empty()) {
        int suma = 0;
        for (auto& p : pacientes) suma += p.edad;
        promedioEdad = (double)suma / pacientes.size();
    }

    // Pacientes con diagnóstico activo
    int pacientesConDiag = (int)diagnosticos.size();
    int pctDiag = pacientes.empty() ? 0 : (pacientesConDiag * 100 / (int)pacientes.size());

    // Medicamento más prescrito
    std::map<std::string, int> medFreq;
    for (auto& d : diagnosticos) medFreq[d.medicamento]++;
    std::string medTop = "—"; int medTopCnt = 0;
    for (auto& [med, cnt] : medFreq) {
        if (cnt > medTopCnt) { medTopCnt = cnt; medTop = med; }
    }

    // Conflictos de citas (mismo médico, misma fecha y hora)
    int conflictos = 0;
    for (int i = 0; i < (int)citas.size(); i++)
        for (int j = i+1; j < (int)citas.size(); j++)
            if (citas[i].medico == citas[j].medico &&
                citas[i].fecha == citas[j].fecha &&
                citas[i].hora  == citas[j].hora)
                conflictos++;

    // Estadísticas por especialidad
    struct EspStat {
        int numMedicos = 0;
        int numCitas = 0;
        std::set<std::string> pacientesUnicos;
        std::string medicoTopNombre;
    };
    std::map<std::string, EspStat> espStats;

    for (auto& m : medicos) {
        espStats[m.especialidad].numMedicos++;
    }
    for (auto& c : citas) {
        // buscar especialidad del médico de esta cita
        for (auto& m : medicos) {
            if (m.nombre == c.medico) {
                espStats[m.especialidad].numCitas++;
                espStats[m.especialidad].pacientesUnicos.insert(c.paciente);
                break;
            }
        }
    }

    // Especialidad con mayor carga
    std::string espTopNombre = "—"; int espTopCitas = 0;
    std::string medicoTopNombre = "—";
    for (auto& [esp, stat] : espStats) {
        if (stat.numCitas > espTopCitas) {
            espTopCitas = stat.numCitas;
            espTopNombre = esp;
            // buscar el médico de esa especialidad con más citas
            std::map<std::string,int> citasPorMedico;
            for (auto& c : citas)
                for (auto& m : medicos)
                    if (m.nombre == c.medico && m.especialidad == esp)
                        citasPorMedico[m.nombre]++;
            int maxC = 0;
            for (auto& [nm, cnt] : citasPorMedico)
                if (cnt > maxC) { maxC = cnt; medicoTopNombre = nm; }
        }
    }

    // Total citas por especialidad para calcular % sobre el total
    int totalCitasHospital = (int)citas.size();

    // ── Generar HTML ────────────────────────────────────────────
    std::ofstream file(filename);
    file << R"(<!DOCTYPE html><html><head><meta charset="UTF-8">
        <title>Reporte Estadístico</title>
        <style>
          body { font-family: Arial, sans-serif; margin: 30px; background: #f5f5f5; color: #333; }
          h1 { color: #1A4731; border-bottom: 3px solid #2E7D52; padding-bottom: 10px; }
          h2 { color: #2E7D52; margin-top: 35px; }
          /* Tabla KPI estilo enunciado */
          .kpi-table { width: 100%; border-collapse: collapse; background: white;
                       box-shadow: 0 2px 6px rgba(0,0,0,0.1); border-radius: 8px; overflow: hidden; }
          .kpi-table th { background: #1A4731; color: white; padding: 12px 16px; text-align: left; }
          .kpi-table td { padding: 11px 16px; border-bottom: 1px solid #eee; }
          .kpi-table tr:last-child td { border-bottom: none; }
          .kpi-table td:first-child { font-weight: bold; width: 45%; background: #f0f7f3; }
          .warn { color: #E53935; font-weight: bold; }
          /* Tabla sección B */
          .esp-table { width: 100%; border-collapse: collapse; background: white;
                       box-shadow: 0 2px 6px rgba(0,0,0,0.1); border-radius: 8px; overflow: hidden; margin-top: 10px; }
          .esp-table th { background: #1A4731; color: white; padding: 12px; text-align: left; }
          .esp-table td { padding: 10px 12px; border-bottom: 1px solid #eee; }
          .esp-table tr:last-child td { border-bottom: none; }
          .esp-table tr:hover td { background: #f0f7f3; }
          .bar-wrap { display: inline-block; background: #ddd; border-radius: 4px;
                      height: 16px; width: 180px; vertical-align: middle; }
          .bar-fill { background: #2E7D52; border-radius: 4px; height: 16px; display: block; }
          .bar-fill.high { background: #E53935; }
          .pct-label { margin-left: 6px; font-weight: bold; font-size: 0.9em; }
        </style>
        </head><body>
        <h1>Reporte Estadístico General del Hospital</h1>)";

    // ── Sección A: tabla de indicadores clave ───────────────────
    file << "<h2>Secci&oacute;n A &mdash; Indicadores Clave del Hospital</h2>";
    file << "<table class='kpi-table'>";
    file << "<tr><th>Indicador</th><th>Valor</th></tr>";

    file << "<tr><td>Total de pacientes registrados</td><td>" << pacientes.size() << "</td></tr>";
    file << "<tr><td>Total de m&eacute;dicos activos</td><td>" << medicos.size() << "</td></tr>";
    file << "<tr><td>Total de citas programadas</td><td>" << citas.size() << "</td></tr>";

    if (conflictos > 0)
        file << "<tr><td>Citas con conflicto de horario</td><td class='warn'>" << conflictos << " &#9888;</td></tr>";
    else
        file << "<tr><td>Citas con conflicto de horario</td><td>0</td></tr>";

    file << "<tr><td>Pacientes con diagn&oacute;stico activo</td><td>"
         << pacientesConDiag << " de " << pacientes.size()
         << " (" << pctDiag << "%)</td></tr>";

    file << "<tr><td>Medicamento m&aacute;s prescrito</td><td>"
         << medTop << " (" << medTopCnt << " paciente"
         << (medTopCnt!=1?"s":"") << ")</td></tr>";

    file << "<tr><td>Especialidad con mayor carga de citas</td><td>"
         << espTopNombre << " &mdash; " << medicoTopNombre
         << " (" << espTopCitas << " citas)</td></tr>";

    // Promedio edad
    std::ostringstream oss;
    oss << std::fixed; oss.precision(1);
    oss << promedioEdad;
    file << "<tr><td>Promedio de edad de los pacientes</td><td>"
         << oss.str() << " a&ntilde;os</td></tr>";

    file << "</table>";

    // ── Sección B: distribución por especialidad ────────────────
    file << "<h2>Secci&oacute;n B &mdash; Distribuci&oacute;n de Carga por Especialidad</h2>";
    file << "<table class='esp-table'>";
    file << "<tr><th>Especialidad</th><th>M&eacute;dicos</th><th>Citas</th>"
         << "<th>Pacientes</th><th>Barra de ocupaci&oacute;n</th></tr>";

    for (auto& [esp, stat] : espStats) {
        int pct = totalCitasHospital > 0
                      ? (stat.numCitas * 100 / totalCitasHospital)
                      : 0;
        std::string barClass = pct >= 80 ? "bar-fill high" : "bar-fill";

        file << "<tr>"
             << "<td><b>" << esp << "</b></td>"
             << "<td>" << stat.numMedicos << "</td>"
             << "<td>" << stat.numCitas << "</td>"
             << "<td>" << stat.pacientesUnicos.size() << "</td>"
             << "<td>"
             << "<div class='bar-wrap'>"
             << "<div class='" << barClass << "' style='width:" << pct << "%'></div>"
             << "</div>"
             << "<span class='pct-label" << (pct>=80?" warn":"") << "'>" << pct << "%</span>"
             << "</td>"
             << "</tr>";
    }
    file << "</table>";
    file << "</body></html>";
    file.close();
}

