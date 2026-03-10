#include <iostream>
#include <fstream>
using namespace std;

#include "lexer/LexicalAnalyzer.h" // Analizador léxico
#include "reports/ReportGenerator.h" // Generador de reportes

int main(){

    std::string input = 
        "paciente : @!# \"Ana\" \n"
        "[edad : 45] +" ;

    LexicalAnalyzer lexer(input);
    lexer.analyze();

    ReportGenerator::generateTokenReport(lexer.tokens, "token_report.html");
    ReportGenerator::generateErrorReport(lexer.errors, "error_report.html");

    return 0;
}
