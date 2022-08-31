# SEMINARIO 4

## 16/11/2021

### Ejercicio 1:

**Dadas dos entidades TCP (A y B) conectadas por una red cuya velocidad de transmisión es 100 Mbps, suponga segmentos de 1024 bytes y un RTT constante de 2 mseg. Si A transmite masivamente datos a B, ¿cuánto tiempo tardará en transmitir 8 tramas? Incluya el número de secuencia y de acuse en todos los segmentos TCP. Haga las suposiciones que estime necesarias.**

Suposiciones:

- [ ] Tiempo ida = Tiempo vuelta (RTT/2).
- [ ] Tiempo de generación/procesamiento de tramas/ACK ≈ 0
- [ ] Consideramos los tamaños de cabecera de TCP = 20 Bytes e IP = 20 Bytes.
- [ ] Las cabeceras de capas inferiores no se considerarán en los cálculos.
- [ ] Se desprecia el tamaño de las cabeceras y colas de las capas inferiores a IP.

Hay que considerar también:

- El tiempo de establecimiento de conexión a TCP.
- El control de congestión de TCP.
- Supondremos un tamaño de ventana de congestión inicial de 2, ya que será más eficiente (no habría que esperar los 500 ms para confirmar el primer segmento en caso de que el tamaño de ventana de congestión fuese 1.)
- Control de flujo: la ventana ofertada por el receptor será suficiente para recibir todos los segmentos en cada momento.



El tiempo de establecimiento es el tiempo que se tarda en hacer SYN y recibir SYN, ACK, que es 1 RTT y el tiempo que se tarda en enviar el ACK sería 1/2 RTT. Y también el de poner todo en los cables.

Tsyn = (20+20)B/10 mbps = 40x8bits/100x10^6 bits/seg

Tsyn,ack = el mismo que el de arriba

Tack = el mismo que el de arriba

Testab = (3x40x8)/10^8 + 1,5 RTT  = 3.0096 ms



Para calcular cuánto se tarda en transmitir 8 tramas, dividimos y calculamos tres tiempos:

- Ta = (1024+20)x8/10^8 + Tida + (20+20)x8/10^8 + Tvuelta = 2.087 ms.

- Tb = Ta = 2.087ms.

- Tc = (20+20)x8/10^8 + Tvuelta + 2,087 

- Ttotal = Testab + Ta + Tb + Tc

  

### Ejercicio 2:

**Explique las diferencias en objetivos y funcionamiento entre el control de flujo y el control de congestión en TCP. ¿Cómo ayudan los routers en el control de congestión de TCP? ¿Y en el control de flujo?**

El control de flujo permite al receptor controlar el envío de segmentos hacia él para evitar que se sature. (Visto en teoría con un ejemplo de un receptor con un buffer que se va llenando. El receptor es capaz de informar al emisor de cuantos bytes se le pueden enviar como máximo). El receptor es el encargado de implementar este mecanismo.  El receptor usa el campo **WIN** de la cabecera **TCP**.

![]()

En cuanto al control de congestión, en condiciones normales, la ventana de congestión se llama **CWIN**, que se implementa en +2 MSS (dos segmentos), cada **ACK**. Esto significa que cada vez que se confirman todos los elementos que se envían, se ha duplicado el tamaño de la ventana (se confirman cada dos los ACK ¿?). Cuando ocurra un evento de congestión, entonces se pasa a la fase de **CONGESTION AVOIDANCE**. EN esta ase CWIN crece en un +1 MSS con cada **ACK**. Este mecanismo lo lleva a cabo el emisor, que es quien controla la ventana de congestión (o envíos), él mismo controla lo que mete en la red. Lo gestiona con una variable para el tamaño de ventana de congestión.

![]()

El router no puede ayudar en el control de congestión de TCP ni en el control de flujo, puesto que no puede gestionar ni el emisor ni el receptor. Sin embargo, pueden ayudar a evitar la congestión en la red con otros mecanismos.

Estos dos mecanismos pueden funcionar al mismo tiempo. El receptor pone limitaciones al emisor, por muy grande que sea la ventana de congestión, estamos dependiendo de que se pueda escribir en el buffer o no.



### Ejercicio 3:

**Suponga el envío de un fichero grande sobre una conexión TCP y suponga que el RTT (tiempo de ida y vuelta) es constante.**

1. **Si CongWin es 1MSS (tamaño del segmento). ¿cuánta tiempo como mínimo se necesitará para que CongWin sea 7 MSS? (Suponga que no hay pérdidas y que no entra en la zona de prevención de congestión).**
2. **¿Cuál será el tamaño de CongWin tras 6  RTTs?**
3. **Si CongWin es 101 MSS y está en la zona de prevención de la congestión, ¿cuánto tiempo se necesitará para que CongWin sea 107 MSS?**
4. **¿Cuál será el throughput medio tras 6  RTTs?**

**Respuesta 1:**

Suponemos tiempo de transmisión 0 (el tiempo que se tarda en poner la trama en el medio).

Solución: se tarda 3 RTTs + 500 ms aprox.

**Respuesta 2:**

En cada RTT se cumple que crece al doble.

Solución: 2^6 = 64.

**Respuesta 3:**

Si estamos en prevención, la ventana solo crece en 1 MSS cada vez que se recibe una confirmación. Si partimos de un tamaño de ventana 101, nos harán falta 6 RTTs más para alcanzar 107.

**Respuesta 4:**

Tras 6 RTTs, el tamaño de la ventana será 64 MSS. Se pueden mandar 64MSS en cada RTT. El throughput sería dividir 64 MSS en 1 RTT.



### Ejercicio 4:

**Suponga dos entidades TCP A y B con la siguiente configuración: el valor inicial de la ventana de congestión es 3 MSS; el tamaño del buffer en recepción es 4 MSS; la aplicación receptora consume 1 MSS cada 30 ms. Suponiendo que el round trip time (RTT) es de 10 ms y que no hay pérdidas ni descartes de paquetes, ¿cuánto tiempo tarda la entidad A en enviar 8 segmentos TCP con datos a la entidad B? Considere despreciable el tiempo que se tarda en emitir los segmentos por los interfaces de red.**

