## Pasos para sorting:

- Generar inputs:
    en la carpeta INF221/T1/code/sorting/scripts 
        python3 array_generator.py

- Para ejecutar sorting (genera outputs, mediciones y plots):
    en la carpeta /INF221/T1/code/sorting
       (sin tiempo) make run_all

       (con tiempo) time make run_all



- Hacer los make:
    en la carpeta INF221/T1/code/sorting
        make

## Pasoso para matrix

- Generar inputs:
    en la carpeta INF221/T1/code/matrix_multiplication/scripts
        python3 matrix_generator.py

- Hacer make
    en la carpeta INF221/T1/code/matrix_multiplication/
        make

- run_matrix.sh:
    en la carpeta INF221/T1/code/matrix_multiplication/
        chmod +x run_matrix.sh

- para recorrer todos los inputs y guardar mediciones:
    en la carpeta INF221/T1/code/matrix_multiplication/
        (sin tiempo) ./run_matrix.sh.
        (con tiempo) time ./run_matrix.sh

- para revisar que se generaron 288 líneas de datos en mediciones
    wc -l data/measurements/mediciones.txt

- para generar graficos:
    en la carpeta INF221/T1/code/matrix_multiplication/scripts/
        python3 plot_generator.py 

# 1. Compilar (usa tu makefile de siempre, no cambia)
make

# 2. Generar las matrices de entrada, si aún no las tienes
cd scripts && python3 matrix_generator.py && cd ..

# 3. Ejecutar — ya no necesitas run_matrix.sh
./matrix_multiplication