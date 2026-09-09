//algoritmo Quick Sort basada en el ppt visto en clases "6.1 Dividir y Conquistar (parte 1) "
#include <vector>
#include <algorithm>

using namespace std;

//funcion de particion
int particion(vector<int>& A, int i, int d, int p){
    i--;
    d++;
    do {
        i++;
        while (A[i] < A[p]){
            i++;
        }
        d--;
        while (i < d && A[d] > A[p]){
            d--;
        }
        if (i < d){
            swap(A[i], A[d]);
        }
    } while (i < d);
    
    return i;
}

//funcion recursiva de QuickSort
void QuickSort(vector<int>& A, int i, int d){
    int n = d -i +1;
    if (n <= 1){
        return;
    } else {
        int p = i + n/2;
        swap(A[p], A[d]);
        int k = particion(A, i, d-1, d);
        swap(A[k], A[d]);
        if (k == d-1){
            k = d;
        }
        QuickSort(A, i, k-1);
        QuickSort(A, k+1, d);
    }
}

//fusion envoltorio
void sortArray(vector<int>& arr){
    if (!arr.empty()){
        QuickSort(arr, 0, arr.size() -1);
    }
}