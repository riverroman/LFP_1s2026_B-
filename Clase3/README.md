```
    // Arrays
    // ====== Array con limite fijo =========
    int arr[5] = {1, 2, 3, 4, 5};
    cout << "Array elements: ";
    for(int i = 0; i < 5; i++) {
        cout << arr[i] - 1<< " ";
    }
    cout << endl;
    arr[0] = 10; // Modificando el primer elemento del array
    cout << "Primer Elemento: " << arr[0] << endl;


    // ====== Array con limite fijo =========
    string fruits[] = {"Apple", "Banana", "Cherry", "Date", "Elderberry"};
    cout << "Fruits: ";
    for(int i = 0; i < 5; i++) {
        cout << fruits[i] << " ";
    }
    cout << endl;


    // Arrays
    int numbers[5] = {1, 2, 3, 4, 5};
    int size = sizeof(numbers) / sizeof(numbers[0]);
                // Calculo matematico para obtener el tamaño del array
                // sizeof(numbers) devuelve el tamaño total del array en bytes
                // sizeof(numbers[0]) devuelve el tamaño de un elemento del array en bytes
                // Ejemplo 5 elementos * 4 bytes por elemento= 20 bytes total, entonces 20 / 4 = 5 elementos

    cout << "Array elements: ";
    for (int i = 0; i < size; i++) {
        cout << numbers[i] << " ";
    }
    cout << endl;