#ifndef COMPORTAMIENTOJUGADOR_H
#define COMPORTAMIENTOJUGADOR_H

#include "comportamientos/comportamiento.hpp"

#include <list>

struct estado {
  int fila;
  int columna;
  int orientacion;
  bool bikini = false; //Para nivel 1
  bool zapatillas = false;
  vector<bool> visitados; //Para nivel 2
  int obj_visitados; //Para nivel 2
  bool operator<(const estado &n) const
	{
		if ((fila > n.fila) or (fila == n.fila and columna > n.columna) or
			(fila == n.fila and columna == n.columna and orientacion > n.orientacion) or
			(fila == n.fila and columna == n.columna and orientacion == n.orientacion and zapatillas > n.zapatillas) or
			(fila == n.fila and columna == n.columna and orientacion == n.orientacion and zapatillas == n.zapatillas and bikini > n.bikini))
			return true;
		else
			return false;
	}
};

class ComportamientoJugador : public Comportamiento {
  public:
    ComportamientoJugador(unsigned int size) : Comportamiento(size) {
      // Inicializar Variables de Estado
      hayPlan = false;
      ultimaAccion = actIDLE;
      bikini = false;
      zapatillas = false;
      fila = -1;
      columna = -1;
      orientacion = -1;
      numero_obj = 3; //NIvel 2


    }
    ComportamientoJugador(std::vector< std::vector< unsigned char> > mapaR) : Comportamiento(mapaR) {
      // Inicializar Variables de Estado
      hayPlan = false;
      ultimaAccion = actIDLE;
      bikini = false;
      zapatillas = false;
      fila = -1;
      columna = -1;
      orientacion = -1;
      numero_obj = 3; //Nivel 2
    }
    ComportamientoJugador(const ComportamientoJugador & comport) : Comportamiento(comport){}
    ~ComportamientoJugador(){}

    Action think(Sensores sensores);
    int interact(Action accion, int valor);
    void VisualizaPlan(const estado &st, const list<Action> &plan);
    ComportamientoJugador * clone(){return new ComportamientoJugador(*this);}

  private:
    // Declarar Variables de Estado
    estado actual;
    list<estado> objetivos;
    list<Action> plan;
    bool hayPlan;
    int fila;
    int columna;
    int orientacion;
    bool bikini;
    bool zapatillas;
    Action ultimaAccion;
    int numero_obj;

    // Métodos privados de la clase
    bool pathFinding(int level, const estado &origen, const list<estado> &destino, list<Action> &plan);
    bool pathFinding_Profundidad(const estado &origen, const estado &destino, list<Action> &plan);

    void PintaPlan(list<Action> plan);
    bool HayObstaculoDelante(estado &st);
    int heuristica(const estado &origen, const estado &destino );
    int calcular_costo(estado &origen, Action accion);
    bool pathFinding_Aestrella(const estado &origen, const estado &destino, list<Action> &plan);
    bool pathFinding_Aestrella(const estado &origen, vector<estado> &destino, list<Action> &plan);
    bool pathFinding_CU (const estado &origen, const estado &destino, list<Action> &plan);
    int mejorHeuristica(const estado& origen, vector<estado> destinos, vector<bool> visitados);
};

#endif
