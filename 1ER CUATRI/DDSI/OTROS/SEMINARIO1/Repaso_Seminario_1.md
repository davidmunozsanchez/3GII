# Repaso Seminario 1

## David Muñoz Sánchez 3ºGII A

Para el menú principal y el submenú dentro de la opción 1, usamos un bucle do-while.

**Caso 1:**



**Empecemos por el caso 2:**

Esta funcionalidad nos permite borrar todas las tablas existentes y crearlas nuevas, para ellos ejecutamos DROP en cada una de las tablas.

Para crearlas, usamos CREATE TABLE, donde en cada tabla tenemos que indicar cuál será la clave primaria y cual será el contenido, y si hay que hacer alguna comprobación.

Por último insertamos los valores predefinidos en stock y hacemos un commit para que todos los cambios se guarden.

**Continuando ahora con el caso 3:**

Queremos mostrar el contenido de todas las tablas de las cuales disponemos. Para ello, en un pequeño menú, damos la opción al usuario que elija la tabla que quiere mostrar y simplemente se consigue con la sentencia SQL SELECT * FROM "nombre de tabla".

**Continuando ahora con el caso 4:**

Tenemos que salir del menú, el cierre de la conexión se ejecuta más abajo en el finally.

