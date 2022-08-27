/**
 * Nombre: David   
 * Apellidos: Muñoz Sánchez
 * Grupo: B2
 * DNI: 07256819C     
 *
 *
 * Breve justificación de la solución
 * ----------------------------------
 *
 */

#include <iostream>
#include <iomanip>
#include <random>
#include <vector>
#include <cassert>
#include "scd.h"

using namespace std ;
using namespace scd ;

const int N = 4;        // Número de jugadores.
const int EMPTY = -1;   // Cómo señalamos las casillas vacías.
const int B = 10;       // Número de ataques que hará el enemigo.



//Devuelve un número aleatorio entre 0 y 4
int aleatorio(){
    static default_random_engine generador( (random_device())() );
    static uniform_int_distribution<int> distribucion_uniforme(1, 4);
    return distribucion_uniforme(generador);
}

/**
 * Imprime el tablero por pantalla.
 */
/*void imprime() {
    for(int i = 0; i < tablero.size(); i++){
        for(int j = 0; j < tablero[i].size(); j++){
            if(tablero[i][j] == EMPTY)
                cout << setw(3) << "." << " ";
            else
                cout << setw(3) << tablero[i][j] << " ";

        }
        cout << endl;
    }
}*/

/**
 * Imprime las posiciones de los jugadores.
 */
/*void imprimeJugadores() {
    for(int i = 0; i < pos_jugadores.size(); i++){
        for(int j = 0; j < pos_jugadores[i].size(); j++){
            cout << pos_jugadores[i][j] << " ";
        }
        cout << endl;
    }
}*/

/**
 * Muestra las vidas de cada jugador.
 */
/*void imprimeVidas() {
    cout << " Vidas: "; 
    for(int i = 0; i < vida_jugadores.size(); i++){
        cout << vida_jugadores[i] << " ";
    }
    cout << endl;
}*/

/**
 * Devuelve las vidas que tiene un jugador.
 * @param  j Número del jugador.
 * @return   Número de vidas.
 */
/*int getVidas(int j) {
    return vida_jugadores[j];
}*/

/**
 * Devuelve el número de ataques que han hecho los enemigos.
 * @return Número de ataques.
 */
/*int getBarridos(){
    return barridos;
}*/

/**
 * Cuenta el número de jugadores vivos.
 * @return Número de jugadores vivos.
 */
/*int getJugadoresVivos() {
    int v = 0;
    for(int i = 0; i < N; i++)
        if(vida_jugadores[i] > 0)
            v++;
    return v;
}*/


/* Monitor para representar el tablero de juego*/
class Tablero{
    private:
        vector< vector<int> > tablero(N, vector<int>(N));          // Matriz del tablero.
        vector< vector<int> > pos_jugadores(N, vector<int>(2));    // Indica la posición en la que está cada jugador.
        vector<int> vida_jugadores(N);             // Vector con la vida de los jugadores.
        int barridos = B;                           // Número de ataques que se han producido.
        bool ataque_en_proceso;
        bool movimiento_en_proceso;
        mutex cerrojo_monitor;
        condition_variable cola_jugadores, cola_enemigos;

    public:
        /**
        * Constructor dado el número de jugadores (máximo 4)
        */
        Tablero()
        {
            int i, j;

            // Se inicializa la vida.
            for(i = 0; i < N; i++) 
                vida_jugadores[i] = 3;
    
            // Se inicializa el tablero.
            for(i = 0; i < N; i++)
                for(j = 0; j < N; j++)
                    tablero[i][j] = EMPTY;

            // Inicializamos las posiciones de los jugadores.
            for(i = 0; i < N; i++)
                for(j = 0; j < 2; j++)
                    pos_jugadores[i][j] = EMPTY;

            ataque_en_proceso = false; //al principio no hay ningún ataque
            movimiento_en_proceso = true; //empezamos moviendo
            
        }

        /**
         * Función para mover un jugador a una posición dada
         */
        public bool moverJugador(int i, int j, int num_jugador){
            unique_lock<mutex> guarda( cerrojo_monitor ); //con esto ya seguramos que no se pueden mover dos jugadores a la vez
            bool a_devolver = false;;
            
            cout << "Movemos jugador " <<setw(2) << num_jugador << endl ;
            if(this->ataque_en_proceso || (this->vida_jugadores[num_jugador] != 3 && this->barridos < B)
                cola_jugadores.wait(guarda);
            else
            {
                if (tablero[i][j] == EMPTY){
                    tablero[i][j] = 1; //marcamos que hay un jugador en esa casilla
                    a_devolver = true;
                }

                
            }
            this->movimiento_en_proceso = false;
            cout << "Movimiento jugador " <<setw(2) << num_jugador << " terminado" << endl ;
            return a_devolver;
            
        }

        /**
         * Ataque a los jugadores. Recorre una línea y si encuentra algún jugador, le resta vida.
         * También lo elimina del tablero para no volver a quitarle más vida en el siguiente ataque.
         */
        public void recorrerLinea(int i, bool orientacion){
            unique_lock<mutex> guarda( cerrojo_monitor );
            this->ataque_en_proceso = true;
            
            cout << "Inicio recorrido " << endl ;
            if (this->movimiento_en_proceso && this->barridos < 1){
                cola_enemigos.wait(guarda);
            }
            else
            {   //si orientación true suponemos horizontal
                if(orientacion){
                    
                    for (int j = 0; j < N; j++){
                        if (this->tablero[i][j] != -1){
                            this->vida_jugadores[tablero[i][j]] -= 1;
                        }
                    }
                }

                else{
                    for (int j = 0; j < N; j++){
                        if (this->tablero[j][i] != -1){
                            this->vida_jugadores[tablero[j][i]] -= 1;
                        }
                    }  
                }
            }

            cout << "Final recorrido."
            


        }


};

/**
 * Función que se encarga de ejecutar cada una de las hebras jugadoras.
 * Debe colocar los jugadores en una posición del tablero.
 * Por defecto se pueden colocar todos en una columna distinta en la primera fila.
 */
void funcion_hebra_jugador(Tablero monitor, int num_hebra) { //en nuestro caso num_hebra = num_jugador
    const int ms = aleatorio<5,100>();
    this_thread::sleep_for( chrono::milliseconds(ms) );
    monitor->moverJugador(aleatorio()-1, aleatorio()-1, aleatorio());
    
}

/**
 * Función que se encarga de atacar a los jugadores. 
 * Recorre el campo y si encuentra en alguna línea a un jugador, le resta vida.
 */
void funcion_hebra_enemigo(Tablero monitor, int num_hebra) { //en este caso, num_hebra = num_linea
    const int ms = aleatorio<5,100>();
    this_thread::sleep_for( chrono::milliseconds(ms) );
    monitor->recorrerLinea(num_hebra, true); //lo hacemos siempre en horizontal
    
}

/*****************************
 * Programa principal
 *****************************/
int main(int argc, char const *argv[]) {
    const int num_hebras = 4;
    Tablero monitor();

    thread hebra[num_hebras];

    // crear y lanzar hebras
    thread hebra[num_hebras];
    for( unsigned i = 0 ; i < num_hebras ; i++ )
        hebra[i] = thread( funcion_hebra, &monitor, i );

   // esperar a que terminen las hebras (no pasa nunca)
    for( unsigned i = 0 ; i < num_hebras ; i++ )
        hebra[i].join();
    
    return 0;
}