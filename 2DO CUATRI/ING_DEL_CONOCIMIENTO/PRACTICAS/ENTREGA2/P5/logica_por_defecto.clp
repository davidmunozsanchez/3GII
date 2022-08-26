(deffacts datos
    (ave gorrion) (ave paloma) (ave aguila) (ave pinguino)
    (mamifero vaca) (mamifero perro) (mamifero caballo)
    (vuela pinguino no seguro) )

;Reglas seguras
;Las aves son animales
(defrule aves_son_animales
    (ave ?x)
    =>
        (assert (animal ?x))
        (bind ?expl (str-cat "sabemos que un " ?x " es un animal porque las aves son un tipo de animal"))
        (assert (explicacion animal ?x ?expl)) )

;Los mamiferos son animales
(defrule mamiferos_son_animales
    (mamifero ?x)
    =>
        (assert (animal ?x))
        (bind ?expl (str-cat "sabemos que un " ?x " es un animal porque los mamiferos son un tipo de animal"))
        (assert (explicacion animal ?x ?expl)) )

(defrule insectos_son_animales
    (insecto ?x)
    =>
        (assert (animal ?x))
        (bind ?expl (str-cat "sabemos que un " ?x " es un animal porque los insectos son un tipo de animal"))
        (assert (explicacion animal ?x ?expl)) )
;Casi todas las aves vuelan --> puedo asumir por defecto que las aves vuelan
;Asumimos por defecto
(defrule ave_vuela_por_defecto
    (declare (salience -1))
    (ave ?x)
    =>
        (assert (vuela ?x si por_defecto))
        (bind ?expl (str-cat "asumo que un " ?x " vuela, porque casi todas las aves vuelan"))
        (assert (explicacion vuela ?x ?expl)) )

;Retractamos cuando hay algo en contra
(defrule retracta_vuela_por_defecto
    (declare (salience 1))
    ?f <- (vuela ?x ?r por_defecto)
    (vuela ?x ?s seguro)
    =>
        (retract ?f)
        (bind ?expl (str-cat "retractamos que un " ?x ?r " vuela por defecto, porque sabemos seguro que " ?x ?s "vuela"))
        (assert (explicacion retracta_vuela ?x ?expl))
        (bind ?expl1 (str-cat "asumo que un " ?x " " ?s " vuela porque lo se seguro"))
        (assert (explicacion vuela ?x ?expl1)) )
    

;;Tratamiento de informacion incompleta
(defrule mayor_parte_animales_no_vuelan
    (declare (salience -2))
    (animal ?x)
    (not (vuela ?x ? ?))
    =>
        (assert (vuela ?x no por_defecto))
        (bind ?expl (str-cat "asumo que " ?x " no vuela, porque la mayor parte de los animales no vuelan"))
        (assert (explicacion vuela ?x ?expl)) )

;;Tratamiento de informacion incompleta
;;;Se añaden insectos para el caso en que el usuario introduzca un animal y no diga si es mamifero o ave
;;;se deduzca por defecto que el animal introducido es un insecto y vuela porque la mayor parte de animales son 
;;;insectos voladores
(defrule mayor_parte_de_insectos_vuelan
    (declare (salience -3))
    (animal ?x)
    
    =>
        (assert (insecto ?x))
        (assert (vuela ?x si por_defecto))
        (bind ?expl (str-cat "asumo que " ?x " vuela y es un insecto, porque la mayor parte de los animales son insectos voladores"))
        (assert (explicacion vuela ?x ?expl)) )

;;;EJERCICIO
;;;1ERO pregunto de que animal quiere saber si vuela o no vuela
(defrule pregunta1
    (declare (salience 12)) ;Lo que se hace primero
    =>
        (printout t "De que animal te gustaria saber si vuela o no?" crlf)
        (assert (animal_introducido (read)))
        )

;;;Ahora comprobamos si tenemos ese animal y mostramos la informacion de la que dispongamos en 
;;;caso afirmativo
(defrule conoce_animal
    (declare (salience -11))
    ?regla <- (animal_introducido ?x)
    (animal ?x)
    (explicacion animal ?x ?expl1)
    (explicacion vuela ?x ?expl2)
    =>
        (retract ?regla)
        (printout t ?expl1 crlf)
        (printout t ?expl2 crlf)
        )
;;;2do, en caso de no conocer al animal le preguntamos si es ave o mamifero (esta la opcion de que no lo sepa)
(defrule no_conoce
    (declare (salience -11))
    ?regla <- (animal_introducido ?x)
    (not (animal ?x))
    =>
        (printout t "El animal " ?x " no esta en mi base de conocimiento, es un ave o un mamifero? (ave, mamifero o ns (no se))" crlf)
        (assert (animal_es ?x (read)))
        (retract ?regla)
        )

(defrule aniadir_mamifero
    (declare (salience -11))
    (animal_es ?a ?b)
    (test (eq ?b mamifero))
    => 
        (assert (mamifero ?a)))


(defrule aniadir_ave
    (declare (salience -11))
    (animal_es ?a ?b)
    (test (eq ?b ave))
    => 
        (assert (ave ?a)))

(defrule aniadir_ns
    (declare (salience -11))
    (animal_es ?a ?b)
    (test (eq ?b ns))
    => 
        (assert (animal ?a)))

;;;Se devuelve conocimiento acerca del animal introducido
(defrule conocimiento_nuevo
    (declare (salience -12))
    ?regla <- (animal_es ?x ?y)
    (animal ?x)
    (explicacion animal ?x ?expl1)
    (explicacion vuela ?x ?expl2)
    => 
        (retract ?regla)
        (printout t ?expl1 crlf)
        (printout t ?expl2 crlf))