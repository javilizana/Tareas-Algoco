#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <chrono>
#include <sstream>
#include <sys/resource.h>

#include <sys/wait.h>
#include <unistd.h>
#include <filesystem>
#include <cstdlib>
#include <set>

using namespace std;
namespace fs = std::filesystem;

//declaracion de las funciones implementadas en naive.cpp y strassen.cpp
vector<vector<int>> NaiveMatrixMult(const vector<vector<int>>& A, const vector<vector<int>>& B);
vector<vector<int>> Strassen(const vector<vector<int>>& M1, const vector<vector<int>>& M2);

/*
//funcion para obtener el consumo max de memoria residente en kilobyes
long obtenerMaxMemoria(){
    struct rusage uso;
    if(getrusage(RUSAGE_SELF, &uso) == 0){
        return uso.ru_maxrss;
    }
    return 0;
}*/

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

//escribe una matriz resultado en un archivo de texto
void guardarMatriz(const vector<vector<int>>& resultado, const string& ruta_output){
    ofstream archivo_out(ruta_output);
    if (!archivo_out.is_open()){
        cerr << "Error al crear el archivo de salida: " << ruta_output << "\n";
        return;
    }
    for (size_t i = 0; i < resultado.size(); i++){
        for (size_t j = 0; j < resultado.size(); j++){
            archivo_out << resultado[i][j] << (j + 1 == resultado[i].size() ? "" : " ");
        }
        archivo_out << "\n";
    }
    archivo_out.close();
}

void ejecutarAlgoritmo(const string& nombre_algoritmo, const vector<vector<int>>& M1, const vector<vector<int>>& M2, const string& ruta_input1, const string& ruta_input2,
                        const string& carpeta_output, const string& base, const string& ruta_mediciones){
    
    size_t n = M1.size();
    string ruta_output = carpeta_output + "/" + base + "_" + nombre_algoritmo + "_out.txt";

    //pipe para que el hijo le devuelva el tiempo medido al padre
    int pipefd[2];
    if(pipe(pipefd) == -1){
        perror("pipe");
        return;
    }

    pid_t pid = fork();
    if (pid < 0){
        perror("fork");
        close(pipefd[0]);
        close(pipefd[1]);
        return;
    }

    if (pid == 0){
        //proceso hijo
        close(pipefd[0]); //no necesita leer
        vector<vector<int>> resultado;

        //medimos el tiempo de ejecucion durante la multiplicacion (dentro del hijo)
        auto inicio = chrono::high_resolution_clock::now();
        
        if (nombre_algoritmo == "naive"){
            resultado = NaiveMatrixMult(M1, M2);
        } else if (nombre_algoritmo == "strassen"){
            resultado = Strassen(M1, M2);
        } else {
            cerr << "Error: Algoritmo '" << nombre_algoritmo << "' no reconocido.\n";
            close(pipefd[1]);
            _exit(1);
        }

        auto fin = chrono::high_resolution_clock::now();
        chrono::duration<double, milli> duracion_ms = fin - inicio;

        //el hijo escribe su propio archivo de salida
        guardarMatriz(resultado, ruta_output);

        //enviamos el tiempo medido de vuelta al padre a traves del pipe
        double t = duracion_ms.count();
        ssize_t escritos = write(pipefd[1], &t, sizeof(t));
        (void)escritos; //si falla el pipe, el padre lo detecta al leer menos bytes de los esperados
        close(pipefd[1]);

        _exit(0); //termina el hijo; su rusage quedara disponible para wait4() en el padre
    }

    //proceso padre
    close(pipefd[1]); //no necesita escribir
    double tiempo_ms = 0.0;
    ssize_t leidos = read(pipefd[0], &tiempo_ms, sizeof(tiempo_ms));
    close(pipefd[0]);

    int status;
    struct rusage uso_hijo;
    wait4(pid, &status, 0, &uso_hijo); //rusage de ESTE hijo especifico, no acumulado

    if(leidos != (ssize_t)sizeof(tiempo_ms) || !WIFEXITED(status) || WEXITSTATUS(status) != 0){
        cerr << "Error: la corrida de '" << nombre_algoritmo << "' sobre '" << base << "' fallo.\n";
        return;
    }

    long memoria_kb = uso_hijo.ru_maxrss;

    //guardamos metricas en formato CSV compatible con plot_generator.py
    ofstream archivo_csv(ruta_mediciones, ios::app);
    if (archivo_csv.is_open()){
        archivo_csv << ruta_input1 << "," << ruta_input2 << "," << nombre_algoritmo << "," << n << "," << tiempo_ms << "," << memoria_kb << "\n";
        archivo_csv.close();
    }
    cout << "[" << nombre_algoritmo << "] " << base << " -> N=" << n << " | Tiempo: " << tiempo_ms << " ms | Memoria: " << memoria_kb << " Kb\n";
                            
}

int main(int argc, char* argv[]){
    //optimizacion de flujos de E/S estandar para instancias masivas
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    //forzamos que cada linea impresa se muestre de inmediato en pantalla
    cout.setf(ios::unitbuf);

    //parametros con valores por defecto: se pueden sobreescribir por linea de comandos
    string carpeta_inputs   = (argc >= 2) ? argv[1] : "data/matrix_input";
    string carpeta_output   = (argc >= 3) ? argv[2] : "data/matrix_output";
    string ruta_mediciones  = (argc >= 4) ? argv[3] : "data/measurements/mediciones.txt";

    //algoritmos a ejecutar sobre cada par de matrices encontrado
    vector<string> algoritmos = {"naive", "strassen"};

    //aseguramos que existan los directorios de salida
    fs::create_directories(carpeta_output);
    fs::create_directories(fs::path(ruta_mediciones).parent_path());

    //reiniciamos el archivo de mediciones y escribimos el encabezado una sola vez
    ofstream archivo_csv(ruta_mediciones, ios::trunc);
    if (archivo_csv.is_open()){
        archivo_csv << "archivo1,archivo2,algoritmo,n,tiempo_ms,memoria_kb\n";
        archivo_csv.close();
    }

    cout << "Iniciando multiplicacion de matrices sobre: " << carpeta_inputs << "...\n";

    //recolectamos los nombres base de cada par (todo archivo que termine en _1.txt)
    set<string> bases;
    for (const auto& entry : fs::directory_iterator(carpeta_inputs)){
        if (!entry.is_regular_file() || entry.path().extension() != ".txt"){
            continue;
        }
        string nombre = entry.path().filename().string();
        const string sufijo1 = "_1.txt";
        if (nombre.size() > sufijo1.size() && nombre.compare(nombre.size() - sufijo1.size(), sufijo1.size(), sufijo1) == 0){
            string base = nombre.substr(0, nombre.size() - sufijo1.size());
            bases.insert(base);
        }
    }

    //procesamos cada par de matrices encontrado
    for (const auto& base : bases){
        string ruta_input1 = carpeta_inputs + "/" + base + "_1.txt";
        string ruta_input2 = carpeta_inputs + "/" + base + "_2.txt";
 
        if (!fs::exists(ruta_input1) || !fs::exists(ruta_input2)){
            cerr << "Advertencia: falta el par completo para '" << base << "', se omite.\n";
            continue;
        }
 
        vector<vector<int>> M1 = leerMatriz(ruta_input1);
        vector<vector<int>> M2 = leerMatriz(ruta_input2);
 
        if (M1.empty() || M2.empty()){
            cerr << "Advertencia: una o ambas matrices de '" << base << "' estan vacias, se omite.\n";
            continue;
        }
 
        for (const auto& algo : algoritmos){
            ejecutarAlgoritmo(algo, M1, M2, ruta_input1, ruta_input2, carpeta_output, base, ruta_mediciones);
        }
    }

    cout << "\nTodas las matrices procesadas. Llamando a Python para generar graficas...\n";
    int resultado = std::system("cd scripts && python3 plot_generator.py");

    if (resultado == 0){
        cout << "Graficas generadas exitosamente.\n";
    } else {
        cerr << "Error al generar graficas.\n";
    }

    return 0;
}   