# Tema 2. Componentes Hardware de un Servidor

## ¿Qué es una placa base?

Una placa base (placa madre, motherboard, mainboard) es la tarjeta de circuito impreso (PCB) principal de un computador. En ella se conectan los principales componentes del computador y contiene diversos conectores para los distintos periféricos.

### Tarjeta de circuito impreso (PCB)

Están hechas de láminas de un sustrato no conductor (normalmente fibra de vidrio con una resina no inflamable) en las que se intercalan pistas de cobre (que sí es un material conductor).

Las placas bases actuales son multi-capa. A través de unos agujeros (vías) podemos conectar las pistas de una capa a otra.

Las placas base se suelen fabricar con distintos tamaños y formas (***form factors*** en ingles), según distintos estándares: ATX, BTX, EATX, Mini-ITX, etc.

![image-20220106191536582](/home/davidms_83/.config/Typora/typora-user-images/image-20220106191536582.png)

![image-20220106191602040](/home/davidms_83/.config/Typora/typora-user-images/image-20220106191602040.png)

### Montaje de los componentes de una placa base

A la hora de montar una placa base, hay que tener cuidado con las descargas electrostáticas (**ESD**, *Electrostatic discharge*). Pueden dañar algunos chips de la placa base: conviene descargar la electricidad estática previamente tocando alguna superficie amplia de metal, usar una muñequera de descarga (*ESD wrist strap*) o guantes *ESD-safe*.

Tampoco conviene tocar ningún contacto metálico de ningún componente de la placa ni de ningún conector, además de asegurarnos de que el equipo esté apagado antes de instalar o quitar cualquier componente (salvo *hot plugging/swapping*).

Normalmente un componente o un conector solo puede instalarse de una única manera: no hay que forzar la inserción de componentes o conectores.

## La fuente de alimentación

La misión de la fuente de alimentación es convertir la corriente alterna en corriente continua. Alimenta tanto a la placa base como a los periféricos y hay diferentes tipos según la potencia (250W, 500W, 700W).

- Entrada: AC (220V - 50Hz).
- Salidas: DC(+-5V, +-12V, +-3,3V).

## El módulo regulador de voltaje (VRM)

Su misión es adaptar la tensión continua de la fuente de alimentación a las tensiones continuas **menores** que necesitan los diferentes elementos de un computador (CPU, memoria, chipset, etc.) dándoles también **estabilidad**.

![image-20220106192324614](/home/davidms_83/.config/Typora/typora-user-images/image-20220106192324614.png)

## Disipadores de calor

Hay dos tipos: activos y pasivos.

![image-20220106192409170](/home/davidms_83/.config/Typora/typora-user-images/image-20220106192409170.png)

## Zócalos para el microprocesador (CPU sockets)

Facilitan la conexión entre el microprocesador y la placa base de tal forma que el microprocesador pueda ser reemplazado sin necesidad de soldaduras.

Los zócalos para microprocesadores con un número grande de pines suelen ser del tipo PGA-ZIF (*pin grid array - zero insertion force*) o LGA (*land grid array*), que hacen uso de una pequeña palanca (PGA-ZIF) o una pequeña placa de metal (LGA) para fijar el micro al zócalo. De esta forma, se minimiza el riesgo de que se doble alguna patilla durante el proceso de inserción.

![image-20220106192626888](/home/davidms_83/.config/Typora/typora-user-images/image-20220106192626888.png)

## Diferencias entre un micro para servidores y uno de PC

Los procesadores de servidores:

- Tienen un mayor número de núcleos.
- Suelen incorporar soporte para multiprocesamiento (2 o más micros en la misma placa).
- Tienen más memoria caché.
- Son compatibles con tecnologías de memoria RAM con ECC, que tienen mayor fiabilidad en general.
- Tienen más canales de memoria RAM y más líneas de E/S.
- Pasan más controles de calidad y están preparados para estar todo el día funcionando.
- Tienen más tecnologías dedicadas a facilitar tareas propias de servidores como la encriptación, la virtualización o la ejecución multi-hebra.
- Los principales fabricantes son: Intel, AMD e IBM.

![image-20220106193112987](/home/davidms_83/.config/Typora/typora-user-images/image-20220106193112987.png)

## AMD: EPYC y Opteron

Son los procesadores de AMD para servidores. El primer Opteron (2003) due el primer procesador con el conjunto de instrucciones AMD x86_64. En 2004, los Opteron fueron los primeros procesadores x86 con 2 núcleos.

Actualmente hay tres grandes familias de procesadores AMD para servidores: EPYC, Opteron (X-Series y A-Series).

### Epyc

Se trata de un módulo multi-chip con 8 chips (*chiplets*) por cada microprocesador EPYC sumado a 1 chip con tecnología más barata para E/S y controladores DRAM.

Cada *chiplet* tiene 8 cores Zen x86 (*amd64*) más las correspondientes cachés. Son verdaderos SoC (*System-on-a-Chip*), es decir, un chipset integrado en el microprocesador.

![image-20220106193740619](/home/davidms_83/.config/Typora/typora-user-images/image-20220106193740619.png)

### AMD Opteron X-Series

Están basados en APU (*Accelerated Processing Units*) que integran CPU (amd64) y GPU (*Graphics Processing Unit*) en un mismo chip.

Además, también integran el controlador de E/S, por lo que es otro SoC.

![image-20220106193752535](/home/davidms_83/.config/Typora/typora-user-images/image-20220106193752535.png)

### AMD Opteron A-Series (ARM)

Se encuentran en muchos servidores de streaming (Spotify, Youtube, Netflix, etc.), en los que el cuellos de botella no está en el procesador.

Estan basados en micro-procesadores de ARM (RISC) usados en dispositivos móviles con excelentes ratios prestaciones/consumo (tienen consumos inferiores a 30W).

Son SoC: tienen controladores PCI-e, Ethernet y SATA en el propio chip.

![image-20220106193933303](/home/davidms_83/.config/Typora/typora-user-images/image-20220106193933303.png)

## IBM Power

POWER (*Performance Optimization With Enhanced RISC*), es el resultado del trabajo continuo entre Apple, IBM y Motorola para servidores de gama alta de muy altas prestaciones por catio, disponibilidad y fiabilidad (***mainframes***).

![image-20220106194110960](/home/davidms_83/.config/Typora/typora-user-images/image-20220106194110960.png)

## Ranuras para la memoria DRAM

DRAM significa *Dynamica Random Access Memory*. Las ranuras son los conectores en los que se insertan los módulos de memoria principal: R/W, volátil, necesitan refresco, prestaciones inferiores a SRAM (caché) pero mayor densidad (bits/cm^2) y menor coste por bit.

![image-20220106194256606](/home/davidms_83/.config/Typora/typora-user-images/image-20220106194256606.png)

![image-20220106194329403](/home/davidms_83/.config/Typora/typora-user-images/image-20220106194329403.png)

![image-20220106194341778](/home/davidms_83/.config/Typora/typora-user-images/image-20220106194341778.png)

### Tipos de DIMM para una tecnología dada

Para servidores:

- EU-DIMM: U-DIMM con *Error Correcting Code*, ECC (las hace más fiables).
- R-DIMM: *Registered DIMM*. Hay un registro que almacena las señales de control (operación a realizar, líneas de dirección...). Mayor latencia pero permite módulos de mayor tamaño. Tienen ECC.
- LR-DIMM: *Load Reduced DIMM*. Hay un registro que almacena tanto las señales de control como los datos a leer/escribir. Son las que permiten los módulos con mayor tamaño. Tienen ECC.

Para PC y portátiles:

- DIMM o U-DIMM: *Unbuffered (Unregistered) DIMM*.

- SO-DIMM: *Small Outline DIMM*. Tamaño más reducido para equipos portátiles (tienen menos contactos).

  ![image-20220106194957084](/home/davidms_83/.config/Typora/typora-user-images/image-20220106194957084.png)

### Canales y bancos de memoria DRAM

Estas ranuras de las que estamos hablando están agrupadas en **canales** de memoria (*memory channels*) a los que la CPU puede acceder en paralelo, pudiendo conectarse varios módulos de memoria en cada canal (**bancos**).

**No** se puede acceder **simultáneamente** a dos módulos del **mismo** canal de memoria.

![image-20220106195012721](/home/davidms_83/.config/Typora/typora-user-images/image-20220106195012721.png)

### Rangos de memoria DRAM (*memory ranks*)

Cada módulo de memoria está, a su vez, distribuido en rangos de memoria que no son más que agrupaciones de chips que proporcionan la palabra completa de 64 bits (72 bits en caso de memorias ECC).

En el caso de un módulo de un solo rango (single rank) todos los chips de memoria del módulo se asocian para dar la palabra de 64 bits (72 si ECC).

En el caso de n rangos, es como si tuviéramos una agrupación de n memorias DRAM independientes en el mismo módulo, cada una con su conjunto diferente de chips.

Notación: 1Rx8: Módulo de 1 rango con chips de 8 bits (64/8=8 chips).

![image-20220106195940751](/home/davidms_83/.config/Typora/typora-user-images/image-20220106195940751.png)

![image-20220106200051218](/home/davidms_83/.config/Typora/typora-user-images/image-20220106200051218.png)

## Ranuras de expansión

Permiten la conexión a la placa base de otras tarjetas de circuito impreso.

![image-20220106200145890](/home/davidms_83/.config/Typora/typora-user-images/image-20220106200145890.png)

![image-20220106200219850](/home/davidms_83/.config/Typora/typora-user-images/image-20220106200219850.png)

### Interfaz PCI-Express (PCIe)

![image-20220106200749304](/home/davidms_83/.config/Typora/typora-user-images/image-20220106200749304.png)

## Almacenamiento permanente (no volátil)

![image-20220106200818292](/home/davidms_83/.config/Typora/typora-user-images/image-20220106200818292.png)

### Discos duros (HDD, *Hard Disk Drives*)

Se trata de almacenamiento permanente a lo largo de la superficie de unos discos recubiertos de material magnético.

La lectura y escritura se realiza a través de unos cabezales magnéticos controlados por un brazo motor y la ayuda del giro de los discos.

Los datos se distribuyen en pistas. Cada pista se subdivide en sectores de 512 bytes. Los sectores se agrupan en clusters lógicos.

Los tiempo de respuesta (latencias) son muy variables, ya que dependen de la pista y el sector concretos donde esté el cabezal y el sector concreto al que se quiere acceder.

Las velocidades de rotación más habituales son (en r.p.m.): 5400, 7200, 10000, 15000.

### Unidades de estado sólido (SSD, Solid State Drives)

Se trata de almacenamiento no volátil distribuido en varios circuitos integrados (chips) de memoria flash (transistores MOSFET de puerta flotante).

Los tipos de celdas habituales son: SLC (*single-level cell*), MLC (*multi-level cell*).

El acceso es aleatorio, es decir, mismo tiempo de respuesta independientemente de la celda de memoria a la que se quiere acceder (NVRAM, *Non-volatile RAM*).

Un controlador se encarga de distribuir la dirección lógica de las celdas de memoria para evitar su desgaste tras múltiples re-escrituras (*wear levelling*).

![image-20220106201428741](/home/davidms_83/.config/Typora/typora-user-images/image-20220106201428741.png)

![image-20220106201442895](/home/davidms_83/.config/Typora/typora-user-images/image-20220106201442895.png)

### RAID (Redundant Array of Independent Disks)

Combinan diversas unidades de almacenamiento (normalmente de idénticas características) para generar unidades de almacenamiento lógicas con mayor tolerancia a fallos, fiabilidad y/o ancho de banda.

![image-20220106202106685](/home/davidms_83/.config/Typora/typora-user-images/image-20220106202106685.png)

### Unidades ópticas

Almacenan la información de forma permanente a través de una serie de surcos en un disco que pueden ser leídos por un haz de luz láser.

Los discos compactos (CD), discos versátiles digitales (DVD) y discos Blu-ray (BD) son los tipos de medios ópticos más comunes que pueden ser leídos y grabados por estas unidades.

![image-20220106202243320](/home/davidms_83/.config/Typora/typora-user-images/image-20220106202243320.png)

### Unidades de cinta (tape drives)

Almacenan información de forma permanente a través de una cinta recubierta de material magnético que se enrolla por medio de carretes.

Las latencias suelen ser muy altas ya que hay que rebobinar la cinta hasta que el cabezal se encuentre en la posición deseada.

Es el medio con la mayor capacidad de bits para un área dada. Actualmente, permiten almacenamiento de decenas de TB por cinta y velocidades de lectura secuencial en torno a 150 Mbps.

Es el medio de almacenamiento masivo más barato y se usan normalmente como almacenamiento de respaldo (backup) y archivado.

### Interfaz P-ATA (ATA paralelo)

![image-20220107113612753](/home/davidms_83/.config/Typora/typora-user-images/image-20220107113612753.png)

### Interfaz Serial-ATA (SATA)

![image-20220107113642173](/home/davidms_83/.config/Typora/typora-user-images/image-20220107113642173.png)

### Interfaces SCSI y SAS

![image-20220107113741416](/home/davidms_83/.config/Typora/typora-user-images/image-20220107113741416.png)

### Conexión de múltiples unidades SAS

![image-20220107113811459](/home/davidms_83/.config/Typora/typora-user-images/image-20220107113811459.png)

![image-20220107113825220](/home/davidms_83/.config/Typora/typora-user-images/image-20220107113825220.png)

### NVMe: Non-Volatile Memory Express

![image-20220107113858423](/home/davidms_83/.config/Typora/typora-user-images/image-20220107113858423.png)

## Conectores del panel trasero

![image-20220107113925579](/home/davidms_83/.config/Typora/typora-user-images/image-20220107113925579.png)

### Conectores del panel trasero de un servidor

![image-20220107113950017](/home/davidms_83/.config/Typora/typora-user-images/image-20220107113950017.png)

### Adaptador de red RJ-45 para Ethernet

El conector RJ-45 (8P8C) se usa para redes de área local. Permite conexiones a largas distancias (100m con par trenzado y varios km con fibra óptica).

Existen varios estándares, todos ellos son compatibles unos con otros y pueden ser full-duples:

- Ethernet clásico: 10 Mbit/s.
- Fast-Ethernet: 100 Mbit/s.
- Gigabit-Ethernet: 1Gbit/s.
- 10G-Ethernet: 10 Gbit/s.

Muchos estándares de comunicación se pueden realizar a través de Ethernet:

- iSCSI (Internet SCSI): estándar que permite el uso del protocolo SCSI sobre redes TCP/IP.

- FCoE (*Fibre Channel over Ethernet*): estándar que permite el uso de tramas *Fibre Channel* sobre TCP/IP.

  ### Universal Serial Bus (USB)

![image-20220107114538407](/home/davidms_83/.config/Typora/typora-user-images/image-20220107114538407.png)

### Thunderbolt (Intel)

Combina PCIe y DisplayPort (almacenamiento externo de altas prestaciones y monitor de alta resolución).

Se pueden alcanzar diversos anchos de banda en cada dirección según la generación:

- 10 Gbps (Gen1)
- 20 Gbps (Gen2)
- 40 Gbps (Gen3)

Mediante conexión en cadena (*daisy-chain*) se pueden conectar hasta 6 dispositivos. Permite longitudes de hasta 3 m con cable eléctricos o 60 m con cables ópticos y puede proporcionar hasta 10W de potencia a periféricos externos si se usan cables externos.

### Otros conectores internos

![image-20220107114826365](/home/davidms_83/.config/Typora/typora-user-images/image-20220107114826365.png)

## ROM/Flash BIOS (Basic I/O System)

Almacena el código de arranque (*boot*) del computador. Este código se encarga de identificar los dispositivos instalados, instalar drivers básicos para acceder a los mismos, realizar el Power-on self-test (POST) del sistema e iniciar el SO.

Los parámetros de configuración de cada placa se almacenan en una pequeña memoria RAM alimentada por una pila (que también se usa para el reloj en tiempo real). Algunos de esos parámetros se seleccionan mediante *jumpers* en la propia placa pero la mayoría se configuran a través de un programa especial al que se puede acceder antes de arrancar el SO.

## Juego de chips (chipset)

El chipset es el conjunto de circuitos integrados (chips) de la placa base encargados de controlar las comunicaciones entre los diferentes componentes de la placa base. Un chipset se suele diseñar para una familia específica de microprocesadores.

El juego de chips suele estar distribuido en dos componentes principales:

- El puente norte (*north bridge*), encargado de las transferencias de mayor velocidad (principalmente con el microprocesador, la memoria, la tarjeta gráfica y el puente sur).
- El puente sur (*south bridge*), encargado de las transferencias entre el puente norte y el resto de periféricos con menores exigencias de velocidad de la placa.

![image-20220107115525434](/home/davidms_83/.config/Typora/typora-user-images/image-20220107115525434.png)

![image-20220107115543362](/home/davidms_83/.config/Typora/typora-user-images/image-20220107115543362.png)
