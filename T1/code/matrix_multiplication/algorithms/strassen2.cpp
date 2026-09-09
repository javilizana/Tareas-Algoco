//algoritmo Strassen basada en el ppt visto en clases "6.1 Dividir y Conquistar (parte 2)" y en
// el algoritmo de esta pag web: https://www.geeksforgeeks.org/dsa/easy-way-remember-strassens-matrix-equation/
//la base matematica y estructural del codigo viene del ppt


#include <iostream>
#include <vector>

using namespace std;


//cambio para optimizacion 1: usando naive
//umbral: por debajo de este tamaño, usamos naive en vez de seguir recursando Strassen
static const int UMBRAL_STRASSEN = 64;


//funcion aux para sumar 2 matrices
vector<vector<int>> sumar(const vector<vector<int>>& A, const vector<vector<int>>& B){
    int n = A.size();
    vector<vector<int>> C(n, vector<int>(n,0));
    for(int i = 0; i < n; i++){
        for (int j = 0; j < n; j++){
            C[i][j] = A[i][j] + B[i][j];
        }
    }
    return C;
}

//funcion aux para restar 2 matrices
vector<vector<int>> restar(const vector<vector<int>>& A, const vector<vector<int>>& B){
    int n = A.size();
    vector<vector<int>> C(n, vector<int>(n,0));
    for (int i = 0; i < n; i++){
        for (int j = 0; j < n; j++){
            C[i][j] = A[i][j] - B[i][j];
        }
    }
    return C;
}


//cambio para optimizacion 1: usando naive
//multiplicacion naive usada como caso base cuando n <= UMBRAL_STRASSEN
vector<vector<int>> multiplicarNaive(const vector<vector<int>>& A, const vector<vector<int>>& B){
    int n = A.size();
    vector<vector<int>> C(n, vector<int>(n,0));
    for (int i = 0; i < n; i++){
        for (int j = 0; j < n; j++){
            for (int k = 0; k < n; k++){
                C[i][j] += A[i][k] * B[i][k];
            }
        }
    }
    return C;
}



//algoritmo de strassen
vector<vector<int>> Strassen(const vector<vector<int>>& M1, const vector<vector<int>>& M2){
    int n = M1.size();

    //caso base: si la matriz es 1x1, se retorna el producto de sus unicos elementos
    if (n == 1){
        return {{M1[0][0] * M2[0][0]}};
    }

    //caso base ampliado: para submatrices muy pequeñas usar naive es mas rapido
    //que seguir dividiendo 
    if (n <= UMBRAL_STRASSEN){
        return multiplicarNaive(M1, M2);
    }

    int mitad = n/2;

    //inicializacion de las submatrices n/2 x n/2
    vector<vector<int>> A(mitad, vector<int>(mitad));
    vector<vector<int>> B(mitad, vector<int>(mitad));
    vector<vector<int>> C(mitad, vector<int>(mitad));
    vector<vector<int>> D(mitad, vector<int>(mitad));
    vector<vector<int>> E(mitad, vector<int>(mitad));
    vector<vector<int>> F(mitad, vector<int>(mitad));
    vector<vector<int>> G(mitad, vector<int>(mitad));
    vector<vector<int>> H(mitad, vector<int>(mitad));

    //dividimos las matrices M1 y M2 en 4 submatrices c/u (de las que ya generamos)
    for(int i = 0; i < mitad; i++){
        for(int j = 0; j < mitad; j++){
            A[i][j] = M1[i][j];
            B[i][j] = M1[i][j + mitad];
            C[i][j] = M1[i + mitad][j];
            D[i][j] = M1[i + mitad][j + mitad];

            E[i][j] = M2[i][j];
            F[i][j] = M2[i][j + mitad];
            G[i][j] = M2[i + mitad][j];
            H[i][j] = M2[i + mitad][j + mitad];
        }
    }

    //Calculamos los 7 productos P1 a P7 recursivamente siguiendo las ec de strassen
    vector<vector<int>> P1 = Strassen(A, restar(F,H));
    vector<vector<int>> P2 = Strassen(sumar(A,B), H);
    vector<vector<int>> P3 = Strassen(sumar(C,D), E);
    vector<vector<int>> P4 = Strassen(D, restar(G, E));
    vector<vector<int>> P5 = Strassen(sumar(A,D), sumar(E,H));
    vector<vector<int>> P6 = Strassen(restar(B, D), sumar(G,H));
    vector<vector<int>> P7 = Strassen(restar(A,C), sumar(E,F));

    //combinamos las submatrices resultantes en los cuadrantes finales
    vector<vector<int>> C11 = sumar(restar(sumar(P5, P4), P2), P6);
    vector<vector<int>> C12 = sumar(P1, P2);
    vector<vector<int>> C21 = sumar(P3, P4);
    vector<vector<int>> C22 = restar(restar(sumar(P1, P5), P3), P7);
    
    //armamos la matriz final resultante de tamaño n x n
    vector<vector<int>> Resultado(n, vector<int>(n));
    for(int i = 0; i < mitad; i++){
        for(int j = 0; j < mitad; j++){
            Resultado[i][j] = C11[i][j];
            Resultado[i][j + mitad] = C12[i][j];
            Resultado[i + mitad][j] = C21[i][j];
            Resultado[i + mitad][j + mitad] = C22[i][j];
        }
    }
    return Resultado;
}



