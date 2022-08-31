# Modelos de computación

## Práctica 2

Para la realización de esta práctica, usaré Visual Studio Code con la extensión Lex. El archivo plantilla se llamará ***plantilla.l***.

El caso práctico es que vamos a recibir un archivo escrito de forma automática en el que aparecen diversos datos que tenemos que reconocer. Deberíamos ser capaces de captar correo, dni y teléfono de varias personas. El primer correo, tfno. y email que aparezcan serían para la persona 1 y así sucesivamente.

En cuanto a la plantilla, en la zona de declaraciones añado **list** y **cassert**. Vamos a usar listas para guardar los datos, ya que con funciones predefinidas nos permite insertar datos al final, consultar el primero y eliminar el primero (**push_back, front, pop_front**). También mostraremos la solución con una función que recibe como parámetro tres listas (una para los email, otra para los dni y otra para los teléfonos).

También declaro expresiones regulares con el fin de usar un nombre más simple en el apartado de reglas. Las reglas simplemente meten en su lista correspondiente el dato captado que devuelve la función YYText(). La variable yytext no me ha funcionado puesto que no es un const char* si no un char* y mis listas son de string. Hay que notar que en C++ podemos igualar string a const char* no a char*. Además muestro un mensaje por la salida estándar para confirmar que se ha leído un dato.

Atendiendo ahora al main, primero hacemos tratamiento de errores con el número de argumentos introducido y con el flujo de entrada. Después creamos el objeto **yyFlexLexer flujo** al que le hemos pasado como parámetro para su construcción (por referencia), el flujo de entrada y el flujo de salida.

La función mostrar solución comprueba con un assert que todas las listas tengan el mismo tamaño. Después, en un bucle, va mostrando el frente de cada lista y eliminando dicho elemento de ella.



## Compilación, ejecución y resultados

La compilación se hace como muestro en la siguiente captura:

![](/home/davidms_83/Imágenes/Captura de pantalla de 2021-12-24 16-24-20.png)

Aquí tenemos un ejemplo de ejecución con el fichero de entrada mostrado:

![Captura de pantalla de 2021-12-27 10-34-17](/home/davidms_83/Imágenes/Captura de pantalla de 2021-12-27 10-34-17.png)


