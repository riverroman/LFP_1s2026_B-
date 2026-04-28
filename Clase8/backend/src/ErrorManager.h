#pragma once
#include <string>
#include <vector>

enum class TipoError {LEXICO, SINTACTICO};

struct ErrorEntry{
   int numero;
   std::string lexema;
   TipoError tipo;
   std::string descripcion;
   int linea;
   int columna;  
};

// Ejemplo de ErrorEntry:  1. LEXICO: "Caracter no reconocido" en la línea 5, columna 10.

class ErrorManager {
public:
    void agregarError(TipoError tipo, const std::string& lexema, const std::string& descripcion, int linea, int columna);

    // No se necesita un método para obtener los errores, ya que se pueden imprimir directamente desde el vector de errores.
    bool tieneErrores() const {return !errores_.empty();};
    const std::vector<ErrorEntry>& getErrores() const { return errores_; }
    void limpiarErrores() { errores_.clear(); contador = 0; }

private:
    std::vector<ErrorEntry> errores_;
    int contador = 0;
  
};




