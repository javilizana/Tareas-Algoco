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
    
    for tipo in tipos:
        if tipo == 'desconocido':
            continue
        
        df_tipo = df_agrupado[df_agrupado['tipo'] == tipo]
        
        #grafico1: tiempo de ejecicion vs N
        plt.figure(figsize=(10, 6))
        for algo in algoritmos:
            datos = df_tipo[df_tipo['algoritmo'] == algo]
            if not datos.empty:
                plt.plot(datos['n'], datos['tiempo_ms'], marker='o', label=algo)
                
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
                plt.plot(datos['n'], datos['memoria_kb'], marker='s', label=algo)

        plt.title(f'Uso de Memoria vs Tamaño (N) - Arreglo {tipo.capitalize()}')
        plt.xlabel('Tamaño del Arreglo (N)')
        plt.ylabel('Memoria (KB)')
        plt.xscale('log')
        plt.legend()
        plt.grid(True, which="both", ls="--", alpha=0.5)
        plt.savefig(os.path.join(directorio_salida, f'uso_memoria_{tipo}.png'))
        plt.close()
    
    #print(f"Gráficos generados exitosamente en: {directorio_salida}")
    print(f"Gráficos detallados por tipo generados exitosamente en: {directorio_salida}")
    
if __name__ == "__main__":
    archivo_csv = "../data/measurements/mediciones.txt" 
    carpeta_salida = "../data/plots/"
    generarGraficos(archivo_csv, carpeta_salida)