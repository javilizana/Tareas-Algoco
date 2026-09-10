#include <iostream>
#include <vector>
#include <string>
#include <chrono>
#include <sys/resource.h>
#include <cstdio> 

#include <cstdlib>
#include <filesystem>

using namespace std;
namespace fs = std::filesystem;

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

    if (argc < 3){
        cerr << "Uso: " << argv[0] << " <carpeta_inputs> <nombre_algoritmo> [archivo_mediciones]\n";
        return 1;
    }

    string carpeta_inputs = argv[1];
    string nombre_algoritmo = argv[2];
    string ruta_mediciones = (argc >= 4) ? argv[3] : "data/measurements/mediciones.txt";
    //string ruta_output = (argc >= 5) ? argv[4] : "";

    cout << "Iniciando ordenamiento con " << nombre_algoritmo << "...\n";

    //iteramos sobre cada arch dentro de la carpeta
    for(const auto& entry : fs::directory_iterator(carpeta_inputs)){
        //filtramos solo los archivos txt
        if(entry.is_regular_file() && entry.path().extension() == ".txt"){
            string ruta_input = entry.path().string();
            vector<int> arr;

            //cargamos el arreglo
            FILE* archivo_in = fopen(ruta_input.c_str(), "r");
            if (!archivo_in){
                cerr << "Error al abrir: " << ruta_input << "\n";
                continue;
            }

            int elemento;
            while (fscanf(archivo_in, "%d", &elemento) == 1){
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

            //string ruta_output = "data/array_output/" + entry.path().filename().string();
            string ruta_output = "data/array_output/" + nombre_algoritmo + "_" + entry.path().filename().string();
            FILE* archivo_out = fopen(ruta_output.c_str(), "w");
            if(archivo_out){
                for(size_t i = 0; i < arr.size(); i++){
                    fprintf(archivo_out, "%d%s", arr[i], (i + 1 == arr.size() ? "" : " "));
                }
                fprintf(archivo_out, "\n");
                fclose(archivo_out);
            } else {
                cerr << "Error al crear el archivo de salida: " << ruta_output << "\n";
            }

            //guardamos las metricas
            FILE* archivo_csv = fopen(ruta_mediciones.c_str(), "a");
            if(archivo_csv){
                //si el archivo est  vacio, escribimos el encabezado
                fprintf(archivo_csv, "%s,%s,%zu,%f,%ld\n", ruta_input.c_str(), nombre_algoritmo.c_str(), n, duracion_ms.count(), memoria_kb);
                fclose(archivo_csv);
            }

            cout << "[" << nombre_algoritmo << "] " << entry.path().filename().string() << " -> N=" << n << " | Tiempo: " << duracion_ms.count() << " ms | Memoria: " << memoria_kb << " Kb\n";
        
        }
    }

    //generamos las gráficas

    cout << "\nTodos los arreglos procesados. Llamando a Python para generar gráficas...\n";
    //int resultado = std::system("python3 scripts/plot_generator.py");
    int resultado = std::system("cd scripts && python3 plot_generator.py");

    if (resultado == 0){
        cout << "Gráficas generadas exitosamente.\n";
    } else {
        cerr << "Error al generar gráficas.\n";
    }
    
    return 0;
}