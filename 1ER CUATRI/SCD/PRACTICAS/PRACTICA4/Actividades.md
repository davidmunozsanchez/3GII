# Actividades P4

## Actividad 1

En esta actividad tenemos que añadir una nueva funcionalidad al código **ejecutivo1.cpp**.

El programa debe mostrar (cada vez que acaba un ciclo secundario) el retraso del instante final actual respecto al instante final esperado. La comprobación se tiene que hacer al final del bucle, inmediatamente después de **sleep_until**.

Para hacer todo esto, añadimos dentro del bucle (justo después de **sleep_until**), un **time_point** llamado **end_sec** (siempre **steady_clock**), y usamos la función **now()**. Así obtenemos el instante final actual.

Para calcular el retraso declaramos una variable **duration** y calculamos **end_sec - ini_sec** (que es el instante final esperado, el parámetro de **sleep_until**).  Esta variable la mostramos por pantalla de la forma que se indica en el código.

.![](/home/davidms_83/Imágenes/captura1.png)



## Actividad 2

En este ejercicio debemos diseñar una planificación para las tareas y restricciones que se nos muestra en la última diapositiva de la Práctica 4.

A partir de la tabla podemos obtener que el **ciclo principal** dura **2000ms** ya que es **mcm(500,500,1000,2000)**. 

En esta captura se da una posible planificación del sistema con una **duración del ciclo secundario de 500 ms** (realizamos 4 iteraciones, ahí tendríamos los 2000ms anteriores). 

![captura 2](/home/davidms_83/Imágenes/captura 2.png)

En cuanto al código, solo hay que cambiar la duración del ciclo secundario y  el número de tareas que hay así como el tiempo que duermen. También cambiamos la planificación según nuestro esquema.

![captura 4](/home/davidms_83/Imágenes/captura 3.png)

![captura 3](/home/davidms_83/Imágenes/captura 4.png)

En cuanto a las cuestiones a responder:

1. El mínimo tiempo de espera que queda al final de las iteraciones del ciclo secundario sería el del tercer ciclo, puesto que son **500-490=10 ms**, frente a los demás, que son todos mayores.
2. Si la tarea D tuviese un tiempo de cómputo de 250 ms seguiría siendo planificable con nuestra solución. Lo único que cambia es que la iteración 3 en vez de durar 490 ms dura **500 ms**.