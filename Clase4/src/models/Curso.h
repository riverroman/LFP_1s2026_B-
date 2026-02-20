#ifndef CURSO_H
#define CURSO_H

#include <string>

/**
 * @brief Modelo que representa un curso disponible en el sistema académico.
*/

struct Curso {
    int         codigo;
    std::string nombre;
    int         creditos;
    int         semestre;
    std::string carrera;

    Curso()
        : codigo(0), nombre(""), creditos(0), semestre(0), carrera("") {}

    Curso(int cod, const std::string& n, int cred, int sem, const std::string& car)
        : codigo(cod), nombre(n), creditos(cred), semestre(sem), carrera(car) {}
};
#endif