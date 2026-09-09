#!/bin/bash

# Posicionarse en el directorio donde está el script
cd "$(dirname "$0")"

# Asegurar que existan los directorios de salida
mkdir -p data/measurements
mkdir -p data/matrix_output

# Limpiar mediciones previas si se desea reiniciar el registro
rm -f data/measurements/mediciones.txt

NS=(16 64 256 1024)
TS=("dispersa" "diagonal" "densa")
DS=("D0" "D10")
MS=("a" "b" "c")
ALGORITMOS=("naive" "strassen")

for n in "${NS[@]}"; do
    for t in "${TS[@]}"; do
        for d in "${DS[@]}"; do
            for m in "${MS[@]}"; do
                input1="data/matrix_input/${n}_${t}_${d}_${m}_1.txt"
                input2="data/matrix_input/${n}_${t}_${d}_${m}_2.txt"

                # Verificar que los archivos de input existan antes de ejecutar
                if [ -f "$input1" ] && [ -f "$input2" ]; then
                    for algo in "${ALGORITMOS[@]}"; do
                        # Definir un output único por cada algoritmo y entrada
                        output="data/matrix_output/${n}_${t}_${d}_${m}_${algo}_out.txt"

                        echo "Ejecutando -> Algoritmo: $algo | N: $n | Tipo: $t | Dominio: $d | Muestra: $m"
                        ./matrix_multiplication "$input1" "$input2" "$algo" "data/measurements/mediciones.txt" "$output"
                    done
                else
                    echo "Advertencia: No se encontró el par de inputs para N=$n, T=$t, D=$d, M=$m."
                fi
            done
        done
    done
done

echo "¡Proceso masivo de matrices finalizado con los 144 outputs!"
echo "Revisa tus mediciones en: data/measurements/mediciones.txt"