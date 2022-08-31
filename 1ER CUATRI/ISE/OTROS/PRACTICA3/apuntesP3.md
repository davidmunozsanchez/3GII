# Práctica 3

## Monitorización, automatización y "profiling"

Esta práctica consiste en la utilización de herramientas de monitorización del sistema para visualizar cómo se comporta el sistema ante ciertas actividades que los usuarios u otros servicios generan. Estas herramientas presentan medidas del sistema permitiendo generar informes e históricos que puedan ser de utilidad para un análisis a posteriori o para tomar decisiones sobre la marcha.



### Monitores para Hardware

Existen programas que nos permiten ver el estado hardware de la máquina.

1. En primer lugar, muchas BIOS (Basic Input Output System) nos permiten acceder a cierta información sobre el del HW (aunque ya están en extinción debido a UEFI, Universal Extended Firmwares).
2. Para no tener que reiniciar al querer usar la BIOS, tenemos otras herramientas como **hddtemp** y el proyecto **lm-sensors** (con su correspondiente GUI xsensors).
3. **lspci, lsusb, lshw** para listar HW de nuestro equipo.

El kernel de Linux permite conocer qué actividad ha ocurrido gracias a los mensajes que proporciona el kernel. Los podemos ver con **dmesg**.



### Monitores para Software

#### Subsistema de archivos

En linux (UNIX), todo se manipula a través de archivos. Los directorios /proc y /var contienen archivos con información tanto del software como del hardware. Se encuentran archivos fundamentales en la monitorización del comportamiento del sistema, de las aplicaciones y de los usuarios. El subdirectorio /var/log/ contiene los archivos en los que se vuelcan los logs de los servicios y algunas aplicaciones.

Una mala gestión de estos archivos puede derivar en un sistema caído. La rotación de los archivos de log (logrotate) se hace para evitar que los logs crezcan indefinidamente.

En esta web se profundiza más sobre los logs y logrotate: [LOGS Y LOGROTATE](https://atareao.es/ubuntu/rotado-de-logs-con-logrotate/)

Estos archivos nos ayudan a identificar errores y problemas, además esta tarea puede ser automatizada.

Con **mdadm y /proc/mdstat** podemos identificar, monitorizar y reconstruir los discos RAID que tenemos en nuestras máquinas virtuales.

##### /proc/mdstat

![1](/home/davidms_83/Documentos/3GII/ISE/PRACTICA3/1.png)

![](/home/davidms_83/Documentos/3GII/ISE/PRACTICA3/2.png)

##### mdadm

![3](/home/davidms_83/Documentos/3GII/ISE/PRACTICA3/3.png)![4](/home/davidms_83/Documentos/3GII/ISE/PRACTICA3/4.png)![5](/home/davidms_83/Documentos/3GII/ISE/PRACTICA3/5.png)

##### Recursos interesantes

- [Detecting, quering and testing](https://raid.wiki.kernel.org/index.php/Detecting,_querying_and_testing)
- [/proc/mdstat](https://raid.wiki.kernel.org/index.php/Mdstat)

#### Monitorizando un servicio o ejecución de un programa: strace

Para detectar problemas que no se muestran en los archivos de log.

Estos programas permiten hacer un traza de las llamadas al sistema realizadas por un programa (servicio) en ejecución, como es el caso de **strace**.

[Ejemplos de uso **strace**](https://conpilar.es/como-rastrear-la-ejecucion-del-programa-usando-el-comando-strace-de-linux/)



### Monitores generales

#### Munin

En Ubuntu está disponible sin añadir ningún repositorio adicional. En CentOS, hay un paquete disponible en el repositorio [EPEL](https://fedoraproject.org/wiki/EPEL/es#.C2.BFC.C3.B3mo_puedo_utilizar_estos_paquetes_adicionales.3F)(click para ver como activar el repositorio), después podremos usar dnf para instalar Munin.

Aquí tenemos una demo de [Munin corriendo](http://demo.munin-monitoring.org/).

#### Nagios y Naemon

Nagios ha pasado a ser proyecto empresarial, por eso ha surgido Naemon ([demo, uso de la interfaz Thruk](https://demo.thruk.org/demo/thruk/cgi-bin/login.cgi?demo/thruk)).

#### Ganglia

Monitoriza sistemas de cómputo distribuidos (normalmente para altas prestaciones) y permite una gran escalabilidad.

#### Zabbix

Nos centraremos en él en las lecciones. Para su instalación solo hay que añadir los repositorios e instalarlo con el correspondiente gestor de paquetes. [Aquí](https://www.zabbix.com/documentation/5.0/manual) encontramos toda la información para el seguimiento de las lecciones.

El objetivo es que seamos capaces de instalar este sistema de monitorización y configurar este para que se monitorice a sí mismo así como a CentOS (a través del agente) además de conocer cómo interactuar con el sistema de monitorización usando la API que proporcionar y conocer en qué consiste el protocolo SNMP.

#### Cacti

[Cacti](http://www.cacti.net/) es otro front-end para [RDRtool](http://oss.oetiker.ch/rrdtool/), que permite monitorizar muchos parámetros sin sobrecargar excesivamente el sistema.

#### AWstats

Monitor de servicios específicos de servidores web HTTP, FTP, correo, etc. Se puede compilar el [código fuente](https://awstats.sourceforge.io/) aunque están disponibles los paquetes en los repos.



### Automatización

#### cron y systemd

El servicio cron ha permitido ejecutar cada cierto intervalo de tiempo una tarea concreta. Con la introducción de systemd, cron puede seguir siendo usado pero su funcionamiento está basado en los timers que acrtivan services. [Documentación](https://access.redhat.com/documentation/es-es/red_hat_enterprise_linux/8/html/configuring_basic_system_settings/managing-services-with-systemd_configuring-basic-system-settings) de RedHat.

Podemos automatizar la ejecución de este script (ejemplo del pdf de SWAD) en systemd definiendo un timer dentro del directorio /etc/systemd/system/ que se encarga de gestionar un servicio. Por tanto, hemos de crear dos archivos: mon_raid.timer y mon_raid.service
[ Unit ]
D e s c r i p t i o n=Monitor RAID s t a t u s

[ Timer ]
OnCalendar=m i n u t e l y

[ Install ]
WantedBy=t i m e r s . t a r g e t }

[ Unit ]
D e s c r i p t i o n=Monitor RAID s t a t u s

[ Service ]
Type=s i m p l e
E x e c S t a r t=/u s r / b i n / python3 /home/ a l b e r t o /mon−r a i d . py

Para que pueda funcionar hemos de activar y habilitar el timer (de manera análoga a
lo que hicimos con los servicios tras instalarlos en la Práctica 2). Una vez hecho esto, podremos monitorizar su ejecución gracias al comando journalctl, p.ej. journalctl -u mon-raid --since="yesterday".

#### Scripts

##### Shell y comandos del sistema: grep, find, awk y sed

Existen cuatro comandos muy útiles (ejemplos) para la automatización de tareas: grep, find, awk y sed.

Con **sed** podemos buscar una cadena en un archiva y reemplazarla por otra.

Con **awk** podemos programar la manipulación de texto así como generar salidas más completas a partir de la información en un archivo.

Con **grep** podemos realizar filtrado de cadenas (ps -Af | grep firefox).

Con **find** podemos buscar archivos y una vez encontrados realizar acciones sobre ellos (find /home/alberto/docs -name '*pdf' -exec cp {} ~/PDFs \;).

##### Python y PHP

Mirar PDF prácticas.

##### A nivel de Plataforma: Ansible

Existen interfaces que permiten programar scripts y visualizar su ejecución de una manera cómoda y visual. Veremos una lección con el funcionamiento básico de [Ansible](https://servernotfound.es/como-instalar-y-configurar-ansible/)

### Profiling

#### Scripts

Para estudiar el comportamiento de los scripts, independientemente del lenguaje que usemos, podemos usar varios profilers por ejemplo, en Bash, podemos usar la opción set -x y modificar la variable local PS4 para que [muestre el tiempo](http://www.tldp.org/LDP/Bash-Beginners-Guide/html/sect_03_02.html).

En el caso de PHP tenemos [XHProf](https://github.com/facebook/xhprof) ([aquí](http://docs.moodle.org/dev/Profiling_PHP) tutorial de cómo realizar el profiling).

Por último, en el caso de Python, se pude utilizar el módulo [eProfile](https://docs.python.org/3/library/profile.html).

#### SQL

El MySQL y MariaDB que instalamos en la práctica anterior tienen un ["profiler"](http://dev.mysql.com/doc/refman/5.0/en/show-profiles.html) para analizar cuánto tardan las consultas. Son útiles también las herramientas **MySQLWorkBench** y comandos como **mysqloptimize**, que optimizan la ejecución.