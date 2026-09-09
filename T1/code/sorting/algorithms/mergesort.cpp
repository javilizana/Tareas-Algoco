//algoritmo Merge Sort basada en el ppt visto en clases "6.1 Dividir y Conquistar (parte 1) "
#include <vector>

using namespace std;

//Funcion de fusion 
//Adaptada del ppt 6.1 diapo 31
void merge(vector<int>& T, vector<int>& A, int left, int mid, int right){
    int i1 = left;
    int i2 = mid;
    int i = left;

    while(i1 < mid && i2 <= right){
        if (T[i1] <= A[i2]){
            A[i++] = T[i1++];
        } else {
            A[i++] = T[i2++];
        }
    }

    while (i1 < mid){
        A[i++] = T[i1++];
    }
    while (i2 <= right){
        A[i++] = T[i2++];
    }
}

//Funcion recursiva merge
//Adaptada del ppt 6.1 diapo 32
void MergeSort(vector<int>& A, vector<int>& T, int left, int right){
    int n = right - left + 1;
    if (n <= 1){
        return; //caso base: sub-arreglo de tamaño 1
    }
    int mid = left + n/2;
    
    //se ordenan recursivamente ambas mitades
    MergeSort(A, T, left, mid -1);
    MergeSort(A, T, mid, right);
    
    //copiamos elementos al arreglo temporal T antes de mezclar
    for(int i = left; i <= right; i++){
        T[i] = A[i];
    }
    
    //combinamos
    merge(T, A, left, mid, right);
}

//funcion envoltorio
void sortArray(vector<int>& arr){
    if (arr.empty()){
        return;
    }
    //arreglo temporal necesario para la mezcla eficiente
    vector<int> temp(arr.size());
    MergeSort(arr, temp, 0, arr.size()-1);
}