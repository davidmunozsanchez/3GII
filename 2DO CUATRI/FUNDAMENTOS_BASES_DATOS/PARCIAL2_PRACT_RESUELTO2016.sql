CREATE TABLE REPARA (
    ID VARCHAR2(30) REFERENCES ORDENADOR(ID),
    F_INI DATE,
    NIF VARCHAR2(30) REFERENCES EMPRESA(NIF),
    F_FIN DATE,
    CAUSA LONG,
    COSTO REAL,
    PRIMARY KEY (ID, F_INI),
    CHECK(F_INI <= F_FIN)
)

INSERT INTO REPARA(...) VALUES(.....)


---Listar los nombres de aquellos usuarios a los que le han tenido
--que reparar todos los ordenadores que han tenido asignados en algún momento

SELECT U.NOMBRE FROM USUARIO U WHERE NOT EXISTS (
    (SELECT A.ID FROM ASIGNA A WHERE A.DNI = U.DNI)
    MINUS
    (SELECT R.ID FROM REPARA R WHERE R.ID = A.ID)
)

--Encontrar todos los usuarios que tienen o han tenido asignados al menos dos ordenadores
SELECT NOMBRE, F_NAC FROM USUARIO NATURAL JOIN ASIGNA GROUP BY (NOMBRE, FECHA_NAC) HAVING COUNT(*) >= 2;

--vISTA QUE MUESTRE AQUELLOS ORDENADORES que han tenido
-- que ser reparados más de 5 veces o cuyo costo toal de reparación hasta el momento supere los 500$
--la vista deberá incluir los datos de los ordenadores y se deben ordenar por marca y modelo
create view pisapapeles as
SELECT ID, MARCA, MODELO, TIPO FROM ORDENADOR WHERE ID IN(
(SELECT A1.ID FROM ORDENADOR A1, REPARA A2 WHERE A1.ID = A2.ID GROUP BY A1.ID HAVING SUM(COSTO) > 500) 
UNION
(SELECT R.ID FROM REPARA R GROUP BY(R.ID) HAVING COUNT(*) > 5)) GROUP BY MARCA,MODELO;