
;;;; HECHOS GENERALES DEL SISTEMA ;;;;;
;;;;(seran validos para todas las ejecuciones del sistema ;;;;

; Listado de personas de la familia en cuestion introduccidas con la propiedad unaria de hombre o mujer

(deffacts personas
   (hombre AlfonsoXIII) ; "Antonio es un hombre"
   (hombre Alfonso)
   (hombre Jaime)
   (hombre Juan)
   (hombre Alfonso2)
   (hombre Gonzalo)
   (hombre JuanCarlos)
   (hombre Luis) 
   (mujer VictoriaEugenia)       
   (mujer Edelmira)
   (mujer Emanuela)
   (mujer MdelasMercedes)
   (mujer Carmen)
   (mujer Mercedes)
   (mujer Pilar)
   (mujer Sofia) )

;;;;; Plantilla t�pica de Relaciones binarias, ajustada a relaciones de parentesco restringiendo los valores de tipo de relacion a estas. Se usa para registrar "El <sujeto> es <tipo de relacion> de <objeto>", por ejemplo "Juan es TIO de Julia" 

(deftemplate Relacion 
  (slot tipo (type SYMBOL) (allowed-symbols HIJO PADRE ABUELO NIETO HERMANO ESPOSO PRIMO TIO SOBRINO  CUNIADO YERNO SUEGRO))
  (slot sujeto)
  (slot objeto))

;;;;; Datos de la relacion HIJO y ESPOSO en mi familia que es suficiente para el problema, pues el resto se deduce de estas

(deffacts relaciones
   (Relacion (tipo HIJO) (sujeto Alfonso) (objeto AlfonsoXIII)) ; "Luis es HIJO de Antonio"
   (Relacion (tipo HIJO) (sujeto Jaime) (objeto AlfonsoXIII))
   (Relacion (tipo HIJO) (sujeto Juan) (objeto AlfonsoXIII))
   (Relacion (tipo HIJO) (sujeto Alfonso2) (objeto Jaime))
   (Relacion (tipo HIJO) (sujeto Gonzalo) (objeto Jaime))
   (Relacion (tipo HIJO) (sujeto JuanCarlos) (objeto Juan))
   (Relacion (tipo HIJO) (sujeto Pilar) (objeto Juan))
   (Relacion (tipo ESPOSO) (sujeto AlfonsoXIII) (objeto VictoriaEugenia)) ; "Antonio es ESPOSO de Laura"
   (Relacion (tipo ESPOSO) (sujeto Alfonso) (objeto Edelmira)) 
   (Relacion (tipo ESPOSO) (sujeto Jaime) (objeto Emanuela))
   (Relacion (tipo ESPOSO) (sujeto Juan) (objeto MdelasMercedes))
   (Relacion (tipo ESPOSO) (sujeto Alfonso2) (objeto Carmen))
   (Relacion (tipo ESPOSO) (sujeto Alfonso2) (objeto Carmen))
   (Relacion (tipo ESPOSO) (sujeto Gonzalo) (objeto Mercedes))
   (Relacion (tipo ESPOSO) (sujeto Luis) (objeto Pilar))
   (Relacion (tipo ESPOSO) (sujeto JuanCarlos) (objeto Sofia)))

;;;;;;; Cada relacion tiene una relacion dual que se produce al cambiar entre si objeto y sujeto. Por ejejmplo, Si x es HIJO de y, y es PADRE de x". Para poder deducirlo con una sola regla metemos esa informacion como hechos con la etiqueta dual, "Dual de HIJO PADRE", y asi con todas las relaciones consideradas
 
(deffacts duales
(dual HIJO PADRE) (dual ABUELO NIETO) (dual HERMANO HERMANO) 
(dual ESPOSO ESPOSO) 
(dual PRIMO PRIMO) (dual TIO SOBRINO) 
(dual CUNIADO CUNIADO) 
(dual YERNO SUEGRO))

;;;;;; Para deducir las reglas que se aplican son de composicion, del tipo "el HERMANO del PADRE es un TIO". Por comodidad, en lugar de crear una regla por cada posible composici�n, metemos como hechos la relacion que se obtiene por composicion. Solo metemos unas cuantas composiciones que sean suficientes para deducir cualquier cosa

(deffacts compuestos
(comp HIJO HIJO NIETO) (comp PADRE PADRE ABUELO) (comp ESPOSO PADRE PADRE)(comp HERMANO PADRE TIO) (comp HERMANO ESPOSO CUNIADO) (comp ESPOSO HIJO YERNO) (comp ESPOSO HERMANO CUNIADO) (comp HIJO PADRE HERMANO) (comp ESPOSO CUNIADO CUNIADO) (comp ESPOSO TIO TIO)  (comp HIJO TIO PRIMO)  ) 


;;;;;; Para que cuando digamos por pantalla el parentesco lo espresemos correctamente, y puesto que el nombre que hemos puesto a cada relacion es el caso masculino, vamos a meter como hechos como se diaria esa relacion en femenino mediante la etiqueta femenino

(deffacts femenino
(femenino HIJO HIJA) (femenino PADRE MADRE) (femenino ABUELO ABUELA) (femenino NIETO NIETA) (femenino HERMANO HERMANA) (femenino ESPOSO ESPOSA) (femenino PRIMO PRIMA) (femenino TIO TIA) (femenino SOBRINO SOBRINA) (femenino CUNIADO CUNIADA) (femenino YERNO NUERA) (femenino SUEGRO SUEGRA)) 

(deffacts plural 
(plural HIJOS HIJO)(plural ABUELOS ABUELO)(plural HERMANOS HERMANO)(plural PRIMOS PRIMO)(plural TIOS TIO)
(plural PADRES PADRE)(plural NIETOS NIETO)(plural SOBRINOS SOBRINO)(plural ESPOSOS ESPOSO)
(plural CUNIADOS CUNIADO)(plural YERNOS YERNO)(plural SUEGROS SUEGRO))

;;;;; REGLAS DEL SISTEMA ;;;;;

;;;; La dualidad es simetrica: si r es dual de t, t es dual de r. Por eso solo metimos como hecho la dualidad en un sentidos, pues en el otro lo podiamos deducir con esta regla

(defrule autodualidad
      (dual ?r ?t)
=> 
   (assert (dual ?t ?r)))


;;;; Si  x es R de y, entonces y es dualdeR de x

(defrule dualidad
   (Relacion (tipo ?r) (sujeto ?x) (objeto ?y))
   (dual ?r ?t)
=> 
   (assert (Relacion (tipo ?t) (sujeto ?y) (objeto ?x))))


;;;; Si  y es R de x, y x es T de z entonces y es RoT de z
;;;; a�adimos que z e y sean distintos para evitar que uno resulte hermano de si mismo y cosas asi.

(defrule composicion
   (Relacion (tipo ?r) (sujeto ?y) (objeto ?x))
   (Relacion (tipo ?t) (sujeto ?x) (objeto ?z))
   (comp ?r ?t ?u)
   (test (neq ?y ?z))
=> 
   (assert (Relacion (tipo ?u) (sujeto ?y) (objeto ?z))))

;;;;; Como puede deducir que tu hermano es tu cu�ado al ser el esposo de tu cu�ada, eliminamos los cu�ados que sean hermanos

(defrule limpiacuniados
    (Relacion (tipo HERMANO) (sujeto ?x) (objeto ?y))
    ?f <- (Relacion (tipo CUNIADO) (sujeto ?x) (objeto ?y))
=>
	(retract ?f) )

;;;;; Solicitamos el nombre de la primera persona sobre el que se desea informacion y guardamos y a�adimos ese hecho 
 
(defrule pregunta
(declare (salience 1000)) 
=>
   (printout t "Dime el nombre de la persona de la Casa de Borbon sobre la que quieres informacion (escribe solo el nombre): " crlf)
   (assert (primerapersona (read))))
   
   ;;;;; Solicitamos el nombre de la segunda persona 
 
(defrule pregunta2
(declare (salience 100))
(primerapersona ?persona)
=>
   (printout t "Dime el nombre de la relacion de "?persona" en la Casa de Borbón de la cual quieres saber que miembros la componen (escribe relacion en MAYUSCULAS): " crlf)
   (assert (relacion_pers (read))))

;;;;; Hacemos que nos diga por pantalla la relacion entre las persona introducida. Como la forma de expresarlo dependera del sexo, usamos dos reglas, una para cada sexo

(defrule relacionmasculino
   (primerapersona ?x)		
   (relacion_pers ?r)
   (plural ?r ?t)
   (Relacion (tipo ?t) (sujeto ?y) (objeto ?x))
   (hombre ?y)
 =>
   (printout t ?y " es " ?t " de " ?x crlf) )

(defrule relacionfemenino
   (primerapersona ?x)		
   (relacion_pers ?r)
   (plural ?r ?t)
   (Relacion (tipo ?t) (sujeto ?y) (objeto ?x))
   (mujer ?y)
   (femenino ?t ?f)
 =>
   (printout t ?y " es " ?f " de " ?x crlf) )


(defrule ningunarelacion          ;;;;;;Lo ejecutamos lo último para que compruebe todas las relaciones
(declare (salience -1))
   (primerapersona ?x)		
   (relacion_pers ?r)
   (plural ?r ?t)
   (not(Relacion (tipo ?t) (sujeto ?y) (objeto ?x)))
 =>
   (printout t ?x " no tiene ningun " ?t crlf) )
