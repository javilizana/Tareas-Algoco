import pandas as pd
import matplotlib.pyplot as plt
import os

def generarGraficos(ruta_csv, directorio_salida):
    #creamos el directorio de salida si no existe
    os.makedirs(directorio_salida, exist_ok = True)
    
    #leemos el archivo de mediciones
    try:
        df = pd.read_csv(ruta_csv)
    except FileNotFoundError:
        print(f"Error: No se encontró el archivo {ruta_csv}")
        return
    
    #verificamos que el csv tenga las columnas correctas
    columnas_req = ['algoritmo', 'n', 'tiempo_ms', 'memoria_kb']
    if not all(col in df.columns for col in columnas_req):
        print("Error: El archivo CSV no tiene el formato esperado")
        return
    
    #agrupamos por algoritmo y tamaño de matriz (n), calculando el promedio
    #esto es útil si se tienen múltiples corridas (densa, dispersa, etc.) para un mismo 'n'
    #df_agrupado = df.groupby(['algoritmo', 'n']).mean().reset_index()
    df_agrupado = df.groupby(['algoritmo', 'n']).mean(numeric_only=True).reset_index()
    algoritmos = df_agrupado['algoritmo'].unique()
    
    #grafico 1: tiempo de evecucion vs N
    plt.figure(figsize=(10,6))
    for algo in algoritmos:
        datos = df_agrupado[df_agrupado['algoritmo'] == algo]
        plt.plot(datos['n'], datos['tiempo_ms'], marker='o', label=algo)
        
    plt.title('Tiempo de Ejecución vs Dimensión de la Matriz (N)')
    plt.xlabel('Dimensión de la Matriz (N)')
    plt.ylabel('Tiempo (ms)')
    plt.legend()
    plt.grid(True)
    plt.savefig(os.path.join(directorio_salida, 'tiempo_ejecucion.png'))
    plt.close()
    
    #grafico 2: uso de memoria vs N
    plt.figure(figsize=(10,6))
    for algo in algoritmos:
        datos = df_agrupado[df_agrupado['algoritmo'] == algo]
        plt.plot(datos['n'], datos['memoria_kb'], marker='s', label=algo)
    
    plt.title('Uso de Memoria vs Dimensión de la Matriz (N)')
    plt.xlabel('Dimensión de la Matriz (N)')
    plt.ylabel('Memoria (KB)')
    plt.legend()
    plt.grid(True)
    plt.savefig(os.path.join(directorio_salida, 'uso_memoria.png'))
    plt.close()
    
    print(f"Gráficos generados exitosamente en: {directorio_salida}")

if __name__ == "__main__":
    #las rutas asumen que el script se ejecuta desde code/matrix_multiplication/scripts/
    archivo_csv = "../data/measurements/mediciones.txt"
    carpeta_salida = "../data/plots/"
    
    generarGraficos(archivo_csv, carpeta_salida)