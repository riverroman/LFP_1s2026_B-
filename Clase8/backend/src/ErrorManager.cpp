#include "ErrorManager.h"

void ErrorManager::agregarError(TipoError tipo, const std::string& lexema, const std::string& descripcion, int linea, int columna) {
    contador++;
    errores_.push_back({contador, lexema, tipo, descripcion, linea, columna});
}