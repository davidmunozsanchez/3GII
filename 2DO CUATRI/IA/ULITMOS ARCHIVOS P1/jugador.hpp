#ifndef COMPORTAMIENTOJUGADOR_H
#define COMPORTAMIENTOJUGADOR_H
#include <vector> //Incluyo la biblio para trabajar con vectores
#include "comportamientos/comportamiento.hpp"
using namespace std;

class ComportamientoJugador : public Comportamiento{

  public:
    ComportamientoJugador(unsigned int size) : Comportamiento(size){
      // Constructor de la clase
      // Dar el valor inicial a las variables de estado
      fil = col = 99;
      brujula = 0;
      ultimaAccion =  actIDLE; //No hizo nada en la acción anterior por defecto
      girar_derecha = false;
      bien_situado = false;
      primer_paso = true;
      bikini = false;
      zapatillas = false;
      contador_giros = 0;
    }

    ComportamientoJugador(const ComportamientoJugador & comport) : Comportamiento(comport){}
    ~ComportamientoJugador(){}

    Action think(Sensores sensores);
    int interact(Action accion, int valor);
    

  private:
  
  // Declarar aquí las variables de estado
  int fil, col, brujula; //TUTORIAL 16MAR22
  Action ultimaAccion;
  bool girar_derecha;
  bool bien_situado;
  bool zonaPrometedoraPos(Sensores sensores);
  void copiarResultado(Sensores sensores);
  Action giro_aleatorio();
  bool primer_paso; //si nivel 0, estamos bien situados
  bool bikini;
  bool zapatillas;
  int contador_giros;
  std::vector<std::vector<char>> mapa_auxiliar;
  int fil_aux, col_aux; //tamanio de la matriz auxiliar

};

#endif
