#!/bin/bash
contador=1
termina=5


while [ $contador -le $termina ]
do
    ./BIN/a.out $1 $contador >> salidas_BL.txt
    contador=$(( $contador + 1 ))
done
