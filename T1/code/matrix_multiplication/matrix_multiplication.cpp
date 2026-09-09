#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <chrono>
#include <sstream>
#include <sys/resource.h>

using namespace std;

//declaracion de las funciones implementadas en naive.cpp y strassen.cpp
vector<vector<int>> NaiveMatrixMult(const vector<vector<int>>& A, const vector<vector<int>>& B);
vector<vector<int>> Strassen(const vector<vector<int>>& M1, const vector<vector<int>>& M2);

//funcion para obtener el consumo max de memoria residente en kilobyes
long obtenerMaxMemoria(){
    struct rusage uso;
    if(getrusage(RUSAGE_SELF, &uso) == 0){
        return uso.ru_maxrss;
    }
    return 0;
}

//funcion aux para leer una matriz desde un txt
vector<vector<int>> leerMatriz(const string& ruta){
    ifstream archivo(ruta);
    vector<vector<int>> matriz;

    if(!archivo.is_open()){
        cerr << "Error al abrir el archivo de entrada: " << ruta << "\n";
        return matriz;
    }

    string linea;
    while(getline(archivo, linea)){
        if (linea.empty()){
            continue;
        }
        vector<int> fila;
        stringstream ss(linea);
        int val;
        while (ss >> val){
            fila.push_back(val);
        }

        matriz.push_back(fila);
    }
    archivo.close();
    return matriz;
}

int main(int argc, char* argv[]){
    //optimizacion de flujos de E/S estandar para instancias masivas
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    if(argc < 5){
        cerr << "Uso: " << argv[0] << "<archivo_input_1> <archivo_input_2> <nombre_algoritmo> <archivo_mediciones> [archivo_output]\n";
        return 1;
    }

    string ruta_input1 = argv[1];
    string ruta_input2 = argv[2];
    string nombre_algoritmo = argv[3];
    string ruta_mediciones = argv[4];
    string ruta_output = (argc >= 6) ? argv[5] : "";

    //cargamos las matrices desde los archivos de entrda
    vector<vector<int>> M1 = leerMatriz(ruta_input1);
    vector<vector<int>> M2 = leerMatriz(ruta_input2);

    if(M1.empty() || M2.empty()){
        cerr << "Error: Una o ambas matrices están vacías o no pudieron ser leídas.\n";
        return 1;
    }

    size_t n = M1.size();
    vector<vector<int>> resultado;

    //medimos el tiempo de ejecucion durante la multiplicacion
    auto inicio = chrono::high_resolution_clock::now();

    if(nombre_algoritmo == "naive"){
        resultado = NaiveMatrixMult(M1, M2);
    } else if (nombre_algoritmo == "strassen"){
        resultado = Strassen(M1, M2);
    } else {
        cerr << "Error: Algoritmo '" << nombre_algoritmo << "' no reconocido. Use 'naive' o 'strassen'.\n";
        return 1;
    }

    auto fin = chrono::high_resolution_clock::now();

    chrono::duration<double, milli> duracion_ms = fin - inicio;
    long memoria_kb = obtenerMaxMemoria();

    //escribimos la matriz resultante en el archivo de salida
    if(!ruta_output.empty()){
        ofstream archivo_out(ruta_output);
        if(archivo_out.is_open()){
            for(size_t i = 0; i < resultado.size(); i++){
                for(size_t j = 0; j < resultado.size(); j++){
                    archivo_out << resultado[i][j] << (j +1 == resultado[i].size() ? "" : " ");
                }
                archivo_out << "\n";
            }
            archivo_out.close();
        }
    }

    //guardamos metricas en formato CSV compatible con plot_generator.py
    bool existe_arch = false;
    {
        ifstream check(ruta_mediciones);
        existe_arch = check.good();
    }

    ofstream archivo_csv(ruta_mediciones, ios::app);
    if(archivo_csv.is_open()){
        if(!existe_arch){
            archivo_csv << "archivo1,archivo2,algoritmo,n,tiempo_ms,memoria_kb\n";
        }
        archivo_csv << ruta_input1 << "," << ruta_input2 << "," << nombre_algoritmo << "," << n << "," << duracion_ms.count() << "," << memoria_kb << "\n";
        archivo_csv.close();
    }
    
    cout << "[" << nombre_algoritmo << "] N = " << n << " | Tiempo: " << duracion_ms.count() << " ms" << " | Memoria: " << memoria_kb << " Kb\n";
    return 0;
}   