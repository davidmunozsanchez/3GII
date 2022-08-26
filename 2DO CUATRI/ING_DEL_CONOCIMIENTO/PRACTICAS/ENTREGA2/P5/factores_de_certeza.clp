

(deffunction combinacion (?fc1 ?fc2)
    (if (and (> ?fc1 0) (> ?fc2 0))
        then
            (bind ?rv (- (+ ?fc1 ?fc2)(* ?fc1 ?fc2)))
        else
            (if (and (< ?fc1 0)(< ?fc2 0))
                then    
                    (bind ?rv (+ (+ ?fc1 ?fc2)(* ?fc1 ?fc2)))
                else
                    (bind ?rv (/ (+ ?fc1 ?fc2)(- 1 (min(abs ?fc1)(abs ?fc2))) )) )
            
            )
            
    ?rv)
(deffunction encadenado (?fc_antecedente ?fc_regla)
    (if (> ?fc_antecedente 0)
        then
            (bind ?rv (* ?fc_antecedente ?fc_regla))
        else
            (bind ?rv 0))
    ?rv)



;;;Hacemos las preguntas sobre las evidencias del estado del coche
(defrule pregunta1
    (declare (salience 9))
    (not (Evidencia hace_intentos_arrancar ?x))
    =>
        (printout t "PREGUNTAS SOBRE CIERTOS ASPECTOS DEL COCHE: "crlf)
        (printout t "El coche hace intentos de arrancar?(si/no)" crlf)
        (assert (Evidencia hace_intentos_arrancar (read)))
        )

(defrule pregunta2
    (declare (salience 9))
    (not (Evidencia hay_gasolina_en_deposito ?y))
    =>
        (printout t "El coche hace tiene gasolina en el deposito?(si/no)" crlf)
        (assert (Evidencia hay_gasolina_en_deposito (read)))
        )
(defrule pregunta3
    (declare (salience 9))
    (not (Evidencia encienden_las_luces ?z)) 
    =>
        (printout t "Se encienden las luces del coche?(si/no)" crlf)
        (assert (Evidencia encienden_las_luces (read)))
        )
(defrule pregunta4
    (declare (salience 9))
    (not (Evidencia gira_motor ?t))
    =>
        (printout t "El motor gira?(si/no)" crlf)
        (assert (Evidencia gira_motor (read)))
        )
;;;convertimos cadad evidencia en una afirmacion sobre su factor de certeza
(defrule certeza_evidencias
    (Evidencia ?e ?r)
    =>
        (assert (FactorCerteza_ev ?e ?r 1)))
    
;;;Traduccion de reglas
;R1: Si el motor obtiene gasolina Y el motor gira ENTONCES problemas
;con las bujías con certeza 0.7
;;Se incluyen las demas reglas propuestas
(defrule R1
    (FactorCerteza_ev motor_llega_gasolina si ?f1)
    (FactorCerteza_ev gira_motor si ?f2)
    (test (and (> ?f1 0) (> ?f2 0)))
    =>
        (assert (FactorCerteza problema_bujias si (encadenado (* ?f1 ?f2) 0.7))))

(defrule R2
    (FactorCerteza_ev gira_motor no ?f2)
    (test (> ?f2 0))
    =>
        (assert (FactorCerteza problema_starter si (encadenado ?f2 0.8))))

(defrule R3
    (FactorCerteza_ev encienden_las_luces no ?f2)
    (test (> ?f2 0))
    =>
        (assert (FactorCerteza problema_bateria si (encadenado ?f2 0.9))))
(defrule R4
    (FactorCerteza_ev hay_gasolina_en_deposito si ?f2)
    (test (> ?f2 0))
    =>
        (assert (FactorCerteza motor_llega_gasolina si (encadenado ?f2 0.9))))

(defrule R5
    (FactorCerteza_ev hace_intentos_arrancar si ?f2)
    (test (> ?f2 0))
    =>
        (assert (FactorCerteza problema_starter si (encadenado ?f2 -0.6))))

(defrule R6
    (FactorCerteza_ev hace_intentos_arrancar si ?f2)
    (test (> ?f2 0))
    =>
        (assert (FactorCerteza problema_bateria si (encadenado ?f2 0.5))))




;;;;Combinar misma deduccion por distintos caminos
(defrule Combinar
    (declare (salience 1))
    ?f <- (FactorCerteza ?h ?r ?fc1)
    ?g <- (FactorCerteza ?h ?r ?fc2)
    (test (neq ?fc1 ?fc2))
    =>
        (retract ?f ?g)
        (assert (FactorCerteza ?h ?r (combinacion ?fc1 ?fc2))) )



;;;Aunque en este ejemplo no se da, puede ocurrir que tengamos
;;;deducciones de hipótesis en positivo y negativo que hay que combinar
;;;para compararlas
(defrule combinar_signo
    (declare (salience 2))
    (FactorCerteza ?h si ?fc1)
    (FactorCerteza ?h no ?fc2)
    =>
        (assert (Certeza ?h (- ?fc1 ?fc2)))
)

;;;Se calcula el maximo con el tipico proceso de partir como maximo de cualquer factor de certeza
;;;y en otra regla se comparan 2 a 2 los factores de certeza quedándonos siempre con el máximo
(defrule mejor_hipotesis1
    (declare (salience -9))
    (FactorCerteza ?a ?b ?fc)
    (not (mejor_hipotesis ?x ?y ?z))
    (test (> ?fc -1))
    =>
        (assert (mejor_hipotesis ?a ?b ?fc)))

(defrule mejor_hipotesis2
    (declare (salience -9))
    ?regla <- (mejor_hipotesis ?x ?y ?fc)
    (FactorCerteza ?a ?b ?fc2)
    (test (< ?fc ?fc2))
    
    =>
        (retract ?regla)
        (assert (mejor_hipotesis ?a ?b ?fc2)))

(defrule imprimir_mejor
    (declare (salience -10))
    (mejor_hipotesis ?a ?b ?c)
    =>
        (printout t "La mejor hipotesis es " ?b " " ?a " dado que tiene un factor de certeza " ?c " , que es el maximo de todas las hipotesis." crlf))
        