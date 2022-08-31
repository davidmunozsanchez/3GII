
import java.sql.*;
import java.util.*;

public class App {

    public static void main(String[] args) {
        Scanner capt = new Scanner(System.in);
        Connection conn = null;
        ResultSet rs = null;
        Statement st = null;
        boolean interfaz = true;
        boolean caso1 = true;
        int numero = 0;
        int accion_case1 = 0;
        int accion_case3 = 0;
        Calendar c = Calendar.getInstance(); 
        int Ccliente = -1; 
        int Cpedido = -1;   
        int Cantidad = 1;
        int Cproducto = -1;
        int cantidad_stock = 0;
        int cantidad_cambio = 0;
        String dia = Integer.toString(c.get(Calendar.DATE));
        String mes = Integer.toString(c.get(Calendar.MONTH));
        String annio = Integer.toString(c.get(Calendar.YEAR));
        
        //Establecimiento de la conexion    
        
        // https://docs.oracle.com/javase/8/docs/api/java/sql/package-summary.html#package.description
        // auto java.sql.Driver discovery -- no longer need to load a java.sql.Driver class via Class.forName

        // register JDBC driver, optional since java 1.6
        /*try {
            Class.forName("oracle.jdbc.driver.OracleDriver");
        } catch (ClassNotFoundException e) {
            e.printStackTrace();
        }
        Tambien se ha tomado información de: https://qastack.mx/programming/2225221/closing-database-connections-in-java
        */

        try{
            conn = DriverManager.getConnection("jdbc:oracle:thin:@//oracle0.ugr.es:1521/practbd.oracle0.ugr.es", "x2095374", "x2095374");
            st = conn.createStatement();
            if (conn != null) {
                System.out.println("Connected to the database!");
            } else 
                System.out.println("FailedClientUI c; to make connection!");
            

            System.out.println("SEMINARIO1:");
            conn.setAutoCommit(false);
            do{
                //Menú inicial con las 4 opciones principales
                System.out.println("\nElige alguna de las siguientes acciones: ");
                System.out.println("1. Dar de Alta Nuevo Pedido \n2. Borrado y Nueva Creacion de las tablas \n3. Mostrar el contenido de las tablas de la BD \n4. Salir del programa y cerrar conexion \n");
                numero = capt.nextInt();
                switch (numero) {
                    //Menú de la opción 1 (Dar de alta a un nuevo pedido)
                    case 1:
                        //Meter por pantalla el código del cliente
                        System.out.println("\nDAR DE ALTA UN NUEVO PEDIDO\n\n");
                        System.out.println("Introduce tu DNI sin letra");
                        Ccliente = capt.nextInt();

                        //Sacar el último código de pedido de las tablas para que el pedido actual tenga el siguiente
                        rs = st.executeQuery("SELECT MAX(Cpedido) FROM PEDIDO");
                        while (rs.next())
                            Cpedido = rs.getInt(1);
                        Cpedido++;
                        
                        //Insertar el nuevo pedido en la tabla Pedido y poner un savepoint
                        st.executeQuery("INSERT INTO Pedido (cpedido, ccliente, fecha_pedido) values ('"+ Cpedido +"', '"+ Ccliente +"', '"+dia+"/"+mes+"/"+annio+"')");
                        
                        Savepoint detalle = conn.setSavepoint("detalle");
                        do {
                            System.out.println("Mostrando las acciones posibles: \n");
                            System.out.println("1. Aniadir detalle del producto \n2. Eliminar todos los detalles del producto \n3. Cancelar pedido \n4. Finalizar pedido \n"); 
                            accion_case1 = capt.nextInt();
                            switch(accion_case1) {
                                case 1:
                                    //Añadir nuevas filas a la tabla Detalle_pedido
                                    System.out.println("\n Seleccione el codigo del producto que desee \n");
                                    Cproducto = capt.nextInt(); 
                                    System.out.println("\n Seleccione la cantidad del producto que desee \n");
                                    Cantidad = capt.nextInt();
                                    
                                    System.out.println("\n Meto el codigo y la cantidad \n");
                                     
                                    rs = st.executeQuery("SELECT cantidad FROM Stock WHERE (cproducto = "+Cproducto+")");
                                    while (rs.next()) {
                                        cantidad_stock = rs.getInt(1);
                                    }
                                    //Distintas opciones disponibles dependiendo de la cantidad de productos disponibles en el stock y la cantidad de dicho producto que queremos     
                                    if (cantidad_stock == 0) {
                                        System.out.println("Lo sentimos, pero no hay ningún producto de los pedidos en stock");
                                    }
                                    else if (cantidad_stock >= Cantidad) {
                                        cantidad_cambio = cantidad_stock-Cantidad;
                                        st.executeQuery("INSERT INTO DETALLE_PEDIDO (cpedido, cproducto, cantidad) VALUES ('"+ Cpedido +"', '"+ Cproducto +"', '"+ Cantidad +"' )");
                                        st.executeQuery("UPDATE Stock SET cantidad = '"+cantidad_cambio+" ' WHERE Cproducto = '"+Cproducto+"'");
                                        System.out.println("Cambio realizado");
                                    }
                                    else {
                                        System.out.println("La cantidad que dispone de stock es: " + cantidad_stock +"\n Introduzca de nuevo una cantidad valida:\n");
                                        Cantidad = capt.nextInt();
                                        if (cantidad_stock >= Cantidad) {
                                            st.executeQuery("INSERT INTO DETALLE_PEDIDO (cpedido, cproducto, cantidad) VALUES ('"+ Cpedido +"'"+ Cproducto +"'"+ Cantidad +"' )");
                                            st.executeQuery("UPDATE Stock SET cantidad = 'cantidad' - ' "+Cantidad+" ' WHERE Cproducto = '"+Cproducto+"' ");                                  //detalle = conn.setSavepoint("detalle");
                   
                                            System.out.println("Cambio realizado\n\n");
                                        }
                                    } 

                                    //Salida por pantalla de las tablas
                                    rs = st.executeQuery("\n\nSELECT * FROM Pedido");
                                    System.out.println("CPedido \tCCliente \tFecha_pedido");

                                    while (rs.next()) {
                                        System.out.println(rs.getString(1) + "\t\t" + rs.getString(2) + "\t\t" + rs.getString(3));
                                    }
                                    System.out.println("\n\n");

                                    rs = st.executeQuery("SELECT * FROM Stock");
                                    System.out.println("CProducto \tCantidad");
                                    
                                    while (rs.next()) {
                                        System.out.println(rs.getString(1) + "\t\t" + rs.getString(2));
                                    }
                                    System.out.println("\n\n");

                                    rs = st.executeQuery("SELECT * FROM Detalle_Pedido");
                                    System.out.println("CPedido \tCProducto \tCantidad");

                                    while (rs.next()) {
                                        System.out.println(rs.getString(1) + "\t\t" + rs.getString(2) + "\t\t" + rs.getString(3));
                                    }
                                    System.out.println("\n\n");
                                    
                                    caso1 = true;
                                    break;
                                case 2: 
                                    //Volver al savepoint hecho justo después de meter los datos en la tabla Pedido
                                    conn.rollback(detalle);

                                    //Salida por pantalla de las tablas
                                    rs = st.executeQuery("\n\nSELECT * FROM Pedido");
                                    System.out.println("CPedido \tCCliente \tFecha_pedido");

                                    while (rs.next()) {
                                        System.out.println(rs.getString(1) + "\t\t" + rs.getString(2) + "\t\t" + rs.getString(3));
                                    }
                                    System.out.println("\n\n");

                                    rs = st.executeQuery("SELECT * FROM Stock");
                                    System.out.println("CProducto \tCantidad");
                                    
                                    while (rs.next()) {
                                        System.out.println(rs.getString(1) + "\t\t" + rs.getString(2));
                                    }
                                    System.out.println("\n\n");

                                    rs = st.executeQuery("SELECT * FROM Detalle_Pedido");
                                    System.out.println("CPedido \tCProducto \tCantidad");

                                    while (rs.next()) {
                                        System.out.println(rs.getString(1) + "\t\t" + rs.getString(2) + "\t\t" + rs.getString(3));
                                    }
                                    System.out.println("\n\n");

                                    caso1 = true;
                                    break;
                                case 3: 
                                    //Volver al inicio
                                    conn.rollback();

                                    //Salida por pantalla de las tablas
                                    rs = st.executeQuery("\n\nSELECT * FROM Pedido");
                                    System.out.println("CPedido \tCCliente \tFecha_pedido");

                                    while (rs.next()) {
                                        System.out.println(rs.getString(1) + "\t\t" + rs.getString(2) + "\t\t" + rs.getString(3));
                                    }
                                    System.out.println("\n\n");

                                    rs = st.executeQuery("SELECT * FROM Stock");
                                    System.out.println("CProducto \tCantidad");
                                    
                                    while (rs.next()) {
                                        System.out.println(rs.getString(1) + "\t\t" + rs.getString(2));
                                    }
                                    System.out.println("\n\n");

                                    rs = st.executeQuery("SELECT * FROM Detalle_Pedido");
                                    System.out.println("CPedido \tCProducto \tCantidad");

                                    while (rs.next()) {
                                        System.out.println(rs.getString(1) + "\t\t" + rs.getString(2) + "\t\t" + rs.getString(3));
                                    }
                                    System.out.println("\n\n");

                                    caso1 = false;
                                    break;
                                case 4:
                                    //Guardar todo lo hecho
                                    conn.commit();
                                    caso1 = false;
                                    break;

                                default:
                                    System.out.println("Tienes que introducir un número entre 1 y 4.");
                            }
                        }while(caso1 == true); 
                        break;
                    
                    //Menú de la opción 2 (Borrado y Nueva Creacion de las tablas)    
                    case 2:   
                        System.out.println("Borrado y creacion de nuevas tablas\n");
                        
                        //Borrado de Tablas:
                        st.executeQuery("DROP TABLE Detalle_Pedido");
                        st.executeQuery("DROP TABLE Stock");
                        st.executeQuery("DROP TABLE Pedido");
                        
                        System.out.println("Tablas eliminadas.\n");
                        
                        //Creación de tablas
                        st.executeQuery("CREATE TABLE Stock("
                            + "Cproducto varchar(7) PRIMARY KEY," 
                            + "Cantidad int CHECK(Cantidad>0))"
                        );
                        
                        st.executeQuery("CREATE TABLE Pedido("
                            + "Cpedido varchar(7) PRIMARY KEY,"
                            + "Ccliente varchar(7) NOT NULL,"
                            + "Fecha_Pedido DATE )" //CHECK(Fecha_Pedido>TO_DATE('01/11/2000','dd/mm/yyyy'))
                        );
                        
                        st.executeQuery("CREATE TABLE Detalle_Pedido("
                            + "Cpedido REFERENCES Pedido(Cpedido),"
                            + "Cproducto REFERENCES Stock(Cproducto),"
                            + "Cantidad int CHECK(Cantidad>=0),"
                            + "PRIMARY KEY (Cpedido, Cproducto))"
                        );

                        System.out.println("Tablas creadas.\n");
                        
                        //Insercción de los datos predefinidos en la tabla Stock
                        st.executeQuery("INSERT INTO Stock (Cproducto,Cantidad) VALUES ('1',46)");
                        st.executeQuery("INSERT INTO Stock (Cproducto,Cantidad) VALUES ('2',32)");
                        st.executeQuery("INSERT INTO Stock (Cproducto,Cantidad) VALUES ('3',67)");
                        st.executeQuery("INSERT INTO Stock (Cproducto,Cantidad) VALUES ('4',89)");
                        st.executeQuery("INSERT INTO Stock (Cproducto,Cantidad) VALUES ('5',16)");
                        st.executeQuery("INSERT INTO Stock (Cproducto,Cantidad) VALUES ('6',13)");
                        st.executeQuery("INSERT INTO Stock (Cproducto,Cantidad) VALUES ('7',7)");
                        st.executeQuery("INSERT INTO Stock (Cproducto,Cantidad) VALUES ('8',28)");
                        st.executeQuery("INSERT INTO Stock (Cproducto,Cantidad) VALUES ('9',46)");
                        st.executeQuery("INSERT INTO Stock (Cproducto,Cantidad) VALUES ('10',66)");
                        
                        System.out.println("Datos insertados.\n");

                        //Guardar todo lo hecho
                        conn.commit();
                                 
                    break;
                    
                    //Menú de la opción 3 (Mostrar el contenido de las tablas de la BD)
                    case 3:
                        System.out.println("\nTeclea una tabla a mostrar: \n1. PEDIDO \n2. STOCK \n3. DETALLE DE PEDIDO\n");
                        System.out.println("Introduce el nombre de una tabla: Pedido, Stock o Detalle_Pedido");
                        accion_case3 = capt.nextInt();
                        
                        switch(accion_case3) {
                            case 1:  
                                //Sacar por pantalla todos los datos almacenados de la tabla Pedido
                                rs = st.executeQuery("SELECT * FROM Pedido");
                                System.out.println("CPedido \tCCliente \tFecha_pedido");

                                while (rs.next()) {
                                    System.out.println(rs.getString(1) + "\t\t" + rs.getString(2) + "\t\t" + rs.getString(3));
                                }
                                System.out.println("\n\n");
                                break;
                            case 2:
                                //Sacar por pantalla todos los datos almacenados de la tabla Stock 
                                System.out.println("Stock\n");      
                                rs = st.executeQuery("SELECT * FROM Stock");
                                System.out.println("CProducto \tCantidad");
                                
                                while (rs.next()) {
                                    System.out.println(rs.getString(1) + "\t\t" + rs.getString(2));
                                }
                                System.out.println("\n\n");
                                break;
                            case 3:
                                //Sacar por pantalla todos los datos almacenados de la tabla Detalle-Pedido
                                System.out.println("Detalle-Pedido\n");      
                                rs = st.executeQuery("SELECT * FROM Detalle_Pedido");
                                System.out.println("CPedido \tCProducto \tCantidad");

                                while (rs.next()) {
                                    System.out.println(rs.getString(1) + "\t\t" + rs.getString(2) + "\t\t" + rs.getString(3));
                                }
                                System.out.println("\n\n");
                                break;
                            default:
                                System.out.println("Tienes que introducir un número entre 1 y 3.");
                        }
                        break;
                        
                    //Menú de la opción 4 (Salir del programa y cerrar conexión)
                    case 4:
                        interfaz = false;
                        System.out.println("Gracias por usar nuestro programa.");
                        break;
                    default:
                        System.out.println("Tienes que introducir un número entre 1 y 4.");
                }
                
            }while (interfaz == true);

        //Mensajes de error en caso de que no se pueda conectar a la base de datos
        }catch (SQLException e) {
            System.err.format("SQL State: %s\n%s", e.getSQLState(), e.getMessage());
        }catch (Exception e) {
            e.printStackTrace();
        }finally {
            //Desconexión de la base de datos
            try { rs.close(); } catch (Exception e) { /* ignored */ }
            try { st.close(); } catch (Exception e) { /* ignored */ }
            try { conn.close();  
                if (conn.isClosed()) {
                    System.out.println("Disconnected to the database!");
                } else {
                    //Mensaje de error en caso de que no se pueda desconectar de la base de datos
                    System.out.println("FailedClientUI c; to make disconnection!");
                }
            } catch (Exception e) { /* ignored */ }
        }
    }
}