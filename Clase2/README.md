```
    #include <iostream>
    using namespace std;

    // EL LENGUAJE INTERPRETADO TIENE VARIAS LECTURAS Y EJECUCIONES <- Primer ciclo -> Se realiza reconocimiento de las variables, funciones, clases, etc. 
    // EL LENGUAJE COMPILADO TRADUCE TODO EL CODIGO A CODIGO

    // Prototipos de declaraciones
    void menu();
    double sumar(double a, double b);
    int espar(int numero);

    // No tiene retorno
    void menu(){
        cout << "==========================" << endl;
        cout << "Menu de opciones" << endl;
        cout << "1. Sumar dos numeros" << endl;
        cout << "2. Calculo de numero par" << endl;
        cout << "3. Salir" << endl;
        cout << "==========================" << endl;
    }

    // tipo nombre (parametros)
    double sumar(double a, double b){
        // retorno valor;
        return a + b; // De tipo double
    }   

    int espar(int numero){
        // Retorna 1 si es par, 0 si es impar   
        // Si el numero es 100 % 2 == 0 -> es par
        return numero % 2 == 0;
    }

    // Tipos boleano en c++ 1 -> true, 0 -> false


    int main(){
        cout << "Hola Mundo desde C++" << endl;
        // Declaracion de variable - para almacenar la opcion del usuario
        int opcion;
        do{
            menu();
            cout << "Ingrese una opcion: ";
            // Java - Scanner entrada = new Scanner(System.in);
            cin >> opcion;
            if(opcion == 1){
                double num1, num2;
                cout << "Ingrese el primer numero: ";
                cin >> num1;
                cout << "Ingrese el segundo numero: ";
                cin >> num2;
                double resultado = sumar(num1, num2);
                cout << "El resultado de la suma es: " << resultado << endl;
            }else if(opcion == 2){
                // Se captura un numero entero
                int numero;
                // Salida por consola
                cout << "Ingrese un numero entero: ";
                // Se captura el numero por consola
                cin >> numero;
                // Llamar a la funcion espar
                // Si es par 1 -> true
                if(espar(numero)){
                    cout << "El numero " << numero << " es par." << endl;
                // Si no es par 0 -> false
                }else{
                    cout << "El numero " << numero << " es impar." << endl; 
                }
            }else if(opcion != 3){
                cout << "Opcion no valida. Intente de nuevo." << endl;
            }
        }while(opcion != 3);
        return 0;
    }