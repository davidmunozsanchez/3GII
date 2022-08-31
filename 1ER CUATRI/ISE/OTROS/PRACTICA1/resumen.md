# PRÁCTICA 1

### Resumen de los vídeos

###### Lección 2

En la lección 2, tenemos 1 disco duro con dos particiones. Sda2 es el PV donde tenemos el grupo de volúmenes cl y en el que hay dos volúmenes lógicos.

Queremos añadir un disco para crear otro grupo de volúmenes y alojar /var como LV.

Una vez hemos añadido el disco, creamos el PV usando todo el disco o creando una partición.

 Usamos fdisk. Para darle formato al disco que acabamos de introducir. Con p vemos las distintas particiones. Con n añadimos una nueva y seguimos los pasos. Tenemos sdb1.

Creamos el PV. Usamos LVM. Dos tabuladores y vemos todo. pvcreate /dev/sdb1.

Ahora creamos el VG. vgextend VG PV (cl) (/dev/sdb1). vgs para ver cambios.

Cremos nuestro LV. lv.... ; lvcrreate -n -L. Tenemos new_var.

Para terminar tenemos que dar una serie de pasos:

1. Acceder al LV (montarlo).
2. Copiar info de /var a LV. De manera atómica.
3. Indicar al SO donde esta /var.
4. Liberar espacio.
5. El paso 0 es crear un FS para el lv y después montarlo.

Paso 1: creamos carpeta para montar lv. Para crear FS, mkfs. sudo mkfs -t ext4 /dev/cl/new_var. Lo hacemos accesible. sudo mount /dev/cl/new_var new_var/.

Paso 2: hacemos la copia de los datos. Pasamos a mantenimiento, systemctl isolate recue, system ctl status. Otra vez lo primero. Hacemos cp -a /var/. new_var/.

Paso 3: ahora decimos donde esta /var punto de montaje. Nos vamos al fstab. Modificamos. Desmontamos. umount /home/davidms/new_var,

Paso 4: hemos hecho un cp entonces tenemos que liberar espacio del /var original. Vamos marcha atrás. Editamos fstab y comentamos la línea que hemos añadido. mount -a. movemos /var a /var_old. No tenemos var, creamos var y con restorecon /var y con mount -a y ya estaría.



###### Lección 3

Partimos de exactamente lo mismo que antes pero ahora queremos añadir un /var en RAID1 cifrado. Añadimos sdb y sdc y crear md0 y un pv y hay dos opciones, extender el cl como la semana pasada pero no  sirve porque no tenemos seguro que se guarde en raid. Solución, creo nuevo VG y como LV pongo /var, que tiene que estar cifrado: tenemos LVM on LUKS y LUKS on LVM.

FDE (Full Disk Encription): no lo estamos consiguiendo porque no ciframos /boot ni grub.

Volvemos a la máquina virtual y añadimos los dos discos de 2gb.

Vamos a crear el RAID1: con mdadm **multidevice administration.** Vamos a crear las particiones correspondientes con fdisk en sdb y sdc. Tenemos sdb1 y sdc1. Vamos a crear nuestro RAID1 ahora, tenemos que indicar modo dispositivo opciones y distintos dispositivos. sudo mdadm --create /dev/md0 --level=1 --raid-devices=2 /dev/sdb1 /dev/sdc1.

Creamos nuestro PV a partir de md0. sudo pvcreate /dev/md0. Creamo nuestro VG: sudo vgcreate vg_raid1 /dev/md0. Creamos LV: sudo lvcreate -n new_var -L 1.8G vg_raid1.



Ahora vienen los pasos similares a lo que hicimos en la práctica anterior. Para cifrar utilizamos cryptsetup: dnf. No tenemos conexión: sudo ifup enp0s3. dnf search sudo dnf install cryptsetup -y.

Le damos formato al LV. sudo cryptsetup luksFormat /dev/vg_raid1/new_var  y ya estaría cifrado.

Tenemos que acceder a el, luksOpen /dev/vg_raid1/new_var vg_raid1-new_var_crypt

Si hacemos ls /dev/mapper/ vemos que todo va bien.

Pasamos a crear el sistema de archivos: sudo mkfs -t xfs /dev/mapper/vg_raid1-new_var_crypt.

System ctl.

mkdir /new_var

mount /dev/mapper/vg_raid1-new_var_crypt /new_var/

cp -a /var/. /new_var/

Ponemos en /etc/fstab /dev/mapper...

Ahora tenemos que indicar al SO que cuando arranque active el volumen lógico cifrado. etc/crypttab

Primero vemos  UUID con blkid/grep crypto

Redirigimos la salida a crypttab y editamos el archivo. Tenemos que dejar nombre y UUID.

Liberamos espacio: mv /var/ /var_old ... mkdir /var restorecon /var







