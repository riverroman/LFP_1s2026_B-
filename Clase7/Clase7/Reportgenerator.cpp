#include "ReportGenerator.h"
#include "ErrorManager.h"
#include "Token.h"
#include <fstream>

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