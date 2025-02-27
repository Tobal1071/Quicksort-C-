#include <iostream>
#include <chrono>
#include <stdlib.h>
#include <ctime>
#include <ctime>
#include <sstream>
#include <vector>
#include <iomanip>

using namespace std;
using namespace std::chrono;






void crono(int (*func)(int*, int), const char* msg) {
    //Inicializa el array con las posiciones las cuales no crearian un overflow.
    int arr[100000];
    //Tamamaño del array
    int n = sizeof(arr) / sizeof(arr[0]);
    //Rellena el array d enumeros randoms para ordenarlos
    for (int i = 0; i < n; i++) {
        arr[i] = rand() % 1000; // Números aleatorios entre 0 y 999
    }
    //Se encarga de obtener el tiempo actual 
    high_resolution_clock::time_point t1 = high_resolution_clock::now();
    //Ejecuta la funcion de ordenacion y devuelve un 0 por return
    int resultado = func(arr, n);
    //Recoge el tiempo del equipo con la mayor precision posible
    high_resolution_clock::time_point t2 = high_resolution_clock::now();
    //Calcula la diferencia de dos tiempos.
    std::chrono::duration<double> duration_seconds = t2 - t1;
    //Se obtienen los microsegundos
    std::chrono::microseconds duration_microseconds = std::chrono::duration_cast<std::chrono::microseconds> (duration_seconds);
    //Se imprimen los resultados obtenidos
    cout << "Resultado " << ": " << duration_seconds.count() * 1000 << " miliseconds" << endl;
    cout << "Resultado " <<  ": " << duration_seconds.count() << " seconds" << endl;
    cout << "Resultado " <<  ": " << duration_microseconds.count() << "us" << endl;
   
}

//Se encarga de obetener la hora del ordenador
std::string getFormattedTime() {
    auto now = system_clock::to_time_t(system_clock::now());
    std::tm tmStruct;

#ifdef _WIN32
    localtime_s(&tmStruct, &now);  // Windows
#else
    localtime_r(&now, &tmStruct);  // Linux/macOS
#endif

    std::ostringstream oss;
    oss << std::put_time(&tmStruct, "%Y-%m-%d %H:%M:%S");
    return oss.str();
}


// Función para intercambiar elementos
void swap(int& a, int& b) {
    int temp = a;
    a = b;
    b = temp;
}

// Función para la partición
int partition(int arr[], int low, int high) {
    int pivot = arr[high];
    int i = (low - 1);

    for (int j = low; j < high; j++) {
        if (arr[j] < pivot) {
            i++;
            swap(arr[i], arr[j]);
        }
    }
    swap(arr[i + 1], arr[high]);
    return (i + 1);
}

// Función recursiva de Quicksort
void quicksort(int arr[], int low, int high) {
    if (low < high) {
        int pi = partition(arr, low, high);
        quicksort(arr, low, pi - 1);
        quicksort(arr, pi + 1, high);
    }
}

// Función wrapper para usar con crono
int quicksortWrapper(int* arr, int length) {
    quicksort(arr, 0, length - 1);
    return 0; // No tiene un valor de retorno real, solo para cumplir con la firma
}




void testFunction() {
    //Inicia el programa y obtenemos el tiempo
    std::cout << "Inicio: " << getFormattedTime() << std::endl;
    //Medimos el tiempo de ejecucion
    auto start = high_resolution_clock::now();
    crono(quicksortWrapper, "Quicksort");
    // Medir tiempo de ejecución con crono
    auto end = high_resolution_clock::now();
    std::cout << "Fin: " << getFormattedTime() << std::endl;
    
    
    /*La lista ordenada no la imprimo ya que con los elementos que tiene es un follon*/
 
     
}

int main() {
    testFunction();  // Llamamos a la función que prueba el Quicksort
    return 0;
}
