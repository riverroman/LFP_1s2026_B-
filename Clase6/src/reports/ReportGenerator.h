#pragma once
#include <vector>
#include "../model/Token.h"
#include "../errors/ErrorManager.h"
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
};