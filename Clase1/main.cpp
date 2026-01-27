#include <iostream>
using namespace std;

int main() {
    cout << "Hello, World - LFP" << endl;
    // Uso de la funciones esPar
    int numero; // Declaracion de variable sin valor y con valor 
    cout << "Ingrese un numero entero: ";
    cin >> numero;
    int numero2;
    cout << "Ingrese otro numero entero: ";
    cin >> numero2;
    // Uso de la funcion suma
    int suma(int num1, int num2); // Prototipo de la funcion 
    int resultado = suma(numero, numero2);  
    cout << "La suma de " << numero << " y " << numero2 << " es: " << resultado << endl;
    return 0; // No estamos retornando nada 
}

int suma(int num1, int num2) {
    return num1 + num2;
}   

// Variables sin valor inicial - tiene un valor por defecto 
// int numero; // Declaracion de variable sin valor 0

// Variables con valor inicial
// int num1 = 10; // Declaracion de variable con valor


// Pasos para compilar y ejecutar:
// 1. Abre una terminal.
// 2. Navega al directorio donde se encuentra el archivo main.cpp.
// 3. Compila el programa usando el siguiente comando:
//    g++ main.cpp -o main 
// 4. Ejecuta el programa con el comando:
//    ./main