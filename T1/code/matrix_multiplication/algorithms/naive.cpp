//algoritmo Naive basada en el ppt visto en clases "6.1 Dividir y Conquistar (parte 2)"

#include <iostream>
#include <vector>

using namespace std;

//algoritmo naive para la mult de matrices cuadradas 
vector<vector<int>> NaiveMatrixMult(const vector<vector<int>>& A, const vector<vector<int>>& B){
    
    int n = A.size();

    //Inicializamos la matriz resultante C con puros 0s
    vector<vector<int>> C (n, vector<int>(n,0));

    //iteramos sobre las filas A, columnas B y los elementos a mult
    for (int i = 0; i < n; i++){
        for (int j = 0; j < n; j++){
            for (int k = 0; k < n; k++){
                //acumulamos el calculo en la celda
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }
    return C;
}
