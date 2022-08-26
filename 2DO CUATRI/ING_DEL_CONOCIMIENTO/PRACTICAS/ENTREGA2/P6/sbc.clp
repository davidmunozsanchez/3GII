
(defmodule FUNCION2)
;;;Partiremos de una base del conocimiento donde tenemos 
(deffacts ramas 
    (rama CSI)
    (rama TIC)
    (rama SI)
    (rama IC)
    (rama IS))

(deffacts sies_noes
    (sies duda1 0)
    (sies duda2 0)
    (noes duda1 0)
    (noes duda2 0))
;;Dificultades 1 la mas dificil y 5 la menos complicada
(deffacts dificultades 
    (dificultad CSI 1)
    (dificultad IS 2)
    (dificultad TIC 3)
    (dificultad SI 4)
    (dificultad IC 5))

(deffacts preguntas
    (pregunta CSI "Te gustan las matematicas?")
    (pregunta CSI "Te gusta lo relacionado con la IA?")
    (pregunta TIC "Te gustan las redes y la web?")
    (pregunta TIC "Te gusta la programacion?")
    (pregunta SI "Te gustan las bases de datos?")
    (pregunta SI "Te interesaria administrar datos?")
    (pregunta IC "Te gusta el HARDWARE y la Ingenieria de Computadores?")
    (pregunta IC "Te gusta la robotica?")
    (pregunta IS "Te gusta la programacion?")
    (pregunta IS "Te gusta la programacion web?"))

(defrule pregunta1
    (declare (salience 12)) ;Lo que se hace primero
    =>
        (printout t "Introduce la primera asignatura sobre la que estes dudando" crlf)
        (assert (duda1 (read)))
        )

(defrule pregunta2
    (declare (salience 11)) ;Lo que se hace primero
        =>
            (printout t "Introduce la segunda asignatura sobre la que estes dudando" crlf)
            (assert (duda2 (read)))
    )

(defrule pregunta3
    (declare (salience 10))
    (duda1 ?a) ;Lo que se hace primero
           =>
                (printout t "Introduce la rama que mas se relacione con " ?a crlf)
                (assert (rama duda1 (read)))
     )

(defrule pregunta4
    (declare (salience 10))
    (duda2 ?a) ;Lo que se hace primero
           =>
                (printout t "Introduce la rama que mas se relacione con " ?a crlf)
                (assert (rama duda2 (read)))
     )

;;Si el usuario no introduce alguna rama, por defecto se tomará como IS, 
;;dado que es una de las más solicitadas y con la que más tienen que ver muchas asignaturas
(defrule rama_por_defecto
    (declare (salience 8))
    ?regla <- (rama ?a ?b)
    (not (rama ?b)) ;;No hay ninguna rama con ese nombre
    =>
        (assert (rama ?a IS))
        (retract ?regla)
)
;;;Ahora se preguntarán varias preguntas segun con que ramas se relacionen las asignaturas antes de preguntar podriamos poner
;;;un razonamiento por defecto que devuelva una explicacion siempre que se nos pida elegir dos asignaturas
;;;de la misma rama
(defrule final_por_defecto
    (declare (salience 7))
    (rama duda1 ?a)
    (rama duda2 ?b)
    (test (eq ?a ?b))
    =>
        (assert (conclusion IND "Porque no puedo elegir entre dos asignaturas de distinta rama."))
        )


(defrule preguntas_asig_primera
    (not (conclusion ?p ?t))
    (duda1 ?a)
    (rama duda1 ?b)
    (pregunta ?b ?c)
    =>
        (printout t ?c crlf)
        (assert (respuesta duda1 (read)))
)

(defrule preguntas_asig_SEGUNDA
    (not (conclusion ?p ?t))
    (duda2 ?a)
    (rama duda2 ?b)
    (pregunta ?b ?c)
    =>
        (printout t ?c crlf)
        (assert (respuesta duda2 (read)))
)

;;;Asumo que si el usuario no me responde o me responde algo disinto de SI o NO,
;;;su respuesta sera un NO a la pregunta
(defrule no_por_defecto
    ?regla <- (respuesta ?a ?b)
    (and (test (neq ?b SI)) (test (neq ?b NO)))
    =>
        (assert (respuesta ?a NO))
        (retract ?regla)
)

;;;El razonamiento a seguir sera contando el numero de SIES Y NOES para cada asignatura. Devolveremos la asignatura con mas sies
;;;En caso de empate, se devuelve por defecto la asignatura relacionada con la rama más fácil
(defrule razonamiento2
    ?regla1 <- (respuesta duda1 ?b)
    (test (eq ?b SI))
    ?regla2 <- (sies duda1 ?z)
    =>
        (bind ?z (+ ?z 1))
        (assert (sies duda1 ?z))
        (retract ?regla1 ?regla2)
    )

(defrule razonamiento4
    ?regla1 <- (respuesta duda2 ?b)
    (test (eq ?b SI))
    ?regla2 <- (sies duda2 ?z)
    =>
        (bind ?z (+ ?z 1))
        (assert (sies duda2 ?z))
        (retract ?regla1 ?regla2)
    )

(defrule razonamiento6
    ?regla1 <- (respuesta duda1 ?b)
    (test (eq ?b NO))
    ?regla2 <- (noes duda1 ?z)
    =>
        (bind ?z (+ ?z 1))
        (assert (noes duda1 ?z))
        (retract ?regla1 ?regla2)
    )

(defrule razonamiento8
    ?regla1 <- (respuesta duda2 ?b)
    (test (eq ?b NO))
    ?regla2 <- (noes duda2 ?z)
    =>
        (bind ?z (+ ?z 1))
        (assert (noes duda2 ?z))
        (retract ?regla1 ?regla2)
    )



(defrule conclusiones
    (declare (salience -1))
    (duda1 ?x)
    (duda2 ?y)
    (sies duda1 ?a)
    (sies duda2 ?b)
    (test (> ?a ?b))
    =>
    (assert (conclusion ?x "Las preguntas relacionadas con la rama de esa asingatura son mas positivas que las de la otra")))

(defrule conclusiones2
    (declare (salience -1))
    (duda1 ?x)
    (duda2 ?y)
    (sies duda1 ?a)
    (sies duda2 ?b)
    (test (< ?a ?b))
    =>
    (assert (conclusion ?y "Las preguntas relacionadas con la rama de esa asingatura son mas positivas que las de la otra")))

(defrule conclusiones3
    (declare (salience -1))
    (duda1 ?x)
    (rama duda1 ?c)
    (duda2 ?y)
    (rama duda2 ?d)
    (sies duda1 ?a)
    (sies duda2 ?b)
    (test (= ?a ?b))
    =>
    (assert (conclusion IND "Hay igual numero de preguntas positivas y negativas")))


(defrule mostrar_conclusion
    (declare (salience -2))
    (conclusion ?a ?b)
    =>
        (printout t "Te recomiendo " ?a " porque " ?b crlf))

)




;;;;MODULO CON EL SISTEMA EXPERTO DE LA P3
(defmodule FUNCION1)

;;;Para representar que el sistema aconseja elegir una rama <nombre de la rama> por   
;;;el motivo “<texto del motivo>” utilizaremos el hecho
;;;(Consejo <nombre de la rama> “<texto del motivo>”)

(deffacts Ramas
(Rama Computacion_y_Sistemas_Inteligentes CSI)
(Rama Ingenieria_del_Software IS)
(Rama Ingenieria_de_Computadores IC)
(Rama Sistemas_de_Informacion SI)
(Rama Tecnologias_de_la_Informacion TI)
)

;;;Si la nota es media o alta cogeremos la de mayor dificultad disponible y si es baja la de menor
(deffacts Dificultades
(Dificultad CSI 1)
(Dificultad IS 2)
(Dificultad TI 3)
(Dificultad SI 4)
(Dificultad IC 5)
)

;;;El sistema preguntara siguiendo un orden logico (de mayor a menor importancia). Una
;;;pregunta sera importante cuando mas informacion nos proporcione
;;;Lo primero sera preguntar si prefiere asignaturas relacionadas con Software o Hardware
;;;El sistema toma valores para el hecho hard_soft HARDWARE/SOFTWARE
(defrule pregunta1
(declare (salience 10)) ;;;Prioridad alta para preguntar antes de calcular
=>
    (printout t "Si quieres evadir la pregunta escribe ne (en minusculas): " crlf)
   (printout t "Estas mas interesado/a por el hardware o el software? (Escribe H o S): " crlf)
   (assert (hardsoft (read)))
)

;;;Aqui normalizamos la respuesta para que sea mas legible el codigo
(defrule hardware_software
(declare (salience 10))
?regla <- (hardsoft S)
=>
    (retract ?regla)
    (assert (hard_soft SOFTWARE))
)

(defrule hardware_software2
(declare (salience 10))
?regla <- (hardsoft H)
=>
    (retract ?regla)
    (assert (hard_soft HARDWARE))
)

;;;El sistema pregunta la nota media y que se aniadira como hecho de la forma
;;;not_media valor_numerico. Si no se introduce un numero se requerira de nuevo.
;;;posteriormente ese valor numerico se cambiará por ALTA/MEDIA/BAJA
(defrule pregunta2
(declare (salience 9))
=>
   (printout t "Cual es tu nota media?: " crlf)
   (bind ?respuesta (read))
   (while (not (numberp ?respuesta)) do
        (printout t "Eso no es un numero." crlf)
        (printout t "Escribe un numero: ")
        (bind ?respuesta (read)))
   (assert (not_media ?respuesta))
)

;;;Cambiamos a nota media normalizada, segun la franja a la que pertenezca el float
(defrule cambio_not_media
(declare (salience 8))
?regla <- (not_media ?x)
(test (>= ?x 5))
(test (< ?x 6))
=>
    (retract ?regla)
    (assert (not_media_norm BAJA))
)

(defrule cambio_not_media2
(declare (salience 8))
?regla <- (not_media ?x)
(test (>= ?x 6))
(test (< ?x 7))
=>
    (retract ?regla)
    (assert (not_media_norm MEDIA))
)

(defrule cambio_not_media3
(declare (salience 8))
?regla <- (not_media ?x)
(test (>= ?x 7))
(test (<= ?x 10))
=>
    (retract ?regla)
    (assert (not_media_norm ALTA))
)


(defrule cambio_default
(declare (salience 8))
?regla <- (not_media ?x)
(or (test (< ?x 0))
(test (> ?x 10)))
=>
    (retract ?regla)
    (assert (not_media_norm _))
)

;;;La siguiente pregunta es si le gusta programar. A partir de esta pregunta obtendremos
;;;el hecho programacion SI/NO.
(defrule pregunta3
(declare (salience 7))
=>
   (printout t "Te gusta programar?(Escribe SI/NO): " crlf)
   (assert (programacion (read)))
)

;;;La siguiente pregunta es si le gustan las matematicas. A partir de esta pregunta obtendremos
;;;el hecho matematicas SI/NO.
(defrule pregunta4
(declare (salience 6))
=>
   (printout t "Te gustan las matematicas?(Escribe SI/NO): " crlf)
   (assert (matematicas (read)))
)

;;;Ahora preguntamos si le gusta la Teoria o las practicas, lo que guardara el hecho
;;;teopract TEORIA/PRACTICAS una vez normalizado.
(defrule pregunta5
(declare (salience 5))
=>
   (printout t "Eres mas de teoria o de practicas (Escribe T/P): " crlf)
   (assert (teopract (read)))
)

(defrule teoria_practicas
(declare (salience 5))
?regla <- (teopract T)
=>
    (retract ?regla)
    (assert (teo_pract TEORIA))
)

(defrule teoria_practicas2
(declare (salience 5))
?regla <- (teopract P)
=>
    (retract ?regla)
    (assert (teo_pract PRACTICAS))
)

;;;Por ultimo se pregunta sobre su gusto por la programacion web. Se guardara el hecho
;;;web SI/NO
(defrule pregunta6
(declare (salience 4))
=>
   (printout t "Te gusta lo relacionado con la programacion web?(Escribe SI/NO): " crlf)
   (assert (web (read)))
)

;;;Aqui hacemos una primera recomendacion en base a los hechos que hemos obtenido a partir
;;;de las preguntas, que simplemente es recomendacion y todo lo que hemos leido.
;;;Se guardara el hecho recomendacion y todo lo indicado abajo.
(defrule recomendacion
(declare (salience 2))
(hard_soft ?a)
(not_media_norm ?b)
(programacion ?c)
(matematicas ?d)
(teo_pract ?e)
(web ?f)
=>
   (assert (recomendacion ?a ?b ?c ?d ?e ?f))
)


;;;Con prioridad 1, antes de que la maquina razone y de un consejo, buscamos las incongruencias
;;;en recomendacion, es decir, que tome un valor que no pueda ser. Si esto pasa, se asigna _, que a la hora
;;;de que la maquina razone (despues se observara), actuara como un NO. La unica respuesta que podemos asegurar que estara
;;;bien es la nota media, puesto que nos encargamos de normalizarla y nos aseguramos de que no pueda ser otra cosa que no
;;;sea un numero.
(defrule incongruencias1
(declare (salience 1))
?regla1 <- (recomendacion ?a ?b ?c ?d ?e ?f)
(and (test(neq ?a HARDWARE)) (test (neq ?a SOFTWARE)) (test (neq ?a _)))
=>
    (retract ?regla1)
    (assert (recomendacion _ ?b ?c ?d ?e ?f))
)

(defrule incongruencias2
(declare (salience 1))
?regla1 <- (recomendacion ?a ?b ?c ?d ?e ?f)
(and (test(neq ?e TEORIA)) (test (neq ?e PRACTICAS)) (test (neq ?e _)))
=>
    (retract ?regla1)
    (assert (recomendacion ?a ?b ?c ?d _ ?f))
)

(defrule incongruencias3
(declare (salience 1))
?regla1 <- (recomendacion ?a ?b ?c ?d ?e ?f)
(and (test(neq ?c SI)) (test (neq ?c NO)) (test (neq ?c _)))
=>
    (retract ?regla1)
    (assert (recomendacion ?a ?b _ ?d ?e ?f))
)

(defrule incongruencias4
(declare (salience 1))
?regla1 <- (recomendacion ?a ?b ?c ?d ?e ?f)
(and (test(neq ?d SI)) (test (neq ?d NO)) (test (neq ?d _)))
=>
    (retract ?regla1)
    (assert (recomendacion ?a ?b ?c _ ?e ?f))
)

(defrule incongruencias5
(declare (salience 1))
?regla1 <- (recomendacion ?a ?b ?c ?d ?e ?f)
(and (test(neq ?f SI)) (test (neq ?f NO)) (test (neq ?f _)))
=>
    (retract ?regla1)
    (assert (recomendacion ?a ?b ?c ?d ?e _))
)

;;;A partir de ahora la maquina se centrara en darnos recomendaciones previas, con 1 mencion o 3 menciones
;;;Por construccion, podemos aniadir todas los filtros que se nos vayan ocurriendo, ya que la maquina igualmente
;;;dara un unico consejo por criterios de nota.
;;;Si no le gustan las matematicas ni el hardware podemos descartar directamente
;;;Computadores y CSI
;;;El hecho recomendacion previa tiene la forma recomendacion_previa MENCIONES MOTIVO
(defrule recomendacion_previa
(recomendacion ?a ?b ?c ?d ?e ?f)
(test (eq ?a SOFTWARE))
(test (neq ?d SI))
(Rama Ingenieria_del_Software ?x) (Rama Sistemas_de_Informacion ?y) (Rama Tecnologias_de_la_Informacion ?z)

=>
    (assert (recomendacion_previa ?x ?y ?z "De primeras 3 ramas recomendadas relacionadas con SOFTWARE y poca carga matematica. " ))
)

;;;Si le gusta el hardware podemos recomendar directamente la rama ingenieria de computadores
(defrule recomendacion_previa2
(recomendacion ?a ?b ?c ?d ?e ?f)
(test (eq ?a HARDWARE))
=>
    (assert (recomendacion_previa IC "Recomendada por gustar HARDWARE y ser la unica rama que toque este tema. "))
)

;;;Si le gusta lo relacionado con la web podemos directamente recomendarlo
(defrule recomendacion_previa3
(recomendacion ?a ?b ?c ?d ?e ?f)
(test (eq ?f WEB))
=>
    (assert (recomendacion_previa TI "Por gusar programacion WEB y estar esta rama muy relacionada. "))
)

;;;Si le gustan las matematicas recomendamos directamente CSI
(defrule recomendacion_previa4
(recomendacion ?a ?b ?c ?d ?e ?f)
(test (eq ?d SI))
=>
    (assert (recomendacion_previa CSI "Recomendada porque le gustan las matematicas. "))
)

;;;Si le gusta la programacion y el software recomendamos IS
(defrule recomendacion_previa5
(recomendacion ?a ?b ?c ?d ?e ?f)
(and (test (eq ?c SI)) (test (eq ?a SOFTWARE)))
=>
    (assert (recomendacion_previa IS "Por gustar la programacion y el Software. "))
)

;;;Si le gusta el software y no la web, pero si las matematicas, recomendamos CSI, IS o SI
(defrule recomendacion_previa6
(recomendacion ?a ?b ?c ?d ?e ?f)
(test (eq ?a SOFTWARE))
(test (eq ?d SI))
(test (neq ?f SI))
(Rama Ingenieria_del_Software ?x) (Rama Sistemas_de_Informacion ?y) (Rama Computacion_y_Sistemas_Inteligentes ?z)

=>
    (assert (recomendacion_previa ?x ?y ?z "De primeras 3 ramas recomendadas relacionadas con SOFTWARE y con menos relacion con la web. Carga matematica. "))

)

;;;Como podemos tener recomendaciones de 3, seleccionamos una mencion, que sera la de menor dificultad
(defrule recomendacion_previa7
;(declare (salience -1))
(recomendacion_previa ?x ?y ?z ?texto)
(Dificultad ?x ?a)
(Dificultad ?y ?b)
(Dificultad ?z ?c)

=> ;;;Tenemos un criterio de ordenacion, podriamos tener cualquier otro
    (bind ?maximo ?a)
    (bind ?salida ?x)
    (if (> ?b ?maximo) then
        (bind ?maximo ?b)
        (bind ?salida ?y)
    else
    (if (> ?c ?maximo) then 
        (bind ?maximo ?c)
        (bind ?salida ?z)))
    
    (assert (recomendacion_previa ?salida (str-cat ?texto "Seleccionamos la de menor dificultad del grupo de 3. ")))
)

;;;Cuando tenemos varias recomendaciones previas, seleccionamos las que tienen solo una mencion y
;;;si tenemos nota ALTA asigna una de las tres primeras menciones (segun mi orden de dificultad).
;;;Con nota MEDIA asignamos una de las cuatro primeras y con nota BAJA cualquiera. Esto se hace para asegurar
;;;que los mejores expedientes estan en las menciones mas dificiles.
;;;El hecho consejo tiene la forma Consejo mencion motivo. En mi caso solo podrá ser una mencion.
(defrule varias_recomendaciones_previas
(declare (salience -1))
?regla <- (recomendacion_previa ?a ?texto)
(or (recomendacion_previa ?x ?texto_op) (recomendacion_previa ?y ?z ?t ?texto_op1)) ;;Para comprobar que hay varias
(not_media_norm ?b)
(test (eq ?b ALTA))
(Dificultad ?a ?c)
(test (< ?c 3))
=>
    (assert (Consejo ?a (str-cat ?texto "Se recomienda por tener una nota media ALTA")))
    (halt)
    ;(retract ?regla)
)

(defrule varias_recomendaciones_previas2
(declare (salience -1))
?regla <- (recomendacion_previa ?a ?texto)
(or (recomendacion_previa ?x ?texto_op) (recomendacion_previa ?y ?z ?t ?texto_op1)) ;;Para comprobar que hay varias
(not_media_norm ?b)
(test (eq ?b MEDIA))
(Dificultad ?a ?c)
(test (< ?c 4))
=>
    (assert (Consejo ?a (str-cat ?texto "Se recomienda por tener una nota media MEDIA")))
    (halt)
    ;(retract ?regla)
)

(defrule varias_recomendaciones_previas3
(declare (salience -1))
?regla <- (recomendacion_previa ?a ?texto)
(or (recomendacion_previa ?x ?texto_op) (recomendacion_previa ?y ?z ?t ?texto_op1)) ;;Para comprobar que hay varias
(not_media_norm ?b)
(test (eq ?b BAJA))
(Dificultad ?a ?c)
(test (<= ?c 5))
(test (>= ?c 4))
=>
    (assert (Consejo ?a (str-cat ?texto "Se recomienda por tener una nota media BAJA")))
    (halt)
    ;(retract ?regla)
)

;;;Si solo hay una recomendacion previa, con prioridad -2 puesto que si se hubiera podido dar
;;;un consejo anteriormente se hubiera ejecutado (halt)
(defrule una_recomendacion_previa
(declare (salience -2))
?regla <- (recomendacion_previa ?a ?texto)

=>
    
    (assert (Consejo ?a (str-cat ?texto "Se recomienda por ser la unica rama aconsejada por el sistema. ")))
    (halt)
    ;(retract ?regla)
)

;;;Si no podemos seleccionar por nota lo hacemos saber, es decir, no hay ningun consejo.
(defrule caso_extremo_primero
(declare (salience -2000))
(not (Consejo ?a ?b))
=>
    (printout t "No puedo recomendar nada, hay una discordancia con la nota media, vuelve a introducir los datos: " crlf)
    
)

;;;Si no tenemos ninguna recomendacion previa lo mismo.
(defrule caso_extremo_segundo
(declare (salience -2000)) ;;;Se comprueban al final ambos casos
(not (or (recomendacion_previa ?x ?texto) (recomendacion_previa ?y ?z ?t ?texto2)))
?regla1 <- (recomendacion ?a ?b ?c ?d ?e ?f)
=>
    (printout t "No puedo recomendar nada, pocas preguntas contestadas, se ejecutara de nuevo: " crlf)
)

)

