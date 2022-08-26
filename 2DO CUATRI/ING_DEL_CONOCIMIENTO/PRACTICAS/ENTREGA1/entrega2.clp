;;;;;;; JUGADOR DE 4 en RAYA ;;;;;;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;; 
;;;;;;;;;; Version de 4 en raya clásico: Tablero de 6x7, donde se introducen fichas por arriba
;;;;;;;;;;;;;;;;;;;;;;; y caen hasta la posicion libre mas abajo
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

;;;;;;;;;;;;;;;; Hechos para representar un estado del juego

;;;;;;; (Turno M|J)   representa a quien corresponde el turno (M maquina, J jugador)
;;;;;;; (Tablero Juego ?i ?j _|M|J) representa que la posicion i,j del tablero esta vacia (_), o tiene una ficha propia (M) o tiene una ficha del jugador humano (J)

;;;;;;;;;;;;;;;; Hechos para representar estado del analisis
;;;;;;; (Tablero Analisis Posicion ?i ?j _|M|J) representa que en el analisis actual la posicion i,j del tablero esta vacia (_), o tiene una ficha propia (M) o tiene una ficha del jugador humano (J)
;;;;;;; (Sondeando ?n ?i ?c M|J)  ; representa que estamos analizando suponiendo que la ?n jugada h sido ?i ?c M|J
;;;

;;;;;;;;;;;;; Hechos para representar una jugadas

;;;;;;; (Juega M|J ?columna) representa que la jugada consiste en introducir la ficha en la columna ?columna 


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;; INICIALIZAR ESTADO


(deffacts Estado_inicial
(Tablero Juego 1 1 _) (Tablero Juego 1 2 _) (Tablero Juego 1 3 _) (Tablero Juego  1 4 _) (Tablero Juego  1 5 _) (Tablero Juego  1 6 _) (Tablero Juego  1 7 _)
(Tablero Juego 2 1 _) (Tablero Juego 2 2 _) (Tablero Juego 2 3 _) (Tablero Juego 2 4 _) (Tablero Juego 2 5 _) (Tablero Juego 2 6 _) (Tablero Juego 2 7 _)
(Tablero Juego 3 1 _) (Tablero Juego 3 2 _) (Tablero Juego 3 3 _) (Tablero Juego 3 4 _) (Tablero Juego 3 5 _) (Tablero Juego 3 6 _) (Tablero Juego 3 7 _)
(Tablero Juego 4 1 _) (Tablero Juego 4 2 _) (Tablero Juego 4 3 _) (Tablero Juego 4 4 _) (Tablero Juego 4 5 _) (Tablero Juego 4 6 _) (Tablero Juego 4 7 _)
(Tablero Juego 5 1 _) (Tablero Juego 5 2 _) (Tablero Juego 5 3 _) (Tablero Juego 5 4 _) (Tablero Juego 5 5 _) (Tablero Juego 5 6 _) (Tablero Juego 5 7 _)
(Tablero Juego 6 1 _) (Tablero Juego 6 2 _) (Tablero Juego 6 3 _) (Tablero Juego 6 4 _) (Tablero Juego 6 5 _) (Tablero Juego 6 6 _) (Tablero Juego 6 7 _)
(Jugada 0)
)

(defrule Elige_quien_comienza
=>
(printout t "Quien quieres que empieze: (escribre M para la maquina o J para empezar tu) ")
(assert (Turno (read)))
)

;;;;;;;;;;;;;;;;;;;;;;; MUESTRA POSICION ;;;;;;;;;;;;;;;;;;;;;;;
(defrule muestra_posicion
(declare (salience 10))
(muestra_posicion)
(Tablero Juego 1 1 ?p11) (Tablero Juego 1 2 ?p12) (Tablero Juego 1 3 ?p13) (Tablero Juego 1 4 ?p14) (Tablero Juego 1 5 ?p15) (Tablero Juego 1 6 ?p16) (Tablero Juego 1 7 ?p17)
(Tablero Juego 2 1 ?p21) (Tablero Juego 2 2 ?p22) (Tablero Juego 2 3 ?p23) (Tablero Juego 2 4 ?p24) (Tablero Juego 2 5 ?p25) (Tablero Juego 2 6 ?p26) (Tablero Juego 2 7 ?p27)
(Tablero Juego 3 1 ?p31) (Tablero Juego 3 2 ?p32) (Tablero Juego 3 3 ?p33) (Tablero Juego 3 4 ?p34) (Tablero Juego 3 5 ?p35) (Tablero Juego 3 6 ?p36) (Tablero Juego 3 7 ?p37)
(Tablero Juego 4 1 ?p41) (Tablero Juego 4 2 ?p42) (Tablero Juego 4 3 ?p43) (Tablero Juego 4 4 ?p44) (Tablero Juego 4 5 ?p45) (Tablero Juego 4 6 ?p46) (Tablero Juego 4 7 ?p47)
(Tablero Juego 5 1 ?p51) (Tablero Juego 5 2 ?p52) (Tablero Juego 5 3 ?p53) (Tablero Juego 5 4 ?p54) (Tablero Juego 5 5 ?p55) (Tablero Juego 5 6 ?p56) (Tablero Juego 5 7 ?p57)
(Tablero Juego 6 1 ?p61) (Tablero Juego 6 2 ?p62) (Tablero Juego 6 3 ?p63) (Tablero Juego 6 4 ?p64) (Tablero Juego 6 5 ?p65) (Tablero Juego 6 6 ?p66) (Tablero Juego 6 7 ?p67)
=>
(printout t crlf)
(printout t ?p11 " " ?p12 " " ?p13 " " ?p14 " " ?p15 " " ?p16 " " ?p17 crlf)
(printout t ?p21 " " ?p22 " " ?p23 " " ?p24 " " ?p25 " " ?p26 " " ?p27 crlf)
(printout t ?p31 " " ?p32 " " ?p33 " " ?p34 " " ?p35 " " ?p36 " " ?p37 crlf)
(printout t ?p41 " " ?p42 " " ?p43 " " ?p44 " " ?p45 " " ?p46 " " ?p47 crlf)
(printout t ?p51 " " ?p52 " " ?p53 " " ?p54 " " ?p55 " " ?p56 " " ?p57 crlf)
(printout t ?p61 " " ?p62 " " ?p63 " " ?p64 " " ?p65 " " ?p66 " " ?p67 crlf)
(printout t  crlf)
)


;;;;;;;;;;;;;;;;;;;;;;; RECOGER JUGADA DEL CONTRARIO ;;;;;;;;;;;;;;;;;;;;;;;
(defrule mostrar_posicion
(declare (salience 9999))
(Turno J)
=>
(assert (muestra_posicion))
)

(defrule jugada_contrario
?f <- (Turno J)
=>
(printout t "en que columna introduces la siguiente ficha? ")
(assert (Juega J (read)))
(retract ?f)
)

(defrule juega_contrario_check_entrada_correcta
(declare (salience 1))
?f <- (Juega J ?c)
(test (and (neq ?c 1) (and (neq ?c 2) (and (neq ?c 3) (and (neq ?c 4) (and (neq ?c 5) (and (neq ?c 6) (neq ?c 7))))))))
=>
(printout t "Tienes que indicar un numero de columna: 1,2,3,4,5,6 o 7" crlf)
(retract ?f)
(assert (Turno J))
)

(defrule juega_contrario_check_columna_libre
(declare (salience 1))
?f <- (Juega J ?c)
(Tablero Juego 1 ?c ?X) 
(test (neq ?X _))
=>
(printout t "Esa columna ya esta completa, tienes que jugar en otra" crlf)
(retract ?f)
(assert (Turno J))
)

(defrule juega_contrario_actualiza_estado
?f <- (Juega J ?c)
?g <- (Tablero Juego ?i ?c _)
(Tablero Juego ?j ?c ?X) 
(test (= (+ ?i 1) ?j))
(test (neq ?X _))
=>
(retract ?f ?g)
(assert (Turno M) (Tablero Juego ?i ?c J))
)

(defrule juega_contrario_actualiza_estado_columna_vacia
?f <- (Juega J ?c)
?g <- (Tablero Juego 6 ?c _)
=>
(retract ?f ?g)
(assert (Turno M) (Tablero Juego 6 ?c J))
)


;;;;;;;;;;; ACTUALIZAR  ESTADO TRAS JUGADA DE CLISP ;;;;;;;;;;;;;;;;;;

(defrule juega_clisp_actualiza_estado
?f <- (Juega M ?c)
?g <- (Tablero Juego ?i ?c _)
(Tablero Juego ?j ?c ?X) 
(test (= (+ ?i 1) ?j))
(test (neq ?X _))
=>
(retract ?f ?g)
(assert (Turno J) (Tablero Juego ?i ?c M))
)

(defrule juega_clisp_actualiza_estado_columna_vacia
?f <- (Juega M ?c)
?g <- (Tablero Juego 6 ?c _)
=>
(retract ?f ?g)
(assert (Turno J) (Tablero Juego 6 ?c M))
)

;;;;;;;;;;; CLISP JUEGA SIN CRITERIO ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
(defrule elegir_jugada_aleatoria
(declare (salience -9998))
?f <- (Turno M)
=>
(assert (Jugar (random 1 7)))
(retract ?f)
)

(defrule comprobar_posible_jugada_aleatoria
?f <- (Jugar ?c)
(Tablero Juego 1 ?c M|J)
=>
(retract ?f)
(assert (Turno M))
)

(defrule clisp_juega_sin_criterio
(declare (salience -9999))
?f<- (Jugar ?c)
=>
(printout t "JUEGO en la columna (sin criterio) " ?c crlf)
(retract ?f)
(assert (Juega M ?c))
(printout t "Juego sin razonar, que mal"  crlf) 
)


;;;;;;;;;;;;;;;;;;;;;;;;;;;  Comprobar si hay 4 en linea ;;;;;;;;;;;;;;;;;;;;;

(defrule cuatro_en_linea_horizontal
(declare (salience 9999))
(Tablero ?t ?i ?c1 ?jugador)
(Tablero ?t ?i ?c2 ?jugador) 
(test (= (+ ?c1 1) ?c2))
(Tablero ?t ?i ?c3 ?jugador)
(test (= (+ ?c1 2) ?c3))
(Tablero ?t ?i ?c4 ?jugador)
(test (= (+ ?c1 3) ?c4))
(test (or (eq ?jugador M) (eq ?jugador J) ))
=>
(assert (Cuatro_en_linea ?t ?jugador horizontal ?i ?c1))
)

(defrule cuatro_en_linea_vertical
(declare (salience 9999))
?f <- (Turno ?X)
(Tablero ?t ?i1 ?c ?jugador)
(Tablero ?t ?i2 ?c ?jugador)
(test (= (+ ?i1 1) ?i2))
(Tablero ?t ?i3 ?c  ?jugador)
(test (= (+ ?i1 2) ?i3))
(Tablero ?t ?i4 ?c  ?jugador)
(test (= (+ ?i1 3) ?i4))
(test (or (eq ?jugador M) (eq ?jugador J) ))
=>
(assert (Cuatro_en_linea ?t ?jugador vertical ?i1 ?c))
)

(defrule cuatro_en_linea_diagonal_directa
(declare (salience 9999))
?f <- (Turno ?X)
(Tablero ?t ?i ?c ?jugador)
(Tablero ?t ?i1 ?c1 ?jugador)
(test (= (+ ?i 1) ?i1))
(test (= (+ ?c 1) ?c1))
(Tablero ?t ?i2 ?c2  ?jugador)
(test (= (+ ?i 2) ?i2))
(test (= (+ ?c 2) ?c2))
(Tablero ?t ?i3 ?c3  ?jugador)
(test (= (+ ?i 3) ?i3))
(test (= (+ ?c 3) ?c3))
(test (or (eq ?jugador M) (eq ?jugador J) ))
=>
(assert (Cuatro_en_linea ?t ?jugador diagonal_directa ?i ?c))
)

(defrule cuatro_en_linea_diagonal_inversa
(declare (salience 9999))
?f <- (Turno ?X)
(Tablero ?t ?i ?c ?jugador)
(Tablero ?t ?i1 ?c1 ?jugador)
(test (= (+ ?i 1) ?i1))
(test (= (- ?c 1) ?c1))
(Tablero ?t ?i2 ?c2  ?jugador)
(test (= (+ ?i 2) ?i2))
(test (= (- ?c 2) ?c2))
(Tablero ?t ?i3 ?c3  ?jugador)
(test (= (+ ?i 3) ?i3))
(test (= (- ?c 3) ?c3))
(test (or (eq ?jugador M) (eq ?jugador J) ))
=>
(assert (Cuatro_en_linea ?t ?jugador diagonal_inversa ?i ?c))
)

;;;;;;;;;;;;;;;;;;;; DESCUBRE GANADOR
(defrule gana_fila
(declare (salience 9999))
?f <- (Turno ?X)
(Cuatro_en_linea Juego ?jugador horizontal ?i ?c)
=>
(printout t ?jugador " ha ganado pues tiene cuatro en linea en la fila " ?i crlf)
(retract ?f)
(assert (muestra_posicion))
) 

(defrule gana_columna
(declare (salience 9999))
?f <- (Turno ?X)
(Cuatro_en_linea Juego ?jugador vertical ?i ?c)
=>
(printout t ?jugador " ha ganado pues tiene cuatro en linea en la columna " ?c crlf)
(retract ?f)
(assert (muestra_posicion))
) 

(defrule gana_diagonal_directa
(declare (salience 9999))
?f <- (Turno ?X)
(Cuatro_en_linea Juego ?jugador diagonal_directa ?i ?c)
=>
(printout t ?jugador " ha ganado pues tiene cuatro en linea en la diagonal que empieza la posicion " ?i " " ?c   crlf)
(retract ?f)
(assert (muestra_posicion))
) 

(defrule gana_diagonal_inversa
(declare (salience 9999))
?f <- (Turno ?X)
(Cuatro_en_linea Juego ?jugador diagonal_inversa ?i ?c)
=>
(printout t ?jugador " ha ganado pues tiene cuatro en linea en la diagonal hacia arriba que empieza la posicin " ?i " " ?c   crlf)
(retract ?f)
(assert (muestra_posicion))
) 


;;;;;;;;;;;;;;;;;;;;;;;  DETECTAR EMPATE

(defrule empate
(declare (salience -9999))
(Turno ?X)
(Tablero Juego 1 1 M|J)
(Tablero Juego 1 2 M|J)
(Tablero Juego 1 3 M|J)
(Tablero Juego 1 4 M|J)
(Tablero Juego 1 5 M|J)
(Tablero Juego 1 6 M|J)
(Tablero Juego 1 7 M|J)
=>
(printout t "EMPATE! Se ha llegado al final del juego sin que nadie gane" crlf)
)

;;;;;;;;;;;;;;;;;;;;;; CONOCIMIENTO EXPERTO ;;;;;;;;;;
;;;;;Aniadir conocimiento para que juege como vosotros jugariais !!!!!!!!!!!!

;;;;;EJERCICIOS PROPUESTOS PRADO
;;;;;;EJERCICIO1: aniado reglas para deducir la posicion siguiente y anterior a una posicion

;;;siguiente posicion h
(defrule siguienteh
(Tablero ?t ?f ?c ?j)
(test(< ?c 7))
=>
(assert(siguiente ?f ?c h ?f (+ ?c 1)))
)

;;;anterior posicion h
(defrule anteriorh
(Tablero ?t ?f ?c ?j)
(test(> ?c 1))
=>
(assert(anterior ?f ?c h ?f (- ?c 1)))
)

;;;siguiente posicion v
(defrule siguientev
(Tablero ?t ?f ?c ?j)
(test(> ?f 1))
=>
(assert(siguiente ?f ?c v (- ?f 1) ?c))
)

;;;anterior posicion v
(defrule anteriorv
(Tablero ?t ?f ?c ?j)
(test(< ?f 6))
=>
(assert(anterior ?f ?c v (+ ?f 1) ?c))
)

;;;siguiente posicion diagonalmente
(defrule siguiented
(Tablero ?t ?f ?c ?j)
(test(< ?c 7))
(test(> ?f 1))
=>
(assert(siguiente ?f ?c d (- ?f 1) (+ ?c 1)))
)

;;;anterior posicion diagonalmente
(defrule anteriord
(Tablero ?t ?f ?c ?j)
(test(> ?c 1))
(test(< ?f 6))
=>
(assert(anterior ?f ?c d (+ ?f 1) (- ?c 1)))
)

;;;siguiente posicion diagonal inversa
(defrule siguientedi
(Tablero ?t ?f ?c ?j)
(test(< ?c 7))
(test(< ?f 6))
=>
(assert(siguiente ?f ?c di (+ ?f 1) (+ ?c 1)))
)

;;;anterior posicion diagonal inversa
(defrule anteriordi
(Tablero ?t ?f ?c ?j)
(test(> ?c 1))
(test(> ?f 1))
=>
(assert(anterior ?f ?c di (- ?f 1) (- ?c 1)))
)

;;;EJERCICIO 2
;;;Hay que aniadir reglas para que el sistema deduzca donde caeria la ficha segun la columna
;;;en la que se introduzca

;;;Si la columna esta vacia
(defrule caeria_ini
(Tablero ?t 6 ?c _)
=>
(assert(caeria 6 ?c)) ;;;Siempre caeria en la fila 6, por estar vacia la columna
)

;;;Para los demas casos, si la posicion donde cae esta bien
(defrule caeria
?regla <- (caeria ?f ?c) ;;;Para posteriormente eliminarla, no nos sirve si la va a ocupar
(Tablero ?t ?f ?c ?j)
(test (neq ?j _)) ;;Para eliminarla tiene que estar ocupada por M o J
(test (> ?f 1)) ;;Comprobacion que nos asegura que la columna no esta llena
=>
(retract ?regla)
(assert (caeria (- ?f 1) ?c))
)

;;;Si la columna esta llena, esta regla ya no se aniade porque la fila no es mayor que 1, ahora
;;;debemos eliminar la regla anterior
(defrule eliminar_caeria
?regla <- (caeria ?f ?c)
(Tablero ?t ?f ?c ?j)
(test (neq ?j _)) ;;Para eliminarla tiene que estar ocupada por M o J
(test (eq ?f 1))
=>
(retract ?regla)
)

;;;EJERCICIO 3
;;;Hay que detectar tanto para la maquina como para el jugador si hay 2 en linea
(defrule conecta2
(Tablero ?t ?f1 ?c1 ?jugador1)
(Tablero ?t ?f2 ?c2 ?jugador2)
(test (neq ?jugador1 _)) ;;;No puede estar vacia la casilla
(test (neq ?jugador2 _))
(test (eq ?jugador1 ?jugador2)) ;;;Dos en linea del mismo jugador
(siguiente ?f1 ?c1 ?d ?f2 ?c2) ;;;Con usar siguiente basta, no hace falta usar anterior
=>
(assert (conecta2 ?t ?d ?f1 ?c1 ?f2 ?c2 ?jugador1))
)

;;;EJERCICIO 4
;;;Igual que antes pero con 3 en linea
(defrule conecta3_1
(conecta2 ?t ?d ?f1 ?c1 ?f2 ?c2 ?jugador)
(Tablero ?t ?f3 ?c3 ?jugador3)
(test (neq ?jugador3 _))
(test (eq ?jugador ?jugador3)) ;; 2 en linea del mismo jugador y la tercera tambien
(siguiente ?f2 ?c2 ?d ?f3 ?c3) ;;
=>
(assert (conecta3 ?t ?d ?f1 ?c1 ?f3 ?c3 ?jugador3))
)

(defrule conecta3_2
(conecta2 ?t ?d ?f1 ?c1 ?f2 ?c2 ?jugador)
(Tablero ?t ?f3 ?c3 ?jugador3)
(test (neq ?jugador3 _))
(test (eq ?jugador ?jugador3)) ;; 2 en linea del mismo jugador y la tercera tambien
(anterior ?f1 ?c1 ?d ?f3 ?c3) ;;Uno anterior en la misma direccion que conecta2
=>
(assert (conecta3 ?t ?d ?f3 ?c3 ?f2 ?c2 ?jugador3))
)


;;;EJERCICIO 5
;;;Con la ayuda de los hechos anteriores, vamos a deducir cuando uno de los jugadores puede ganar,
;;;es decir, que haya 3 en linea y un blanco siguiendo a estos 3
(defrule conecta3_blanco
(conecta3 ?t ?d ?f1 ?c1 ?f3 ?c3 ?jugador)
(siguiente ?f3 ?c3 ?d ?f4 ?c4)
(caeria ?f4 ?c4) 
=>
(assert (ganaria ?t ?jugador ?c4)) ;;;Guardamos la columna para ganar
)

;;;Procedemos de igual forma pero para casillas anteriores
(defrule blanco_conecta3
(conecta3 ?t ?d ?f1 ?c1 ?f3 ?c3 ?jugador)
(anterior ?f1 ?c1 ?d ?f4 ?c4)
(caeria ?f4 ?c4) ;;;Podemos usar esa casilla?
=>
(assert (ganaria ?t ?jugador ?c4)) ;;;Guardamos la columna para ganar
)

(defrule conecta2_blanco_solo
(conecta2 ?t ?d ?f1 ?c1 ?f2 ?c2 ?jugador)
(Tablero ?t ?f4 ?c4 ?jugador3)
(test (neq ?jugador3 _))
(test (eq ?jugador ?jugador3))
(siguiente ?f2 ?c2 ?d ?f3 ?c3)
(siguiente ?f3 ?c3 ?d ?f4 ?c4) ;;;Si esta en el siguiente del siguiente al conecta2 en la misma direccion
(caeria ?f3 ?c3) ;;Tenemos que poder tirar la ficha en el hueco para ganar
=>
(assert (ganaria ?t ?jugador ?c3))
)

(defrule solo_blanco_conecta2
(conecta2 ?t ?d ?f1 ?c1 ?f2 ?c2 ?jugador)
(Tablero ?t ?f4 ?c4 ?jugador3)
(test (neq ?jugador3 _))
(test (eq ?jugador ?jugador3))
(anterior ?f1 ?c1 ?d ?f3 ?c3)
(anterior ?f3 ?c3 ?d ?f4 ?c4) ;;;Idem a antes, si esta en el anterior del anterior
(caeria ?f3 ?c3)
=>
(assert (ganaria ?t ?jugador ?c3))
)

;;;A partir de aqui, vamos a hacer que la maquina juegue con sentido
;;;(basta con asignar una prioridad positiva puesto que el juego sin criterio
;;;tiene prioridad negativa)

;;;Lo primero sera que si la maquina puede ganar, ganara
(defrule ganare
(declare (salience -9000))
(Turno M)
(ganaria ?t M ?c)
=>
(printout t "JUEGO en la columna (con criterio) " ?c " para ganar." crlf)
(assert (Juega M ?c))
)

;;;Ahora se evitara que la maquina pierda, es decir, que nos corte las jugadas en las que
;;;el jugador tenga ya 3 en linea o pueda ganar (los ganaria) meter aqui el retract del ganaria?
(defrule no_quiero_perder
(declare (salience -9001))
(Turno M)
(ganaria ?t J ?c)
=>
(printout t "JUEGO en la columna (con criterio) " ?c " porque si no pierdo." crlf)
(assert (Juega M ?c))
)

;;;Ahora haremos que la maquina nos ponga las cosas dificiles e intente cortar cuaquier
;;;secuencia de 2 del jugador o si hay una secuencia de 2 suya, la continua
(defrule no_te_crezcas
(declare (salience -9002))
(Turno M)
(conecta2 ?t ?d ?f1 ?c1 ?f2 ?c2 ?jugador)
(siguiente ?f2 ?c2 ?d ?f3 ?c3)
(caeria ?f3 ?c3)
=>
(printout t "JUEGO en la columna " ?c3 " porque " ?jugador " tiene dos en linea empezando en la posicion  " ?f1 " " ?c1 crlf)
(assert (Juega M ?c3))
)

;;;Lo mismo pero con la casilla anterior
(defrule no_te_crezcas2
(declare (salience -9002))
(Turno M)
(conecta2 ?t ?d ?f1 ?c1 ?f2 ?c2 ?jugador)
(anterior ?f1 ?c1 ?d ?f3 ?c3)
(caeria ?f3 ?c3)
=>
(printout t "JUEGO en la columna " ?c3 " porque " ?jugador " tiene dos en linea empezando en la posicion  " ?f1 " " ?c1 crlf)
(assert (Juega M ?c3))
)

;;;Ahora si tengo una M, la continuo
(defrule en_linea2
(declare (salience -9003))
(Turno M)
(Tablero ?t ?f ?c M)
(siguiente ?f ?c ?d ?f2 ?c2)
(caeria ?f2 ?c2)
=>
(printout t "JUEGO en la columna " ?c2 " para intentar continuar." crlf)
(assert (Juega M ?c2))
)

(defrule en_linea2_ant
(declare (salience -9003))
(Turno M)
(Tablero ?t ?f ?c M)
(anterior ?f ?c ?d ?f2 ?c2)
(caeria ?f2 ?c2)
=>
(printout t "JUEGO en la columna " ?c2 " para intentar continuar." crlf)
(assert (Juega M ?c2))
)

;;;Elimino los ganaria que ya no me sirven para ganar
(defrule ya_no_gano
(declare (salience 1))
?regla <- (ganaria ?t ?jugador ?c)
(Juega ?jugador2 ?c)
=>
(retract ?regla)
)
