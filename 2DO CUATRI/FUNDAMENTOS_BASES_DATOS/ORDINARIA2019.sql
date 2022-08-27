--Mostrar los equipos españoles que han excavado todas las zonas
SELECT * FROM EQUIPOS E WHERE E.PAIS = 'España' and NOT EXISTS(
    (SELECT Z.COD_Z FROM ZONA Z)
    MINUS
    (SELECT EX.COD_Z FROM EXCAVA WHERE E.COD_E = EX.COD_E)
)

--Mostrar las zonas que han sido excavadas por un solo equipo

SELECT B.CODZ FROM EXCAVA NATURAL JOIN ZONA B GROUP BY B.COD_E HAVING COUNT(*) = 1;
SELECT CODZ FROM EXCAVA GROUP BY COD_Z HAVING COUNT(DISTINCT COD_E) = 1;


--Encontrar la cantidad de piezas de bronce encontradas por zona y día, mostrando solo aquellas zonas
--en las que la cantidad total diaria supere las 10 piezas.

SELECT COD_Z, FECHA, COUNT(H.COD_P) AS TOTAL_PIEZAS FROM HALLAZGO H, PIEZA P WHERE H.COD_P = P.COD_P AND P.MATERIAL='Bronce' GROUP BY(COD_Z, FECHA) HAVING TOTAL_PIEZAS > 10;