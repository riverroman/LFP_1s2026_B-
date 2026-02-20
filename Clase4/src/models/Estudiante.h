#ifndef ESTUDIANTE_H
#define ESTUDIANTE_H

#include <string>

/**
 * @brief Modelo que representa a un estudiante registrado en el sistema.
 *
 * BUENA PRÁCTICA: Se usan structs para modelos de datos simples (solo datos,
 * sin lógica de negocio). Para lógica compleja, se usarían clases.
 */
struct Estudiante {
    int         carnet;
    std::string nombre;
    std::string apellido;
    std::string carrera;
    int         semestre;

    // Constructor con valores por defecto (buena práctica: siempre inicializar)
    Estudiante()
        : carnet(0), nombre(""), apellido(""), carrera(""), semestre(0) {}

    Estudiante(int c, const std::string& n, const std::string& a,
               const std::string& car, int sem)
        : carnet(c), nombre(n), apellido(a), carrera(car), semestre(sem) {}

    // Nombre completo como método de conveniencia
    std::string nombreCompleto() const {
        return nombre + " " + apellido;
    }
};

#endif