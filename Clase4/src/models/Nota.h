#ifndef NOTA_H
#define NOTA_H

#include <string>

/**
 * @brief Modelo que representa una calificación de un estudiante en un curso.
 */
struct Nota {
    int         carnet;
    int         codigoCurso;
    double      nota;
    std::string ciclo;
    int         anio;

    Nota()
        : carnet(0), codigoCurso(0), nota(0.0), ciclo(""), anio(0) {}

    Nota(int c, int cod, double n, const std::string& cic, int a)
        : carnet(c), codigoCurso(cod), nota(n), ciclo(cic), anio(a) {}
        
    bool estaAprobado() const {
        return nota >= 61.0;
    }
};

#endif