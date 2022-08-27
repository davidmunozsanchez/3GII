CREATE TABLE BOT (
    EMAIL VARCHAR2(30) PRIMARY KEY REFERENCES PARTICIPANTE(EMAIL),
    NOMBREBOT VARCHAR2(30) NOT NULL UNIQUE,
    CPP LONG,
    H LONG,
);

CREATE TABLE PARTIDO(
    FECHA DATE,
    GRUPO CHAR(2),
    JLOCAL VARCHAR2(30) REFERENCES BOT(EMAIL),
    JVISIT VARCHAR2(30) REFERENCES BOT(EMAIL),
    GLocal NUMERIC NOT NULL CHECK(GLocal IN (0,1)),
    GVisit NUMERIC NOT NULL CHECK (GVisit IN (0,1)),
    EMPATE NUMERIC NOT NULL CHECK (EMPATE IN (0,1)),
    PRIMARY KEY (FECHA, GRUPO, JLOCAL, JVISIT),
    FOREING KEY (FECHA, GRUPO) REFERENCES LIGA(FECHA,GRUPO),
    CHECK (JLOCAL <> JVISIT),
    CHECK (GLocal+GVisit+EMPATE = 1)

);


--participantes del grupo A1 cuyos bots han jugado algún partido como local en todas las ligas de su grupo
FROM PARTICIPANTE, BOT, PARTIDO, LIGA WHERE PARTICIPANTE.EMAIL = BOT.EMAIL AND BOT.EMAIL = PARTIDO.JLOCAL
                                            AND GRUPO = 'A1' AND PARTIDO.GRUPO

--MOSTRAR nombre y puntuacion de aquello bots que jugando como local en la liga del 3 de junio de 2018 obtuvieron un total de puntos superior a 4. La puntuacion jugando como local se calcula como 3*partidosganados como local + partidos empatados
SELECT B.NOMBREBOT, SUM(3*B.GLocal + B.EMPATE) AS PUNTUACION_TOTAL FROM
    (SELECT A1.NOMBREBOT, A2.FECHA, A2.GRUPO, A2.JLOCAL, A2.JVISIT, A2.GLOCAL, A2.GVISIT, A2.EMPATE FROM BOT A1, PARTIDO A2 WHERE A1.EMAIL = A2.JLOCAL) B WHERE TO_CHAR(B.FECHA, 'DD/MM/AAAA') = '03/06/2018' GROUP BY NOMBREBOT HAVING PUNTUACION_TOTAL > 4;

--MOSTRAR los emails de los participantes que hayan ganado al bot de nombre 'GreedyBot' como local y como visitante
