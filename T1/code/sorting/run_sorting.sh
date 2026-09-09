#!/bin/bash

# Posicionarse en el directorio donde está el script
cd "$(dirname "$0")"

# Asegurar que existan los directorios de salida
mkdir -p data/measurements
mkdir -p data/array_output

# Limpiar mediciones previas si se desea reiniciar el registro
rm -f data/measurements/mediciones.txt

# Parámetros exactos generados por array_generator.py
NS=(10 1000 100000 10000000)
TS=("ascendente" "descendente" "aleatorio")
DS=("D1" "D7")
MS=("a" "b" "c")

# Algoritmos compilados por el makefile
ALGORITMOS=("sort" "mergesort" "quicksort" "patiencesort")

for n in "${NS[@]}"; do
    for t in "${TS[@]}"; do
        for d in "${DS[@]}"; do
            for m in "${MS[@]}"; do
                # El generador de arreglos crea un único archivo de input por combinación
                input="data/array_input/${n}_${t}_${d}_${m}.txt"
                
                # Verificar que el archivo input exista antes de ejecutar
                if [ -f "$input" ]; then
                    for algo in "${ALGORITMOS[@]}"; do
                        # Definir un output único por cada algoritmo y entrada
                        output="data/array_output/${n}_${t}_${d}_${m}_${algo}_out.txt"
                        echo "Ejecutando -> Algoritmo: $algo | N: $n | Tipo: $t | Dominio: $d | Muestra: $m"
                        
                        # Llamar al ejecutable correspondiente (ej: ./sorting_mergesort)
                        ./sorting_${algo} "$input" "$algo" "data/measurements/mediciones.txt" "$output"
                    done
                else
                    echo "Advertencia: No se encontró el input para N=$n, T=$t, D=$d, M=$m."
                fi
            done
        done
    done
done

echo "¡Proceso masivo de ordenamiento finalizado!"
echo "Revisa tus mediciones en: data/measurements/mediciones.txt"