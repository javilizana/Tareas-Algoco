#include <iostream>
#include <vector>
#include <string>
#include <chrono>
#include <sys/resource.h>
#include <cstdio> 

using namespace std;

void sortArray(vector<int>&arr);

//funcion para obtener el consumo max de memoria residente en kilobyes
long obtenerMaxMemoria(){
    struct rusage uso;
    if(getrusage(RUSAGE_SELF, &uso) == 0){
        return uso.ru_maxrss;
    }
    return 0;
}


int main(int argc, char* argv[]){
    if (argc < 4){
        cerr << "Uso: " << argv[0] << " <archivo_input> <nombre_algoritmo> <archivo_mediciones> [archivo_output]\n";
        return 1;
    }

    string ruta_input = argv[1];
    string nombre_algoritmo = argv[2];
    string ruta_mediciones = argv[3];
    string ruta_output = (argc >= 5) ? argv[4] : "";

    //cargamos el arreglo desde el archivo de entrada
    vector<int> arr;
    FILE* archivo_in = fopen(ruta_input.c_str(), "r");
    if (!archivo_in) {
        cerr << "Error al abrir el archivo de entrada: " << ruta_input << "\n";
        return 1;
    }
    
    int elemento;
    while (fscanf(archivo_in, "%d", &elemento) == 1) {
        arr.push_back(elemento);
    }
    fclose(archivo_in);

    size_t n = arr.size();

    //medimos el tiempo de ejecucion durante el ordenamiento
    auto inicio = chrono::high_resolution_clock::now();
    sortArray(arr); 
    auto fin = chrono::high_resolution_clock::now();
    chrono::duration<double, milli> duracion_ms = fin - inicio;

    long memoria_kb = obtenerMaxMemoria();

    if(!ruta_output.empty()){
        FILE* archivo_out = fopen(ruta_output.c_str(), "w");
        if(archivo_out){
            for(size_t i = 0; i < arr.size(); i++){
                fprintf(archivo_out, "%d%s", arr[i], (i + 1 == arr.size() ? "" : " "));
            }
            fprintf(archivo_out, "\n");
            fclose(archivo_out);
        }
    }

    //guardamos las metricas
    FILE* archivo_csv = fopen(ruta_mediciones.c_str(), "a");
    if(archivo_csv){
        //si el archivo est  vacio, escribimos el encabezado
        fprintf(archivo_csv, "%s,%s,%zu,%f,%ld\n", ruta_input.c_str(), nombre_algoritmo.c_str(), n, duracion_ms.count(), memoria_kb);
        fclose(archivo_csv);
    }
    
    cout << "[" << nombre_algoritmo << "] N = " << n << " | Tiempo: " << duracion_ms.count() << " ms" << " | Memoria: " << memoria_kb << " Kb\n";
    return 0;
}