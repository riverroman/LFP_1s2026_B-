#ifndef FILE_READER_H
#define FILE_READER_H

#include <vector>
#include <string>
#include "../models/Estudiante.h"
#include "../models/Curso.h"
#include "../models/Nota.h"

/**
 * @brief Servicio encargado de leer y parsear los archivos .lfp del sistema.
 *
 * BUENA PRÁCTICA: Separar la responsabilidad de lectura de archivos en su
 * propia clase/servicio (Principio de Responsabilidad Única - SRP).
 * El resto del sistema no necesita saber CÓMO se leen los archivos.
 */
class FileReader {
public:
    /**
     * @brief Lee el archivo de estudiantes y retorna un vector de Estudiante.
     * @param ruta  Ruta completa al archivo estudiantes.lfp
     * @return Vector con los estudiantes leídos. Vacío si hay error.
     */
    static std::vector<Estudiante> leerEstudiantes(const std::string& ruta);

    /**
     * @brief Lee el archivo de cursos y retorna un vector de Curso.
     * @param ruta  Ruta completa al archivo cursos.lfp
     * @return Vector con los cursos leídos. Vacío si hay error.
     */
    static std::vector<Curso> leerCursos(const std::string& ruta);

    /**
     * @brief Lee el archivo de notas y retorna un vector de Nota.
     * @param ruta  Ruta completa al archivo notas.lfp
     * @return Vector con las notas leídas. Vacío si hay error.
     */
    static std::vector<Nota> leerNotas(const std::string& ruta);

private:
    // Constructor privado: esta clase solo tiene métodos estáticos (utilidad pura)
    FileReader() = delete;
};

#endif // FILE_READER_H