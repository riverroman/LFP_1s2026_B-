#include <iostream>
#include <string>
#include <fstream> // Permite la lectura y escritura de archivos (ifstream, ofstream)
                   // ifstream: para leer archivos
                   // ofstream: para escribir archivos 
using namespace std;

void mostrarArchivoJugadores(){

    // Abre el archivo "jugadores.txt" para lectura (mismo directorio del proyecto)
    // Ruta relativa
    ifstream archivo("main.txt");  // <- Abriendo el archivo

    // ifstream es una clase que permite manejar archivos para lectura. El constructor de ifstream toma el nombre del archivo como argumento y lo abre para su uso.

    // Verifica si el archivo se abrió correctamente
    if(!archivo.is_open()){
        cout << "No se pudo abrir el archivo." << endl;
        return; // Salir de la función si no se pudo abrir el archivo
    }
       
    // Declaracion de tipos 
    string nombre;
    int dorsal;

    cout << "\n Listado de Jugadores: " << endl;
    cout << "----------------------" << endl;

    // Uso de ciclo while para leer el archivo línea por línea
    while(archivo.good()){ // true -> si el estado del archivo es correcto, false si ocurrio algun error o se llego al final del archivo (EOF) -> vacio 

        getline(archivo, nombre, ',');
        // flujo -> archivo 
        // variable -> nombre
        // delimitador -> ','

        // lee el numero del dorsal despues de la coma
        archivo >> dorsal; // Lee el número del dorsal después de la coma
        // Messi,10
        //.     ^
        // Aqui se detiene el getline 

        // Ignorar el salto de linea 
        archivo.ignore(); // Ignora el siguiente carácter (en este caso, el salto de línea)

        // Si es de diferente a fail -> todo bien [ok]
        if(!archivo.fail()){
            cout << "Nombre: " << nombre << ", Dorsal: " << dorsal << endl;
        }

    }
}

int main() {
    int opcion;
    do
    {
        cout << "Menu de opciones: " << endl;
        cout << "1. Mostrar jugadores" << endl;
        cout << "2. Salir" << endl;
        cout << "Ingrese su opcion: ";
        cin >> opcion;

        switch (opcion)
        {
        case 1:
            mostrarArchivoJugadores();
            break;
        case 2:
            cout << "Saliendo del programa..." << endl;
            break;
        default:
            cout << "Opcion no valida. Intente nuevamente." << endl;
            break;
        }
    } while (opcion !=2);
    return 0;
}   