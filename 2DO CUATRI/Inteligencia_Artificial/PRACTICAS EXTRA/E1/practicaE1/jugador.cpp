#include "../Comportamientos_Jugador/jugador.hpp"
#include "motorlib/util.h"

#include <iostream>
#include <cmath>
#include <set>
#include <stack>
#include <queue>

// Este es el método principal que se piden en la practica.
// Tiene como entrada la información de los sensores y devuelve la acción a realizar.
// Para ver los distintos sensores mirar fichero "comportamiento.hpp"
Action ComportamientoJugador::think(Sensores sensores)
{
	Action accion = actIDLE;

	actual.fila = sensores.posF;
	actual.columna = sensores.posC;
	actual.orientacion = sensores.sentido;

	cout << "Fila: " << actual.fila << endl;
	cout << "Col : " << actual.columna << endl;
	cout << "Ori : " << actual.orientacion << endl;

	// Capturo los destinos
	cout << "sensores.num_destinos : " << sensores.num_destinos << endl;
	objetivos.clear();

	if (sensores.terreno[0] == 'K')
	{
		this->bikini = true;
		this->zapatillas = false;
	}
	else if (sensores.terreno[0] == 'D')
	{
		this->bikini = false;
		this->zapatillas = true;
	}

	for (int i = 0; i < sensores.num_destinos; i++)
	{
		estado aux;
		aux.fila = sensores.destino[2 * i];
		aux.columna = sensores.destino[2 * i + 1];
		objetivos.push_back(aux);
	}
	if (!hayPlan)
		hayPlan = pathFinding(sensores.nivel, actual, objetivos, plan);

	if (hayPlan and plan.size() > 0)
	{
		accion = plan.front();
		plan.erase(plan.begin());
	}
	else
	{
		cout << "No se pudo encontrar un plan\n";
	}
	return accion;
}

// Llama al algoritmo de busqueda que se usara en cada comportamiento del agente
// Level representa el comportamiento en el que fue iniciado el agente.
bool ComportamientoJugador::pathFinding(int level, const estado &origen, const list<estado> &destino, list<Action> &plan)
{
	estado un_objetivo;
	vector<estado> varios_objetivos;
	switch (level)
	{
	case 0:
		cout << "Demo\n";

		un_objetivo = objetivos.front();
		cout << "fila: " << un_objetivo.fila << " col:" << un_objetivo.columna << endl;
		return pathFinding_Profundidad(origen, un_objetivo, plan);
		break;

	case 1:
		cout << "Optimo en coste\n";
		// Incluir aqui la llamada al busqueda A*
		un_objetivo = objetivos.front();
		cout << "fila: " << un_objetivo.fila << " col:" << un_objetivo.columna << endl;
		return pathFinding_Aestrella(origen, un_objetivo, plan);

		break;
	case 2:
		cout << "Optimo en coste (3 objetivos)\n";
		// Incluir aqui la llamada al busqueda de costo uniforme/A*
		for (int i = 0; i < 3; i++)
		{
			varios_objetivos.push_back(objetivos.front());
			objetivos.pop_front();
		}
		cout << "tam " << varios_objetivos.size();
		return pathFinding_Aestrella(origen, varios_objetivos, plan);
		break;
	case 3:
		cout << "Reto 1: Descubrir el mapa\n";
		// Incluir aqui la llamada al algoritmo de busqueda para el Reto 1
		cout << "No implementado aun\n";
		break;
	case 4:
		cout << "Reto 2: Maximizar objetivos\n";
		// Incluir aqui la llamada al algoritmo de busqueda para el Reto 2
		cout << "No implementado aun\n";
		break;
	}
	return false;
}

//---------------------- Implementación de la busqueda en profundidad ---------------------------

// Dado el codigo en caracter de una casilla del mapa dice si se puede
// pasar por ella sin riegos de morir o chocar.
bool EsObstaculo(unsigned char casilla)
{
	if (casilla == 'P' or casilla == 'M')
		return true;
	else
		return false;
}

// Comprueba si la casilla que hay delante es un obstaculo. Si es un
// obstaculo devuelve true. Si no es un obstaculo, devuelve false y
// modifica st con la posición de la casilla del avance.
bool ComportamientoJugador::HayObstaculoDelante(estado &st)
{
	int fil = st.fila, col = st.columna;

	// calculo cual es la casilla de delante del agente
	switch (st.orientacion)
	{
	case 0:
		fil--;
		break;
	case 1:
		fil--;
		col++;
		break;
	case 2:
		col++;
		break;
	case 3:
		fil++;
		col++;
		break;
	case 4:
		fil++;
		break;
	case 5:
		fil++;
		col--;
		break;
	case 6:
		col--;
		break;
	case 7:
		fil--;
		col--;
		break;
	}

	// Compruebo que no me salgo fuera del rango del mapa
	if (fil < 0 or fil >= mapaResultado.size())
		return true;
	if (col < 0 or col >= mapaResultado[0].size())
		return true;

	// Miro si en esa casilla hay un obstaculo infranqueable
	if (!EsObstaculo(mapaResultado[fil][col]))
	{
		// No hay obstaculo, actualizo el parametro st poniendo la casilla de delante.
		st.fila = fil;
		st.columna = col;
		return false;
	}
	else
	{
		return true;
	}
}

struct nodo
{
	estado st;
	list<Action> secuencia;
};

struct ComparaEstados
{
	bool operator()(const estado &a, const estado &n) const
	{
		if ((a.fila > n.fila) or (a.fila == n.fila and a.columna > n.columna) or
			(a.fila == n.fila and a.columna == n.columna and a.orientacion > n.orientacion))
			return true;
		else
			return false;
	}
};

// Implementación de la busqueda en profundidad.
// Entran los puntos origen y destino y devuelve la
// secuencia de acciones en plan, una lista de acciones.
bool ComportamientoJugador::pathFinding_Profundidad(const estado &origen, const estado &destino, list<Action> &plan)
{
	// Borro la lista
	cout << "Calculando plan\n";
	plan.clear();
	set<estado, ComparaEstados> Cerrados; // Lista de Cerrados
	stack<nodo> Abiertos;				  // Lista de Abiertos

	nodo current;
	current.st = origen;
	current.secuencia.empty();

	Abiertos.push(current);

	while (!Abiertos.empty() and (current.st.fila != destino.fila or current.st.columna != destino.columna))
	{

		Abiertos.pop();
		Cerrados.insert(current.st);

		// Generar descendiente de girar a la derecha 135 grados
		nodo hijoTurnR = current;
		hijoTurnR.st.orientacion = (hijoTurnR.st.orientacion + 3) % 8;
		if (Cerrados.find(hijoTurnR.st) == Cerrados.end())
		{
			hijoTurnR.secuencia.push_back(actOVERTURN_R);
			Abiertos.push(hijoTurnR);
		}

		// Generar descendiente de girar a la derecha 45 grados
		nodo hijoSEMITurnR = current;
		hijoSEMITurnR.st.orientacion = (hijoSEMITurnR.st.orientacion + 1) % 8;
		if (Cerrados.find(hijoSEMITurnR.st) == Cerrados.end())
		{
			hijoSEMITurnR.secuencia.push_back(actSEMITURN_R);
			Abiertos.push(hijoSEMITurnR);
		}

		// Generar descendiente de girar a la izquierda 45 grados
		nodo hijoSEMITurnL = current;
		hijoSEMITurnL.st.orientacion = (hijoSEMITurnL.st.orientacion + 7) % 8;
		if (Cerrados.find(hijoSEMITurnL.st) == Cerrados.end())
		{
			hijoSEMITurnL.secuencia.push_back(actSEMITURN_L);
			Abiertos.push(hijoSEMITurnL);
		}

		// Generar descendiente de girar a la izquierda 135 grados
		nodo hijoTurnL = current;
		hijoTurnL.st.orientacion = (hijoTurnL.st.orientacion + 5) % 8;
		if (Cerrados.find(hijoTurnL.st) == Cerrados.end())
		{
			hijoTurnL.secuencia.push_back(actOVERTURN_L);
			Abiertos.push(hijoTurnL);
		}

		// Generar descendiente de avanzar
		nodo hijoForward = current;
		if (!HayObstaculoDelante(hijoForward.st))
		{
			if (Cerrados.find(hijoForward.st) == Cerrados.end())
			{
				hijoForward.secuencia.push_back(actFORWARD);
				Abiertos.push(hijoForward);
			}
		}

		// Tomo el siguiente valor de la Abiertos
		if (!Abiertos.empty())
		{
			current = Abiertos.top();
		}
	}

	cout << "Terminada la busqueda\n";

	if (current.st.fila == destino.fila and current.st.columna == destino.columna)
	{
		cout << "Cargando el plan\n";
		plan = current.secuencia;
		cout << "Longitud del plan: " << plan.size() << endl;
		PintaPlan(plan);
		// ver el plan en el mapa
		VisualizaPlan(origen, plan);
		return true;
	}
	else
	{
		cout << "No encontrado plan\n";
	}

	return false;
}

// Sacar por la consola la secuencia del plan obtenido
void ComportamientoJugador::PintaPlan(list<Action> plan)
{
	bool error = false;
	auto it = plan.begin();
	while (it != plan.end() and !error)
	{
		if (*it == actTURN_R or *it == actTURN_L)
		{
			cout << "Las acciones actTURN_R y actTURN_L no son acciones válidas para el agente para esta práctica: REVISA TU CÓDIGO\n";
			error = true;
		}
		else if (*it == actFORWARD)
		{
			cout << "A ";
		}
		else if (*it == actOVERTURN_R)
		{
			cout << "D ";
		}
		else if (*it == actSEMITURN_R)
		{
			cout << "d ";
		}
		else if (*it == actOVERTURN_L)
		{
			cout << "I ";
		}
		else if (*it == actSEMITURN_L)
		{
			cout << "i ";
		}
		else
		{
			cout << "- ";
		}
		it++;
	}
	cout << endl;
}

// Funcion auxiliar para poner a 0 todas las casillas de una matriz
void AnularMatriz(vector<vector<unsigned char>> &m)
{
	for (int i = 0; i < m[0].size(); i++)
	{
		for (int j = 0; j < m.size(); j++)
		{
			m[i][j] = 0;
		}
	}
}

// Pinta sobre el mapa del juego el plan obtenido
void ComportamientoJugador::VisualizaPlan(const estado &st, const list<Action> &plan)
{
	AnularMatriz(mapaConPlan);
	estado cst = st;
	bool error = false;

	auto it = plan.begin();
	while (it != plan.end() and !error)
	{
		if (*it == actTURN_R or *it == actTURN_L)
		{
			error = true;
		}
		else if (*it == actFORWARD)
		{
			switch (cst.orientacion)
			{
			case 0:
				cst.fila--;
				break;
			case 1:
				cst.fila--;
				cst.columna++;
				break;
			case 2:
				cst.columna++;
				break;
			case 3:
				cst.fila++;
				cst.columna++;
				break;
			case 4:
				cst.fila++;
				break;
			case 5:
				cst.fila++;
				cst.columna--;
				break;
			case 6:
				cst.columna--;
				break;
			case 7:
				cst.fila--;
				cst.columna--;
				break;
			}

			mapaConPlan[cst.fila][cst.columna] = 1;
		}
		else if (*it == actOVERTURN_R)
		{
			cst.orientacion = (cst.orientacion + 3) % 8;
		}
		else if (*it == actSEMITURN_R)
		{
			cst.orientacion = (cst.orientacion + 1) % 8;
		}
		else if (*it == actOVERTURN_L)
		{
			cst.orientacion = (cst.orientacion + 5) % 8;
		}
		else if (*it == actSEMITURN_L)
		{
			cst.orientacion = (cst.orientacion + 7) % 8;
		}
		it++;
	}
	if (error)
	{
		AnularMatriz(mapaConPlan);
	}
}

int ComportamientoJugador::interact(Action accion, int valor)
{
	return false;
}

// Nivel 1
int ComportamientoJugador::calcular_costo(estado &origen, Action accion)
{
	int fila = origen.fila;
	int columna = origen.columna;
	int costo = 0;

	if (mapaResultado[fila][columna] == 'A')
	{
		if (accion == actFORWARD)
		{
			if (origen.bikini)
			{
				costo = 15;
			}
			else
			{
				costo = 100;
			}
		}
		else if (accion == actOVERTURN_L or accion == actOVERTURN_R)
		{
			if (origen.bikini)
			{
				costo = 10;
			}
			else
			{
				costo = 200;
			}
		}
		else if (accion == actSEMITURN_R or accion == actSEMITURN_L)
		{
			if (origen.bikini)
			{
				costo = 5;
			}
			else
			{
				costo = 75;
			}
		}
		else if (accion == actWHEREIS)
		{
			costo = 500;
		}
		else if (accion == actIDLE)
		{
			costo = 0;
		}
	}
	else if (mapaResultado[fila][columna] == 'B')
	{
		if (accion == actFORWARD)
		{
			if (origen.zapatillas)
			{
				costo = 10;
			}
			else
			{
				costo = 200;
			}
		}
		else if (accion == actOVERTURN_R or accion == actSEMITURN_L)
		{
			if (origen.zapatillas)
			{
				costo = 6;
			}
			else
			{
				costo = 350;
			}
		}
		else if (accion == actSEMITURN_R or accion == actSEMITURN_L)
		{
			if (origen.zapatillas)
			{
				costo = 3;
			}
			else
			{
				costo = 150;
			}
		}
		else if (accion == actWHEREIS)
		{
			costo = 500;
		}
		else if (accion == actIDLE)
		{
			costo = 0;
		}
	}
	else if (mapaResultado[fila][columna] == 'T')
	{
		if (accion != actWHEREIS and accion != actIDLE)
		{
			if (origen.zapatillas)
			{
				costo = 1;
			}
			else
			{
				costo = 3;
			}
		}
		else if (accion == actWHEREIS)
		{
			costo = 10;
		}

		else if (accion == actIDLE)
		{
			costo = 0;
		}
	}
	else
	{
		if (accion != actWHEREIS and accion != actIDLE)
		{
			costo = 1;
		}
		else if (accion == actWHEREIS)
		{
			costo = 500;
		}

		else if (accion == actIDLE)
			costo = 0;
	}

	return costo;
}

/*HeurÃ­stica a seguir: distancia mÃ­nima entre dos estados concretos
 */
int ComportamientoJugador::heuristica(const estado &origen, const estado &destino)
{
	/*int fil = 0, col = 0;

	if (origen.columna >= destino.columna)
	{
		col = origen.columna - destino.columna;
	}
	else
	{
		col = destino.columna - origen.columna;
	}

	if (origen.fila >= destino.fila)
	{
		fil = origen.fila - destino.fila;
	}
	else
	{
		fil = destino.fila - origen.fila;
	}

	return (fil + col);*/
	return max(abs(origen.fila - destino.fila),abs(origen.columna - destino.columna));
}

/*Tipo de nodo con los atributos esenciales para poder realizar el Algoritmo A* */
struct nodo_estrella
{
	estado st;
	list<Action> secuencia;
	int g; // coste de lo recorrido
	int h; // heuristica
	int f; // suma g y h

	bool operator<(const nodo_estrella &n) const
	{
		return this->f > n.f;
	}
};

/*Struct que nos permite la comparaciÃ³n de nodos estrella*/

struct comparaEstadosEstrella
{
	bool operator()(const nodo_estrella &a, const nodo_estrella &n) const
	{
		if ((a.st.fila > n.st.fila) or (a.st.fila == n.st.fila and a.st.columna > n.st.columna) or
			(a.st.fila == n.st.fila and a.st.columna == n.st.columna and a.st.orientacion > n.st.orientacion) or
			(a.st.fila == n.st.fila and a.st.columna == n.st.columna and a.st.orientacion == n.st.orientacion and a.st.bikini > n.st.bikini) or
			(a.st.fila == n.st.fila and a.st.columna == n.st.columna and a.st.orientacion == n.st.orientacion and a.st.bikini == n.st.bikini and a.st.zapatillas > n.st.zapatillas))
			return true;
		else
			return false;
	}
};

/*FunciÃ³n para calcular el costo de la acciÃ³n llevada a cabo desde un estado concreo*/

/*Algoritmo Aestrella */
bool ComportamientoJugador::pathFinding_Aestrella(const estado &origen, const estado &destino, list<Action> &plan)
{
	// Borro la lista
	cout << "Calculando plan\n";
	plan.clear();

	set<nodo_estrella, comparaEstadosEstrella> Cerrados; // Lista de Cerrados
	priority_queue<nodo_estrella> Abiertos;				 // Lista de Abiertos

	nodo_estrella current;
	current.st = origen;
	current.secuencia.empty();
	current.g = 0;
	current.h = heuristica(current.st, destino);
	current.f = current.g + current.h;

	current.st.bikini = bikini;
	current.st.zapatillas = zapatillas;

	Abiertos.push(current);
	if (current.st.fila == destino.fila && current.st.columna == destino.columna)
	{ // En caso de que el objetivo seleccionado sea el estado en el que nos encontramos
		if (!Abiertos.empty())
		{
			Abiertos.pop();
		}
		current.secuencia.push_back(actIDLE);
		Cerrados.insert(current);
	}
	while (!Abiertos.empty() and (current.st.fila != destino.fila or current.st.columna != destino.columna))
	{

		Abiertos.pop();
		Cerrados.insert(current);

		if (mapaResultado[current.st.fila][current.st.columna] == 'K')
		{
			current.st.bikini = true;
			current.st.zapatillas = false;
		}

		if (mapaResultado[current.st.fila][current.st.columna] == 'D')
		{
			current.st.zapatillas = true;
			current.st.bikini = false;
		}

		// set<nodo_estrella, ComparaNodos>::iterator it;

		// Generar descendiente de overturnR
		nodo_estrella hijoOverTurnR = current;
		hijoOverTurnR.st.orientacion = (hijoOverTurnR.st.orientacion + 3) % 8;
		hijoOverTurnR.g += calcular_costo(hijoOverTurnR.st, actOVERTURN_R);
		hijoOverTurnR.h = heuristica(hijoOverTurnR.st, destino);
		hijoOverTurnR.f = hijoOverTurnR.g + hijoOverTurnR.h;
		hijoOverTurnR.secuencia.push_back(actOVERTURN_R);
		// it = Cerrados.find(hijoTurnR);
		if (Cerrados.find(hijoOverTurnR) == Cerrados.end())
		{

			Abiertos.push(hijoOverTurnR);
		}
		else if (hijoOverTurnR < *Cerrados.find(hijoOverTurnR))
		{
			Abiertos.push(hijoOverTurnR);
		}

		// Generar descendiente de girar a la derecha 45 grados
		nodo_estrella hijoSEMITurnR = current;
		hijoSEMITurnR.st.orientacion = (hijoSEMITurnR.st.orientacion + 1) % 8;
		hijoSEMITurnR.g += calcular_costo(hijoSEMITurnR.st, actSEMITURN_R);
		hijoSEMITurnR.h = heuristica(hijoSEMITurnR.st, destino);
		hijoSEMITurnR.f = hijoSEMITurnR.g + hijoSEMITurnR.h;
		hijoSEMITurnR.secuencia.push_back(actSEMITURN_R);
		// it = Cerrados.find(hijoSEMITurnR);
		if (Cerrados.find(hijoSEMITurnR) == Cerrados.end())
		{

			Abiertos.push(hijoSEMITurnR);
		}
		else if (hijoSEMITurnR < *Cerrados.find(hijoSEMITurnR))
		{
			Abiertos.push(hijoSEMITurnR);
		}

		// Generar descendiente de girar a la izquierda 90 grados
		nodo_estrella hijoOverTurnL = current;
		hijoOverTurnL.st.orientacion = (hijoOverTurnL.st.orientacion + 5) % 8;
		hijoOverTurnL.g += calcular_costo(hijoOverTurnL.st, actOVERTURN_L);
		hijoOverTurnL.h = heuristica(hijoOverTurnL.st, destino);
		hijoOverTurnL.f = hijoOverTurnL.g + hijoOverTurnL.h;
		hijoOverTurnL.secuencia.push_back(actOVERTURN_L);

		// it = Cerrados.find(hijoTurnL);
		if (Cerrados.find(hijoOverTurnL) == Cerrados.end())
		{

			Abiertos.push(hijoOverTurnL);
		}
		else if (hijoOverTurnL < *Cerrados.find(hijoOverTurnL))
		{
			Abiertos.push(hijoOverTurnL);
		}

		//Generar descendiente de girar a la izquierda 45 grados
		nodo_estrella hijoSEMITurnL = current;
		hijoSEMITurnL.st.orientacion = (hijoSEMITurnL.st.orientacion + 7) % 8;
		hijoSEMITurnL.g += calcular_costo(hijoSEMITurnL.st, actSEMITURN_L);
		hijoSEMITurnL.h = heuristica(hijoSEMITurnL.st, destino);
		hijoSEMITurnL.f = hijoSEMITurnL.g + hijoSEMITurnL.h;
		hijoSEMITurnL.secuencia.push_back(actSEMITURN_L);

		//it = Cerrados.find(hijoSEMITurnL)
		if (Cerrados.find(hijoSEMITurnL) == Cerrados.end())
		{

			Abiertos.push(hijoSEMITurnL);
		}
		else if (hijoSEMITurnL < *Cerrados.find(hijoSEMITurnL))
		{
			Abiertos.push(hijoSEMITurnL);
		}

		// Generar descendiente de avanzar
		nodo_estrella hijoForward = current;
		hijoForward.g += calcular_costo(hijoForward.st, actFORWARD);
		hijoForward.h = heuristica(hijoForward.st, destino);
		hijoForward.f = hijoForward.g + hijoForward.h;
		hijoForward.secuencia.push_back(actFORWARD);
		if (!HayObstaculoDelante(hijoForward.st))
		{
			// it = Cerrados.find(hijoForward);
			if (Cerrados.find(hijoForward) == Cerrados.end())
			{

				Abiertos.push(hijoForward);
			}
			else if (hijoForward < *Cerrados.find(hijoForward))
			{
				Abiertos.push(hijoForward);
			}
		}

		// Tomo el siguiente valor de la Abiertos
		if (!Abiertos.empty())
		{
			current = Abiertos.top();
			while (!Abiertos.empty() && Cerrados.find(current) != Cerrados.end())
			{
				Abiertos.pop();
				if (!Abiertos.empty())
				{
					current = Abiertos.top();
				}
			}
		}
	}

	cout << "Terminada la busqueda\n";

	if (current.st.fila == destino.fila and current.st.columna == destino.columna)
	{
		cout << "Cargando el plan\n";
		plan = current.secuencia;
		cout << "Longitud del plan: " << plan.size() << endl;
		PintaPlan(plan);
		// ver el plan en el mapa
		VisualizaPlan(origen, plan);
		return true;
	}
	else
	{
		cout << "No encontrado plan\n";
	}

	return false;
}

//Nivel 2
struct comparaEstadosEstrella2
{
	bool operator()(const nodo_estrella &a, const nodo_estrella &n) const
	{
		if ((a.st.fila > n.st.fila) or (a.st.fila == n.st.fila and a.st.columna > n.st.columna) or
			(a.st.fila == n.st.fila and a.st.columna == n.st.columna and a.st.orientacion > n.st.orientacion) or
			(a.st.fila == n.st.fila and a.st.columna == n.st.columna and a.st.orientacion == n.st.orientacion and a.st.bikini > n.st.bikini) or
			(a.st.fila == n.st.fila and a.st.columna == n.st.columna and a.st.orientacion == n.st.orientacion and a.st.bikini == n.st.bikini and a.st.zapatillas > n.st.zapatillas) or
			(a.st.fila == n.st.fila and a.st.columna == n.st.columna and a.st.orientacion == n.st.orientacion and a.st.bikini == n.st.bikini and a.st.zapatillas == n.st.zapatillas and a.st.obj_visitados > n.st.obj_visitados))
			return true;
		else
			return false;
	}
};

int ComportamientoJugador::mejorHeuristica(const estado &origen, vector<estado> destinos, vector<bool> visitados)
{
	int valor = 300000;

	for (int i = 0; i < numero_obj; i++)
	{
		if (!visitados[i] and valor >= heuristica(origen, destinos[i]))
		{
			valor = heuristica(origen, destinos[i]);
		}
	}

	return valor;
}
/*Algoritmo Aestrella para varios objetivos */
bool ComportamientoJugador::pathFinding_Aestrella(const estado &origen, vector<estado> &destino, list<Action> &plan)
{
	// Borro la lista
	cout << "Calculando plan\n";
	plan.clear();

	set<nodo_estrella, comparaEstadosEstrella2> Cerrados; // Lista de Cerrados
	priority_queue<nodo_estrella> Abiertos;				  // Lista de Abiertos

	nodo_estrella current;
	current.st = origen;
	current.secuencia.empty();
	current.g = 0;

	current.st.bikini = bikini;
	current.st.zapatillas = zapatillas;
	current.st.obj_visitados = 0;

	for (int j = 0; j < numero_obj; j++)
	{
		current.st.visitados.push_back(false);
	}

	current.h = mejorHeuristica(current.st, destino, current.st.visitados);
	current.f = current.g + current.h;

	Abiertos.push(current);

	bool end = false;

	while (!Abiertos.empty() and !end)
	{

		Abiertos.pop();
		Cerrados.insert(current);

		if (mapaResultado[current.st.fila][current.st.columna] == 'K')
		{
			current.st.bikini = true;
			current.st.zapatillas = false;
		}

		if (mapaResultado[current.st.fila][current.st.columna] == 'D')
		{
			current.st.zapatillas = true;
			current.st.bikini = false;
		}
		//Vemos si hemos alcanzado alguno de los objetivos
		for (int i = 0; i < numero_obj; i++)
		{
			if (current.st.fila == destino[i].fila and current.st.columna == destino[i].columna and !current.st.visitados[i])
			{
				current.st.visitados[i] = true;
				current.st.obj_visitados += 1;
			}
		}

		//Comprobamos si hemos viistados todos los objetivos
		if (numero_obj == current.st.obj_visitados)
		{
			end = true;
		}

		else
		{
			// Generar descendiente de overturnR
			nodo_estrella hijoOverTurnR = current;
			hijoOverTurnR.st.orientacion = (hijoOverTurnR.st.orientacion + 3) % 8;
			hijoOverTurnR.g += calcular_costo(hijoOverTurnR.st, actOVERTURN_R);
			hijoOverTurnR.h = mejorHeuristica(hijoOverTurnR.st, destino, hijoOverTurnR.st.visitados);
			hijoOverTurnR.f = hijoOverTurnR.g + hijoOverTurnR.h;
			
			// it = Cerrados.find(hijoTurnR);
			if (Cerrados.find(hijoOverTurnR) == Cerrados.end())
			{
				hijoOverTurnR.secuencia.push_back(actOVERTURN_R);
				Abiertos.push(hijoOverTurnR);
			}
			else if (hijoOverTurnR < *Cerrados.find(hijoOverTurnR))
			{
				hijoOverTurnR.secuencia.push_back(actOVERTURN_R);
				Abiertos.push(hijoOverTurnR);
			}

			// Generar descendiente de girar a la derecha 45 grados
			nodo_estrella hijoSEMITurnR = current;
			hijoSEMITurnR.st.orientacion = (hijoSEMITurnR.st.orientacion + 1) % 8;
			hijoSEMITurnR.g += calcular_costo(hijoSEMITurnR.st, actSEMITURN_R);
			hijoSEMITurnR.h = mejorHeuristica(hijoSEMITurnR.st, destino, hijoSEMITurnR.st.visitados);
			hijoSEMITurnR.f = hijoSEMITurnR.g + hijoSEMITurnR.h;
			
			// it = Cerrados.find(hijoSEMITurnR);
			if (Cerrados.find(hijoSEMITurnR) == Cerrados.end())
			{
				hijoSEMITurnR.secuencia.push_back(actSEMITURN_R);
				Abiertos.push(hijoSEMITurnR);
			}
			else if (hijoSEMITurnR < *Cerrados.find(hijoSEMITurnR))
			{
				hijoSEMITurnR.secuencia.push_back(actSEMITURN_R);
				Abiertos.push(hijoSEMITurnR);
			}

			// Generar descendiente de girar a la izquierda 90 grados
			nodo_estrella hijoOverTurnL = current;
			hijoOverTurnL.st.orientacion = (hijoOverTurnL.st.orientacion + 5) % 8;
			hijoOverTurnL.g += calcular_costo(hijoOverTurnL.st, actOVERTURN_L);
			hijoOverTurnL.h = mejorHeuristica(hijoOverTurnL.st, destino, hijoOverTurnL.st.visitados);
			hijoOverTurnL.f = hijoOverTurnL.g + hijoOverTurnL.h;
			

			// it = Cerrados.find(hijoTurnL);
			if (Cerrados.find(hijoOverTurnL) == Cerrados.end())
			{
				hijoOverTurnL.secuencia.push_back(actOVERTURN_L);
				Abiertos.push(hijoOverTurnL);
			}
			else if (hijoOverTurnL < *Cerrados.find(hijoOverTurnL))
			{
				hijoOverTurnL.secuencia.push_back(actOVERTURN_L);
				Abiertos.push(hijoOverTurnL);
			}

			//Generar descendiente de girar a la izquierda 45 grados
			nodo_estrella hijoSEMITurnL = current;
			hijoSEMITurnL.st.orientacion = (hijoSEMITurnL.st.orientacion + 7) % 8;
			hijoSEMITurnL.g += calcular_costo(hijoSEMITurnL.st, actSEMITURN_L);
			hijoSEMITurnL.h = mejorHeuristica(hijoSEMITurnL.st, destino, hijoSEMITurnL.st.visitados);
			hijoSEMITurnL.f = hijoSEMITurnL.g + hijoSEMITurnL.h;
			

			//it = Cerrados.find(hijoSEMITurnL)
			if (Cerrados.find(hijoSEMITurnL) == Cerrados.end())
			{
				hijoSEMITurnL.secuencia.push_back(actSEMITURN_L);
				Abiertos.push(hijoSEMITurnL);
			}
			else if (hijoSEMITurnL < *Cerrados.find(hijoSEMITurnL))
			{
				hijoSEMITurnL.secuencia.push_back(actSEMITURN_L);
				Abiertos.push(hijoSEMITurnL);
			}

			// Generar descendiente de avanzar
			nodo_estrella hijoForward = current;
			hijoForward.g += calcular_costo(hijoForward.st, actFORWARD);
			hijoForward.h = mejorHeuristica(hijoForward.st, destino, hijoForward.st.visitados);
			hijoForward.f = hijoForward.g + hijoForward.h;
			
			if (!HayObstaculoDelante(hijoForward.st))
			{
				// it = Cerrados.find(hijoForward);
				if (Cerrados.find(hijoForward) == Cerrados.end())
				{
					hijoForward.secuencia.push_back(actFORWARD);
					Abiertos.push(hijoForward);
				}
				else if (hijoForward < *Cerrados.find(hijoForward))
				{
					hijoForward.secuencia.push_back(actFORWARD);
					Abiertos.push(hijoForward);
				}
			}

			// Tomo el siguiente valor de la Abiertos
			if (!Abiertos.empty())
			{
				current = Abiertos.top();
				while (!Abiertos.empty() && Cerrados.find(current) != Cerrados.end())
				{
					Abiertos.pop();
					if (!Abiertos.empty())
					{
						current = Abiertos.top();
					}
				}
			}
		}
	}

	cout << "Terminada la busqueda\n";

	if (end)
	{
		cout << "Cargando el plan\n";
		plan = current.secuencia;
		cout << "Longitud del plan: " << plan.size() << endl;
		PintaPlan(plan);
		// ver el plan en el mapa
		VisualizaPlan(origen, plan);
		return true;
	}
	else
	{
		cout << "No encontrado plan\n";
	}

	return false;
}