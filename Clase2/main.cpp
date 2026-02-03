#include <iostream>
using namespace std;
#include "operaciones.h"

int main(){
    int resultado = sumar(5, 7);
    int resultado_resta = restar(10, 3);
    cout << "El resultado de la suma es: " << resultado << endl;
    cout << "El resultado de la resta es: " << resultado_resta << endl;
}
