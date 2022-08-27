#include <iostream>
#include <cassert>
#include <thread>
#include <mutex>
#include <random> // dispositivos, generadores y distribuciones aleatorias
#include <chrono> // duraciones (duration), unidades de tiempo
#include "scd.h"

using namespace std ;
using namespace scd ;

const int num_fumadores = 3;

int producir_ingrediente()
{
   // calcular milisegundos aleatorios de duración de la acción de fumar)
   chrono::milliseconds duracion_produ( aleatorio<10,100>() );

   // informa de que comienza a producir
   cout << "Estanquero : empieza a producir ingrediente (" << duracion_produ.count() << " milisegundos)" << endl;

   // espera bloqueada un tiempo igual a ''duracion_produ' milisegundos
   this_thread::sleep_for( duracion_produ );

   const int num_ingrediente = aleatorio<0,num_fumadores-1>() ;

   // informa de que ha terminado de producir
   cout << "Estanquero : termina de producir ingrediente " << num_ingrediente << endl;

   return num_ingrediente ;
}

void fumar( int num_fumador )
{

   // calcular milisegundos aleatorios de duración de la acción de fumar)
   chrono::milliseconds duracion_fumar( aleatorio<20,200>() );

   // informa de que comienza a fumar

    cout << "Fumador " << num_fumador << "  :"
          << " empieza a fumar (" << duracion_fumar.count() << " milisegundos)" << endl;

   // espera bloqueada un tiempo igual a ''duracion_fumar' milisegundos
   this_thread::sleep_for( duracion_fumar );

   // informa de que ha terminado de fumar

    cout << "Fumador " << num_fumador << "  : termina de fumar, comienza espera de ingrediente." << endl;

}

// Declaramos SU
class Estanco : public HoareMonitor
{
   private:
      int mostrador;
      CondVar estanquero, fumador[3];

   public:
      Estanco()
      {
         this->mostrador = -1;
         this->estanquero = newCondVar();
         for (int i = 0; i < 3; i++)
            this->fumador[i] = newCondVar();
      }

      void ObtenerIngrediente(int i){
         if (mostrador != i)
            fumador[i].wait();

         mostrador = -1;

         cout << "Ingrediente " << i << " recogido." << endl;
         estanquero.signal();
      }

      void PonerIngrediente(int i){
         mostrador = i;
         cout << "Ingrediente " << i << " puesto en el mostrador." << endl;
         fumador[i].signal();
      }

      void EsperarRecogida(){
         if(mostrador != -1)
            estanquero.wait();
      }

};

void funcion_hebra_estanquero(MRef<Estanco> monitor){
   int ingrediente;
   while (true){
      ingrediente = producir_ingrediente();
      monitor->PonerIngrediente(ingrediente);
      monitor->EsperarRecogida();
   }
}

void funcion_hebra_fumador(MRef<Estanco> monitor, int i){
   while (true){
      monitor->ObtenerIngrediente(i);
      fumar(i);
   }
}

//-------------------------------------------------------------------------
// Función que simula la acción de producir un ingrediente, como un retardo
// aleatorio de la hebra (devuelve número de ingrediente producido)



//----------------------------------------------------------------------
// función que ejecuta la hebra del estanquero



//-------------------------------------------------------------------------
// Función que simula la acción de fumar, como un retardo aleatoria de la hebra



//----------------------------------------------------------------------


//----------------------------------------------------------------------

int main()
{
   MRef<Estanco> monitor = Create<Estanco>();
   thread hebra_fumador[3];
   thread hebra_estanco(funcion_hebra_estanquero, monitor);

   for (int i=0; i<num_fumadores; i++){
      hebra_fumador[i] = thread(funcion_hebra_fumador, monitor, i);
   }

   hebra_estanco.join();

   for (int i=0; i<num_fumadores; i++){
      hebra_fumador[i].join();
   }
}
