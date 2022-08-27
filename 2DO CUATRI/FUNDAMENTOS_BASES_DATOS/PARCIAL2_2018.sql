--Mostrar el codigo del pedido junto con su importe total, de aquellos
--pedidos cuyo importe total sea mayor que 1000 e, ordenador por importe total

SELECT PED#, SUM(CANTIDAD*PRECIO) AS TOTAL FROM ARTICULO NATURAL JOIN INCLUYE GROUP BY PED# HAVING TOTAL > 1000 GROUP BY TOTAL DESC;

--Mostrar los nombres de los usuarios que hayan pedidio todos los artículos cuyo precio unitario es < 20 euros
SELECT U.NOMBRE FROM usuario U WHERE NOT EXISTS (
    (SELECT A.REF# FROM ARTICULO A WHERE A.PRECIO < 20)
    MINUS
    (SELECT I.REF# FROM PEDIDO P, INCLUYE I WHERE P.PED# = I.PED# AND P.EMAIL = U.EMAIL)
);

