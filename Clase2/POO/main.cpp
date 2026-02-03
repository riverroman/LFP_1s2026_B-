#include <iostream> // Para imprimir en pantalla
#include <string> // Para usar la clase string
using namespace std; // Evita tener que escribir std:: antes de cada cosa de la biblioteca estándar

// ======================================
// 1. Definición de la Clase
// ======================================
class Coche {
private:
// ================================================================================================
// Atributos (Variables): Están encapsulados, nadie fuera de la clase puede cambiarlos directamente
// ================================================================================================
string marca;
    string modelo;
    int velocidad;

public:
    // ==========================================================
    // Constructor: Se ejecuta automáticamente al crear un objeto
    // ==========================================================
    Coche(string m, string mod) {
        marca = m;
        modelo = mod;
        velocidad = 0; // El coche empieza detenido
    }
    // ==================================================
    // Método: Acelerar
    // ==================================================
    void acelerar(int cantidad) {
        velocidad += cantidad;
        cout << "El " << marca << " acelero. Velocidad actual: " << velocidad << " km/h." << endl;
    }
    // ==================================================
    // Método: Frenar
    // ==================================================
    void frenar(int cantidad) {
        if (cantidad > velocidad) {
            velocidad = 0;
        } else {
            velocidad -= cantidad;
        }
        cout << "El " << marca << " freno. Velocidad actual: " << velocidad << " km/h." << endl;
    }
    // ==================================================
    // Método: Mostrar información
    // ==================================================
    void mostrarInfo() {
        cout << "--- Info del Coche ---" << endl;
        cout << "Marca: " << marca << endl;
        cout << "Modelo: " << modelo << endl;
        cout << "----------------------" << endl;
    }
};

int main() {
    // ==================================================
    // 2. Creación de Objetos (Instancias)
    // Creamos dos coches distintos usando la misma clase
    // ==================================================
    Coche miCoche("Toyota", "Corolla");
    Coche tuCoche("Ford", "Mustang");
    
    // ==================================================
    // 3. Uso de los objetos
    // ==================================================
    miCoche.mostrarInfo();
    miCoche.acelerar(50);
    miCoche.acelerar(20);
    miCoche.frenar(10);
    
    // ==================================================
    cout << endl; // Espacio visual es decir una línea en blanco
    // ==================================================
    tuCoche.mostrarInfo();
    tuCoche.acelerar(100);
    return 0;
}