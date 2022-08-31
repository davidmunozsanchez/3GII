CREATE TABLE Stock(
    Cproducto varchar(7) PRIMARY KEY,
    Cantidad int
);

CREATE TABLE Pedido(
    Cpedido varchar(7) PRIMARY KEY,
    Ccliente varchar(7),
    Fecha_Pedido DATE
);

CREATE TABLE Detalle_Pedido(
    Cpedido REFERENCES Pedido(Cpedido),
    Cproducto REFERENCES Stock(Cproducto),
    Cantidad int,
    PRIMARY KEY (Cpedido,Cproducto)
);

DESCRIBE Stock;
DESCRIBE Pedido;
DESCRIBE Detalle_Pedido;