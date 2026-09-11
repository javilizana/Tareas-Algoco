import pandas as pd
import matplotlib.pyplot as plt
import os

def extraer_tipo(ruta):
    if 'dispersa' in ruta:
        return 'dispersa'
    elif 'diagonal' in ruta:
        return 'diagonal'
    elif 'densa' in ruta:
        return 'densa'
    return 'desconocido'

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
    columnas_req = ['archivo1', 'algoritmo', 'n', 'tiempo_ms', 'memoria_kb']
    if not all(col in df.columns for col in columnas_req):
        print("Error: El archivo CSV no tiene el formato esperado")
        return
    
    #extraemos el tipo de matriz desde la ruta almacenada en el CSV
    df['tipo'] = df['archivo1'].apply(extraer_tipo)
    
    #calculamos promedios por algoritmo, tipo de matriz y dimension (n)
    #esto es util porque cada combinacion (n, tipo) tiene 3 muestras (a, b, c)
    df_agrupado = df.groupby(['algoritmo', 'tipo', 'n']).mean(numeric_only=True).reset_index()
    algoritmos = df_agrupado['algoritmo'].unique()
    tipos = df_agrupado['tipo'].unique()

    for tipo in tipos:
        if tipo == "desconocido":
            continue
        
        #filtramos por tipo
        df_tipo = df_agrupado[df_agrupado['tipo'] == tipo]
        
    
        #grafico 1: tiempo de evecucion vs N
        plt.figure(figsize=(10,6))
        for algo in algoritmos:
            datos = df_tipo[df_tipo['algoritmo'] == algo]
            if not datos.empty:
                plt.plot(datos['n'], datos['tiempo_ms'], marker='o', label=algo)
        
        plt.title(f'Tiempo de Ejecución vs Dimensión (N) - Matriz {tipo.capitalize()}') 
        plt.xlabel('Dimensión de la Matriz (N)')
        plt.ylabel('Tiempo (ms)')
        plt.xscale('log')
        plt.yscale('log')
        plt.legend()
        plt.grid(True, which="both", ls="--", alpha=0.5)
        plt.savefig(os.path.join(directorio_salida, f'tiempo_ejecucion_{tipo}.png'))
        plt.close()
        
        #grafico 2: uso de memoria vs N
        plt.figure(figsize=(10,6))
        for algo in algoritmos:
            datos = df_tipo[df_tipo['algoritmo'] == algo]
            if not datos.empty:
                plt.plot(datos['n'], datos['memoria_kb'], marker='s', label=algo)
    
        plt.title(f'Uso de Memoria vs Dimensión (N) - Matriz {tipo.capitalize()}')
        plt.xlabel('Dimensión de la Matriz (N)')
        plt.ylabel('Memoria (KB)')
        plt.xscale('log')
        plt.legend()
        plt.grid(True, which="both", ls="--", alpha=0.5)
        plt.savefig(os.path.join(directorio_salida, f'uso_memoria_{tipo}.png'))
        plt.close()
    
    #graficos generales (promedios de todos los tipos)
    df_general = df.groupby(['algoritmo', 'n']).mean(numeric_only=True).reset_index()
    
    #grafico 3: tiempo de ejecucion vs N (general)
    plt.figure(figsize=(10,6))
    for algo in algoritmos:
        datos = df_general[df_general['algoritmo'] == algo]
        if not datos.empty:
            plt.plot(datos['n'], datos['tiempo_ms'], marker='o', label=algo)
    plt.title('Tiempo de Ejecución vs Dimensión (N) - General') 
    plt.xlabel('Dimensión de la Matriz (N)')
    plt.ylabel('Tiempo (ms)')
    plt.xscale('log')
    plt.yscale('log')
    plt.legend()
    plt.grid(True, which="both", ls="--", alpha=0.5)
    plt.savefig(os.path.join(directorio_salida, 'tiempo_ejecucion_general.png'))
    plt.close()
    
    #grafico 4: uso de memoria  vs N (general)
    plt.figure(figsize=(10,6))
    for algo in algoritmos:
        datos = df_general[df_general['algoritmo'] == algo]
        if not datos.empty:
            plt.plot(datos['n'], datos['memoria_kb'], marker='s', label=algo)
    
    plt.title('Uso de Memoria vs Dimensión (N) - General')
    plt.xlabel('Dimensión de la Matriz (N)')
    plt.ylabel('Memoria (KB)')
    plt.xscale('log')
    plt.legend()
    plt.grid(True, which="both", ls="--", alpha=0.5)
    plt.savefig(os.path.join(directorio_salida, 'uso_memoria_general.png'))
    plt.close()
        
    
    print(f"Gráficos detallados por tipo de matriz generados exitosamente en: {directorio_salida}")

if __name__ == "__main__":
    #las rutas asumen que el script se ejecuta desde code/matrix_multiplication/scripts/
    archivo_csv = "../data/measurements/mediciones.txt"
    carpeta_salida = "../data/plots/"
    
    generarGraficos(archivo_csv, carpeta_salida)