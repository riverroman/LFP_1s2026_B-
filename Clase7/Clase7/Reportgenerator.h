#pragma once
#include <vector>
#include "Token.h"
#include "ErrorManager.h"

class ReportGenerator {

public:
    static void generateTokenReport(
        const std::vector<Token>& tokens,
        const std::string& filename
        );
    static void generateErrorReport(
        const std::vector<LexicalError>& errors,
        const std::string& filename
        );
    static void generateStatisticsReport(
        const std::vector<Token>& tokens,
        const std::string& filename
        );
};