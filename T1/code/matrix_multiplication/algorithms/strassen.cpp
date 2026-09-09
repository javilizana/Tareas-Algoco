//algoritmo Strassen basada en el ppt visto en clases "6.1 Dividir y Conquistar (parte 2)" y en
// el algoritmo de esta pag web: https://www.geeksforgeeks.org/dsa/easy-way-remember-strassens-matrix-equation/
//la base matematica y estructural del codigo viene del ppt


#include <iostream>
#include <vector>

using namespace std;

//cambio para optimizacion 2:
//Representacion plana de una matriz n x n: acceso mediante M[i*n + j]
using Matriz = vector<int>;

//funcion aux para sumar 2 matrices (representacion plana)
Matriz sumar(const Matriz& A, const Matriz& B, int n){
    Matriz C(n * n);

    for(int i = 0; i < n * n; n++){
        C[i] = A[i] + B[i];
    }
    return C;
}

//funcion aux para restar 2 matrices (representacion plana)
Matriz restar(const Matriz& A, const Matriz& B, int n){
    Matriz C(n * n);

    for(int i = 0; i < n * n; n++){
        C[i] = A[i] - B[i];
    }

    return C;
}

//extrae una submatriz de tamaño mitad x mitad desde la esquina (filaOffset, colOffset) de M (de tamaño n x n)
Matriz extraerSubM(const Matriz& M, int n, int mitad, int filaOffset, int colaOffset){
    Matriz Sub(mitad * mitad);
    for(int i = 0; i < mitad; i++){
        for(int j = 0; j < mitad; j++){
            Sub[i * mitad + j] = M[(i + filaOffset) * n + (j + colaOffset)];
        }
    }
    return Sub;
}

//algoritmo de strassen (representacion plana), recibe y retorna matrices de tamaño n x n
Matriz Strassen(const Matriz& M1, const Matriz& M2, int n){

    //caso base: si la matriz es 1x1, se retorna el producto de sus unicos elementos
    if (n == 1){
        return {{M1[0] * M2[0]}};
    }

    int mitad = n/2;

    //extraemos las 4 submatrices de M1 y M2 (A,B,C,D y E,F,G,H respectivamente)
    Matriz A = extraerSubM(M1, n, mitad, 0, 0);
    Matriz B = extraerSubM(M1, n, mitad, 0, mitad);
    Matriz C = extraerSubM(M1, n, mitad, mitad, 0);
    Matriz D = extraerSubM(M1, n, mitad, mitad, mitad);

    Matriz E = extraerSubM(M2, n, mitad, 0, 0);
    Matriz F = extraerSubM(M2, n, mitad, 0, mitad);
    Matriz G = extraerSubM(M2, n, mitad, mitad, 0);
    Matriz H = extraerSubM(M2, n, mitad, mitad, mitad);

    //Calculamos los 7 productos P1 a P7 recursivamente siguiendo las ec de strassen
    Matriz P1 = Strassen(A, restar(F, H, mitad), mitad);
    Matriz P2 = Strassen(sumar(A, B, mitad), H, mitad);
    Matriz P3 = Strassen(sumar(C, D, mitad), E, mitad);
    Matriz P4 = Strassen(D, restar(G, E, mitad), mitad);
    Matriz P5 = Strassen(sumar(A, D, mitad), sumar(E, H, mitad), mitad);
    Matriz P6 = Strassen(restar(B, D, mitad), sumar(G, H, mitad), mitad);
    Matriz P7 = Strassen(restar(A, C, mitad), sumar(E, F, mitad), mitad);

    //combinamos las submatrices resultantes en los cuadrantes finales
    Matriz C11 = sumar(restar(sumar(P5, P4, mitad), P2, mitad), P6, mitad);
    Matriz C12 = sumar(P1, P2, mitad);
    Matriz C21 = sumar(P3, P4, mitad);
    Matriz C22 = restar(restar(sumar(P1, P5, mitad), P3, mitad), P7, mitad);

    //armamos la matriz final resultante de tamaño n x n directamente sobre el arreglo plano
    Matriz Resultado(n * n);
    for (int i = 0; i < mitad; i ++){
        for(int j = 0; j < mitad; j++){
            Resultado[i * n + j] = C11[i * mitad + j];
            Resultado[i * n + (j + mitad)] = C12[i * mitad + j];
            Resultado[(i + mitad) * n + j] = C21[i * mitad + j];
            Resultado[(i + mitad) * n + (j + mitad)] = C22[i * mitad + j];
        }
    }
    
    return Resultado;
}