#include <iostream>
#include <stdlib.h>
#include <chrono>
#include <math.h>
#include <iomanip>
#include <xmmintrin.h> 

using namespace std;
using namespace chrono;

extern "C" void __cdecl middle_QuickSortSSE(int* v, long left, long right);

void middle_QuickSort(int* v, long left, long right) {
    long i, j;
    int pivot, aux;
    if (left < right) {
        i = left; j = right;
        pivot = v[(i + j) / 2];
        do {
            while (v[i] < pivot) i++;
            while (v[j] > pivot) j--;
            if (i <= j) {
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

    cout << "# QuickSort CPU-times in milliseconds:" << endl
         << "# " << setw(10) << "Size\t\t"
         << setw(20) << "Average CPU time (ms.)\t\t"
         << setw(20) << "Average CPU time (s)\t\t"
         << setw(20) << "Average CPU time (us.)\t\t" << endl
         << "# --------------------------------------------------------------------------------------" << endl;

    for (int exp = 15; exp <= 20; exp++) {
        long size = long(pow(2, exp));
        int* v = new int[size];
        if (!v) {
            cerr << "Error, not enough memory!" << endl;
            exit(EXIT_FAILURE);
        }

        long total_time = 0;

        for (int contador = 0; contador < 30; contador++) {
            for (long j = 0; j < size; j++)
                v[j] = rand();

            auto start = steady_clock::now();
            middle_QuickSortSSE(v, 0, size - 1);
            auto end = steady_clock::now();

            total_time += duration_cast<nanoseconds>(end - start).count();
        }

        cout << "\t" << size << "\t\t"
            << std::setprecision(5) << (total_time / 30.0 / 1000) << " ms\t\t"
            << "\t\t" << std::setprecision(5) << (total_time / 30.0 / 1000000) << " s\t\t"
            << std::setprecision(5) << (total_time / 30.0 * 1000) << " us"
            << endl;
        delete[] v;
    }
}


extern "C" void __cdecl middle_QuickSortSSE(int* v, long left, long right) {
    __asm {
        push ebp
        mov ebp, esp
        sub esp, 4  

        mov eax, left  
        mov ebx, right  
        cmp eax, ebx
        jge fin

        mov ecx, eax
        add ecx, ebx
        shr ecx, 1
        mov edx, v
        mov edi, [edx + ecx * 4]  

        mov esi, eax  
        mov ebp, ebx  

        bucle_do_while:
        buscar_i:
            cmp esi, ebp
            jge recursion

            movaps xmm0, [edx + esi * 4]  
            pcmpgtd xmm0, xmm1            
            jge buscar_j
            inc esi
            jmp buscar_i

        buscar_j:
            cmp esi, ebp
            jge recursion

            movaps xmm0, [edx + ebp * 4]
            pcmpgtd xmm1, xmm0            
            jle intercambio
            dec ebp
            jmp buscar_j

        intercambio:
            mov ecx, [edx + esi * 4]
            mov ebx, [edx + ebp * 4]
            movaps xmm0, xmm1
            movaps xmm1, xmm0
            inc esi
            dec ebp
            jmp bucle_do_while

        recursion:
            push eax
            push ebx
            push edx

            push ebp
            push eax
            push edx
            call middle_QuickSortSSE
            add esp, 12

            pop edx
            pop ebx
            pop eax

            push ebx
            push esi
            push edx
            call middle_QuickSortSSE
            add esp, 12

        fin:
            add esp, 4
            pop ebp
    }
}
