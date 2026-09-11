import pandas as pd
import matplotlib.pyplot as plt
import os

def extraer_tipo(ruta):
    if 'ascendente' in ruta:
        return 'ascendente'
    elif 'descendente' in ruta:
        return 'descendente'
    elif 'aleatorio' in ruta:
        return 'aleatorio'
    return 'desconocido'

def generarGraficos(ruta_csv, directorio_salida):
    os.makedirs(directorio_salida, exist_ok=True)
    
    try:
        df = pd.read_csv(ruta_csv, header=None, names=['archivo', 'algoritmo', 'n', 'tiempo_ms', 'memoria_kb'])
    except FileNotFoundError:
        print(f"Error: No se encontró el archivo {ruta_csv}")
        return
    
    #extraer el tipo de arreglo desde la ruta almacenada en el CSV
    df['tipo'] = df['archivo'].apply(extraer_tipo)
    
    #calcular promedios por algoritmo, tipo de arreglo y tamaño de entrada
    df_agrupado = df.groupby(['algoritmo', 'tipo', 'n']).mean(numeric_only=True).reset_index()
    algoritmos = df_agrupado['algoritmo'].unique()
    tipos = df_agrupado['tipo'].unique()
    
    """
    #definimos estilos y marcadores visuales para cada tipo de arreglo
    estilos_tipo = {
        'ascendente': {'linestyle': '-', 'marker': 'o'},
        'descendente': {'linestyle': '--', 'marker': 's'},
        'aleatorio': {'linestyle': '-.', 'marker': '^'}
    }"""
    
    #Definimos estilos visuales específicos para cada algoritmo
    estilos_algo = {
        'MergeSort': {'ls': '-', 'marker': 'o', 'alpha': 0.8, 'lw': 2},
        'QuickSort': {'ls': '--', 'marker': 's', 'alpha': 0.8, 'lw': 2},
        'std::sort': {'ls': ':', 'marker': '^', 'alpha': 0.8, 'lw': 2.5},
        'PatienceSort': {'ls': '-.', 'marker': 'D', 'alpha': 0.8, 'lw': 2}
    }
    
    estilo_por_defecto = {'ls': '-', 'marker': 'x', 'alpha': 0.8, 'lw': 2}
    
    for tipo in tipos:
        if tipo == 'desconocido':
            continue
        
        df_tipo = df_agrupado[df_agrupado['tipo'] == tipo]
        
        #grafico1: tiempo de ejecicion vs N
        plt.figure(figsize=(10, 6))
        for algo in algoritmos:
            datos = df_tipo[df_tipo['algoritmo'] == algo]
            if not datos.empty:
                #obtenemos el estilo correspondiente al algoritmo
                estilo = estilos_algo.get(algo, estilo_por_defecto)
                plt.plot(datos['n'], datos['tiempo_ms'], label=algo, **estilo)
                #plt.plot(datos['n'], datos['tiempo_ms'], marker='o', label=algo)
                
        plt.title(f'Tiempo de Ejecución vs Tamaño (N) - Arreglo {tipo.capitalize()}')
        plt.xlabel('Tamaño del Arreglo (N)')
        plt.ylabel('Tiempo (ms)')
        plt.xscale('log')
        plt.yscale('log')
        plt.legend()
        plt.grid(True, which="both", ls="--", alpha=0.5)
        plt.savefig(os.path.join(directorio_salida, f'tiempo_ejecucion_{tipo}.png'))
        plt.close()
    
    
        #grafico2: uso de memoria vs N
        plt.figure(figsize=(10, 6))
        for algo in algoritmos:
            datos = df_tipo[df_tipo['algoritmo'] == algo]
            if not datos.empty:
                #obtenemos el estilo correspondiente al algoritmo
                estilo = estilos_algo.get(algo, estilo_por_defecto)
                plt.plot(datos['n'], datos['memoria_kb'], label=algo, **estilo)
                #plt.plot(datos['n'], datos['memoria_kb'], marker='s', label=algo)

        plt.title(f'Uso de Memoria vs Tamaño (N) - Arreglo {tipo.capitalize()}')
        plt.xlabel('Tamaño del Arreglo (N)')
        plt.ylabel('Memoria (KB)')
        plt.xscale('log')
        plt.legend()
        plt.grid(True, which="both", ls="--", alpha=0.5)
        plt.savefig(os.path.join(directorio_salida, f'uso_memoria_{tipo}.png'))
        plt.close()
        
    #graficos generales (promedios de todos los tipos)
    df_general = df.groupby(['algoritmo', 'n']).mean(numeric_only=True).reset_index()
    
    #grafico 3: tiempo de ejecucion vs N (general)
    plt.figure(figsize=(10, 6))
    for algo in algoritmos:
        datos = df_general[df_general['algoritmo'] == algo]
        if not datos.empty:
            estilo = estilos_algo.get(algo, estilo_por_defecto)
            plt.plot(datos['n'], datos['tiempo_ms'], label=algo, **estilo)
        
    plt.title('Tiempo de Ejecución vs Tamaño (N) - General')
    plt.xlabel('Tamaño del Arreglo (N)')
    plt.ylabel('Tiempo (ms)')
    plt.xscale('log')
    plt.yscale('log')
    plt.legend()
    plt.grid(True, which="both", ls="--", alpha=0.5)
    plt.savefig(os.path.join(directorio_salida, 'tiempo_ejecucion_general.png'))
    plt.close()
    
    #grafico 4: uso de memoria  vs N (general)
    plt.figure(figsize=(10, 6))
    for algo in algoritmos:
        datos = df_general[df_general['algoritmo'] == algo]
        if not datos.empty:
            estilo = estilos_algo.get(algo, estilo_por_defecto)
            plt.plot(datos['n'], datos['memoria_kb'], label=algo, **estilo)
            
    plt.title('Uso de Memoria vs Tamaño (N) - General')
    plt.xlabel('Tamaño del Arreglo (N)')
    plt.ylabel('Memoria (KB)')
    plt.xscale('log')
    plt.legend()
    plt.grid(True, which="both", ls="--", alpha=0.5)
    plt.savefig(os.path.join(directorio_salida, 'uso_memoria_general.png'))
    plt.close()
    
    #print(f"Gráficos generados exitosamente en: {directorio_salida}")
    print(f"Gráficos detallados por tipo generados exitosamente en: {directorio_salida}")

#funcion que lee el CSV, agrupa los datos y muestra una tabla resumen en la terminal
def imprimirTabla(ruta_csv):
    try:
        df = pd.read_csv(ruta_csv, header=None, names=['archivo', 'algoritmo', 'n', 'tiempo_ms', 'memoria_kb'])
    except FileNotFoundError:
        return
    
    #extraemos el tipo de archivo
    df['tipo'] = df['archivo'].apply(extraer_tipo)

    #agrupamos y calculamos promedios de tiempo y max de memoria
    resumen = df.groupby(['algoritmo', 'tipo', 'n']).agg(
        tiempo_ms=('tiempo_ms', 'mean'),
        memoria_kb=('memoria_kb', 'max')
    ).reset_index()
    
    #redondeamos para mejorar la lectura
    resumen['tiempo_ms'] = resumen['tiempo_ms'].round(2)
    
    #imprimimos
    print("\n" + "="*80)
    print(f"{'Algoritmo':<15} {'Tipo':<20} {'N':<15} {'Tiempo (ms)':<15} {'Memoria (KB)':<15}")
    print("-" * 80)
    for _, row in resumen.iterrows():
        print(f"{row['algoritmo']:<15} {row['tipo']:<20} {str(row['n']):<15} {str(row['tiempo_ms']):<15} {str(row['memoria_kb']):<15}")
    print("="*80 + "\n")

  
if __name__ == "__main__":
    archivo_csv = "../data/measurements/mediciones.txt" 
    carpeta_salida = "../data/plots/"
    
    #llamamos para imprimir la tabla
    imprimirTabla(archivo_csv)
    
    #llamamos para generar los graficos
    generarGraficos(archivo_csv, carpeta_salida)