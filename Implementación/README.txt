Instrucciones de compilación:
1. Abrir terminal en la carpeta donde se encuentren los archivos
2. Ejecutar $ make en la terminal
3. Ejecutar $ make clean en la terminal para eliminar los .o
4. Ejecutar $ ./main
5. Ingresar la instancia que se desea ejecutar (1, 2, 3, etc) por lo que el archivo de la instancia debe estar en la misma carpeta donde se ejecuta el programa.

Otras consideraciones:
- Debido al tiempo que implica encontrar todas las soluciones en una técnica completa, en la función cbj del archivo "alsp.c" en la primera condición asociada al cpu_time_used (línea 305) se puede modificar el tiempo que se desea correr el programa (en segundos) actualmente está configurado en 300 segundos (5 minutos). Al terminar el tiempo o al ocurrir un evento Control + C se creará el archivo con la mejor solución encontrada hasta el momento (o lo que se alcanzó a instanciar) y su costo
- Los archivos de solucion muestran la instanciación de cada avión en el orden que se menciona en la representación
- En caso de probar los experimentos, el orden de dominio y de instanciacion se puede modificar en las líneas 415 y 431 del archivo "alsp.c"
