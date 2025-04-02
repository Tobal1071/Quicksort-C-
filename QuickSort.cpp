// Middle QuickSort

#include <iostream>
#include <stdlib.h>
#include <chrono>
#include <math.h>

using namespace std;
using namespace chrono;

void
middle_QuickSort(int* v, long left, long right) {

    long i, j;
    int pivot, aux;
    if (left < right)
    {
        i = left; j = right;
        pivot = v[(i + j) / 2];
        do
        {
            while (v[i] < pivot) i++;
            while (v[j] > pivot) j--;
            if (i <= j)
            {
                aux = v[i]; v[i] = v[j]; v[j] = aux;
                i++; j--;
            }
        } while (i <= j);
        if (left < j)  middle_QuickSort(v, left, j);
        if (i < right) middle_QuickSort(v, i, right);
    }
}

int main() {

    srand(0);

    cout << "# QuickSort CPU-times in milliseconds:" //apartado d
        << endl
        << "# Size \t\t Average CPU time (ms.) \t Average CPU time (us.)"
        << endl
        << "# -------------------------------------------------------------------------"
        << endl;

    for (int exp = 15; exp <= 20; exp++) {
        long size = long(pow(2, exp)); // size guarda el tamañp 2^exp
        int* v = new int[size]; //Memoria dinámica
        if (!v) {
            exit(EXIT_FAILURE);
        }
        long total_time = 0;
        for (int contador = 0; contador < 30; contador++) { // Hace la prueba 30 veces
            for (long j = 0; j < size; j++) // 
                v[j] = rand(); // Le da valores aleatorios a v

            //Recoje el tiempo que tarda en ordenar los 2^exp números
            auto start = steady_clock::now();
            middle_QuickSort(v, 0, size - 1);
            auto end = steady_clock::now();
            //Guarda el tiempo total de las 30 pruebas en milisegundos
            total_time += duration_cast<milliseconds>(end - start).count();
        }
        // Divide en 30 para hacer una media y muestra los tiempos en milisegundos y segundos
        cout << size << "\t\t\t" << (total_time / 30.0) << "\t\t\t\t" << (1000 * total_time / 30) << endl;
        delete[] v;
    }
}
