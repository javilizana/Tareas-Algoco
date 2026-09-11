#include <iostream>
#include <vector>
#include <string>
#include <chrono>
#include <sys/resource.h>
#include <cstdio> 
#include <cstdlib>
#include <filesystem>

#include <sys/wait.h>
#include <unistd.h>

#include <fstream>
#include <iomanip>
#include <map>


using namespace std;
namespace fs = std::filesystem;

void sortArray(vector<int>&arr);


int main(int argc, char* argv[]){

    if (argc < 3){
        cerr << "Uso: " << argv[0] << " <carpeta_inputs> <nombre_algoritmo> [archivo_mediciones]\n";
        return 1;
    }

    string carpeta_inputs = argv[1];
    string nombre_algoritmo = argv[2];
    string ruta_mediciones = (argc >= 4) ? argv[3] : "data/measurements/mediciones.txt";

    cout << "Iniciando ordenamiento con " << nombre_algoritmo << "\n";

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
            string ruta_output = "data/array_output/" + nombre_algoritmo + "_" + entry.path().filename().string();

            //pipe para que el hijo le devuelva el tiempo medido al padre
            int pipefd[2];
            if(pipe(pipefd) == -1){
                perror("pipe");
                continue;
            }

            pid_t pid = fork();
            if (pid < 0){
                perror("fork");
                close(pipefd[0]);
                close(pipefd[1]);
                continue;
            }

            if(pid == 0){
                //proceso hijo
                close(pipefd[0]); //no necesita leer

                //medimos el tiempo de ejecucion durante el ordenamiento
                auto inicio = chrono::high_resolution_clock::now();
                sortArray(arr); 
                auto fin = chrono::high_resolution_clock::now();
                chrono::duration<double, milli> duracion_ms = fin - inicio;

                //el hijo escribe su propio archivo de salida
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

                //enviamos el tiempo medido de vuelta al padre a traves del pipe
                double t = duracion_ms.count();
                ssize_t escritos = write(pipefd[1], &t, sizeof(t));
                (void)escritos;
                close(pipefd[1]);

                _exit(0); //termina el hijo
            }

            //proceso padre
            close(pipefd[1]); //no necesita escribir
            double tiempo_ms = 0.0;
            ssize_t leidos = read(pipefd[0], &tiempo_ms, sizeof(tiempo_ms));
            close(pipefd[0]);

            int status;
            struct rusage uso_hijo;
            wait4(pid, &status, 0, &uso_hijo); //rusage de este hijo especifico

            if(leidos != (ssize_t)sizeof(tiempo_ms) || !WIFEXITED(status) || WEXITSTATUS(status) != 0){
                cerr << "Error: la corrida de '" << nombre_algoritmo << "' sobre '" << entry.path().filename().string() << "' fallo.\n";
                continue;
            }

            long memoria_kb = uso_hijo.ru_maxrss;

            //guardamos las metricas
            FILE* archivo_csv = fopen(ruta_mediciones.c_str(), "a");
            if(archivo_csv){
                fprintf(archivo_csv, "%s,%s,%zu,%f,%ld\n", ruta_input.c_str(), nombre_algoritmo.c_str(), n, tiempo_ms, memoria_kb);
                fclose(archivo_csv);
            }

            //descomentar esta línea si se quiere ver por pantalla en que arreglo/algoritmo va:
            //cout << "[" << nombre_algoritmo << "] " << entry.path().filename().string() << " -> N=" << n << " | Tiempo: " << tiempo_ms << " ms | Memoria: " << memoria_kb << " Kb\n";
        
        }
    }

    return 0;
}