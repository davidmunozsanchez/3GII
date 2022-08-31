# Configuración de red

Ambas distribuciones traen su interfaz de red que viene conectada por defecto en modo nat, conseguimos conectividad en internet. Vamos a añadir una interfaz de red a cada una y se van a conectar con un router virtual con el router y el anfitrión.

Vamos a usar ip.



Lo primero es habilitar otro adaptador de red en la configuración de virtualbox. Lo ponemos como solo anfitrión. No sale ningún nombre. En el menú de virtual box le damos a la opción ...solo anfitrión. Y ahí añadimos routers como sigue: simplemente le damos a crear. Hacemos esto para ambos SOs.

Una vez dentro de ambos SOs, usamos el comando ip addr. Tenemos que configurarlo todo.

Ponemos ip addr add 192.168.56.110/24 dev enp0s8.

Comprobamos con ip addr.

Hacemos ping 192.168.56.1 (al anfitrión).

A ubuntu le añadimos la 192.168.56.105/24 enp0s8.



Para activar un adaptador con ip: sudo ip link set enp0s8 up

