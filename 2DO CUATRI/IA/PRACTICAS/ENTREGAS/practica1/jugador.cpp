#include "../Comportamientos_Jugador/jugador.hpp"
#include <iostream>
#include <stdlib.h>
#include <time.h>
using namespace std;

Action ComportamientoJugador::think(Sensores sensores)
{

	//AÑADIMOS LOS PRECIPICIOS ANTES DE TODO LO DEMÁS
	if (primer_paso)
	{
		noPrimerPaso();

		for (int i = 0; i < mapaResultado.size(); i++)
		{
			for (int j = 0; j < mapaResultado.size(); j++)
			{
				if (i < 3 || i >= mapaResultado.size() - 3 || j < 3 || j >= mapaResultado.size() - 3)
				{
					mapaResultado[i][j] = 'P';
				}
			}
		}
	}

	Action accion = actIDLE;

	//Si no estamos en el nivel 0 y tampoco en el 1, entonces necesitamos la brujula del tutorial
	if (sensores.nivel != 0 && sensores.nivel != 1)
	{
		switch (ultimaAccion)
		{
		case actFORWARD:
			switch (brujula)
			{
			case 0: //norte
				fil--;
				break;
			case 1: //este
				col++;
				break;
			case 2: //sur
				fil++;
				break;
			case 3: //oeste
				col--;
				break;
			}
			break;
		case actTURN_L:
			brujula = (brujula + 3) % 4;
			girar_derecha = (rand() % 2 == 0);
			break;
		case actTURN_R:
			brujula = (brujula + 1) % 4;
			girar_derecha = (rand() % 2 == 0);
			break;
		}
	}

	//No tenemos sensor de posicion en cualquier nivel que no sea el 0
	if (sensores.nivel != 0)
	{
		if (sensores.terreno[0] == 'G' && !bien_situado)
		{
			fil = sensores.posF;
			col = sensores.posC;

			if (sensores.nivel == 1)
			{
				bien_situado = true;
			}
			if (sensores.nivel != 1)
			{
				if (sensores.terreno[0] == 'G' && !bien_situado)
				{
					brujula = sensores.sentido;
					bien_situado = true;
				}
			}
		}
	}
	else
	{
		bien_situado = true;
		fil = sensores.posF;
		col = sensores.posC;
		brujula = sensores.sentido;
	}

	//Comprobamos si estamos en una casilla de batería
	if (sensores.terreno[0] == 'X' && sensores.bateria < 4990)
	{
		sensores.bateria += 10;
	}

	//Comprobamos que no topemos con un lobo o con un precipicio
	if (sensores.superficie[0] == 'l' || sensores.terreno[0] == 'P')
	{
		sensores.reset = true;
	}

	//Modificamos nuestras variables de estado bikini y zapatillas si nos topamos con alguno de ellos.
	if (sensores.terreno[0] == 'K')
	{
		setBik(true);
	}

	if (sensores.terreno[0] == 'D')
	{
		setZapas(true);
	}

	//Si estamos bien situados, llamamos a sensores
	if (bien_situado)
	{
		copiarResultado(sensores);
	}

	//Decidir nueva accion
	/* Al principio, nos deberíamos enfocar en posicionarnos correctamente si no lo estamos gastando
	   la menos batería posible en base a nuestro sensores y sin deliberar.
	   Una zona observada por nuestro jugador, será prometedora si hay en alguna de las casillas de nuestro
	   vector de sensores una casilla de posicionamiento, de bikini o de zapatillas. Si la hay vamos avanzando. Si no giramos, si tras cuatro giros
	   (es decir, volvemos a la orientación original), no se ve ninguna zona prometedora, nuestro agente avanza y si no puede avanzar
	   hace un giro aleatorio. */

	if (!bien_situado)
	{ //Vamos a evitar el agua, por lo general es un gran gasto de batería si no tenemos bikini. Hay que tener en cuenta que lo primero que tenemos que hacer es posicionarnos

		if ((zonaPrometedoraPos(sensores) && puedeAvanzar(sensores)))
		{
			//Aquí tengo que tener en cuenta los precipicios
			accion = actFORWARD;
			contador_giros = 0;
		}

		else if (((zonaPrometedoraBik(sensores) && !bikini) || (zonaPrometedoraDep(sensores) && !zapatillas)) && puedeAvanzar(sensores))
		{
			accion = actFORWARD;
			contador_giros = 0;
		}
		else
		{
			if (contador_giros >= 4 && puedeAvanzar(sensores))
			{
				accion = actFORWARD;
				contador_giros = 0;
			}
			else if (contador_giros < 4)
			{
				accion = actTURN_L;
				contador_giros += 1;
			}
			else
			{
				accion = giro_aleatorio();
				contador_giros += 1;
			}
		}
	}
	else //Si estamos posicionados recorremos el mapa más libremente y con cierta aleatoriedad
	{
		//en el caso en el que tenga la posicion bien ya juego con sentido
		if ((zonaPrometedoraBik(sensores) || zonaPrometedoraDep(sensores)) || puedeAvanzar(sensores))
		{
			accion = actFORWARD;
			contador_giros = 0;
		}
		else
		{
			if ((contador_giros >= 4 && puedeAvanzar(sensores)))
			{
				accion = actFORWARD;
				contador_giros = 0;
			}
			else if (contador_giros < 4)
			{
				accion = actTURN_L;
				contador_giros += 1;
			}
			else
			{
				accion = giro_aleatorio();
				contador_giros += 1;
			}
		}
	}

	// Determinar el efecto de la ultima accion enviada
	ultimaAccion = accion; //aqui recordamos ultima accion
	return accion;
}

int ComportamientoJugador::interact(Action accion, int valor)
{
	return false;
}

//A partir de aquí tenemos 3 métodos similares que se encargan de devolver true si tenemos en nuestros sensores (siempre que no sea la casilla actual)
//si encuentra una casilla de posicionamiento, de bikini o de deportivas.
bool ComportamientoJugador::zonaPrometedoraPos(Sensores sensores)
{
	bool promete = false;
	int contador_peligros = 0;

	//En principio, si una zona promete será porque podemos optar a posicionarnos en una casilla
	//de posicionamiento.
	for (int i = 0; i < 16; i++)
	{
		if (i != 0)
		{
			if (sensores.terreno[i] == 'G')
				promete = true;
		}
	}

	return promete;

	//Los peligros se tendran en cuenta a la hora de avanzar a la siguiente casilla, este metodo
	//unicamente interesa para ver si una zona merece ser avanzada.
}

bool ComportamientoJugador::zonaPrometedoraBik(Sensores sensores)
{
	bool promete = false;
	int contador_peligros = 0;

	//En principio, si una zona promete será porque podemos optar a posicionarnos en una casilla
	//de posicionamiento.
	for (int i = 0; i < 16; i++)
	{
		if (i != 0)
		{ //si estamos en el sensor ya no es zona prometedora
			if (sensores.terreno[i] == 'K')
				promete = true;
		}
	}

	return promete;

	//Los peligros se tendran en cuenta a la hora de avanzar a la siguiente casilla, este metodo
	//unicamente interesa para ver si una zona merece ser avanzada.
}

bool ComportamientoJugador::zonaPrometedoraDep(Sensores sensores)
{
	bool promete = false;
	int contador_peligros = 0;

	//En principio, si una zona promete será porque podemos optar a posicionarnos en una casilla
	//de posicionamiento.
	for (int i = 0; i < 16; i++)
	{
		if (i != 0)
		{
			if (sensores.terreno[i] == 'D')
				promete = true;
		}
	}

	return promete;

	//Los peligros se tendran en cuenta a la hora de avanzar a la siguiente casilla, este metodo
	//unicamente interesa para ver si una zona merece ser avanzada.
}

/**
 * Este método se encarga de copiar los sensores en base a la posicion que tengamos (dado que no recorreríamos de igual forma la matriz).
 * 
*/
void ComportamientoJugador::copiarResultado(Sensores sensores)
{
	int contador = 0;		   //para el bucle
	int contador_escritos = 1; //En el que estamos situados se escribe si o si
	bool first_inte = true;

	switch (brujula)
	{
	case 0:
		for (int i = fil; i > fil - 4; i--)
		{

			for (int j = col - contador; j <= col + contador && !first_inte; j++)
			{
				mapaResultado[i][j] = sensores.terreno[contador_escritos];
				contador_escritos += 1;
			}

			contador += 1;

			if (first_inte)
			{
				mapaResultado[fil][col] = sensores.terreno[0];
				first_inte = false;
			}
		}

		break;
	case 1:
		for (int i = col; i < col + 4; i++)
		{
			for (int j = fil - contador; j <= fil + contador && !first_inte; j++)
			{
				mapaResultado[j][i] = sensores.terreno[contador_escritos];
				contador_escritos += 1;
			}

			contador += 1;

			if (first_inte)
			{
				mapaResultado[fil][col] = sensores.terreno[0];
				first_inte = false;
			}
		}
		break;
	case 2:
		for (int i = fil; i < fil + 4; i++)
		{
			for (int j = col + contador; j >= col - contador && !first_inte; j--)
			{
				mapaResultado[i][j] = sensores.terreno[contador_escritos];
				contador_escritos += 1;
			}

			contador += 1;

			if (first_inte)
			{
				mapaResultado[fil][col] = sensores.terreno[0];
				first_inte = false;
			}
		}
		break;
	case 3:
		for (int i = col; i > col - 4; i--)
		{
			for (int j = fil + contador; j >= fil - contador && !first_inte; j--)
			{
				mapaResultado[j][i] = sensores.terreno[contador_escritos];
				contador_escritos += 1;
			}

			contador += 1;

			if (first_inte)
			{
				mapaResultado[fil][col] = sensores.terreno[0];
				first_inte = false;
			}
		}
		break;
	}
}

//Este método devuelve un giro aleatorio
Action ComportamientoJugador::giro_aleatorio()
{
	int num_aleatorio;

	srand(time(NULL));

	num_aleatorio = rand();

	if (num_aleatorio % 2 == 0)
	{
		return actTURN_R;
	}
	else
	{
		return actTURN_L;
	}
}

//Cambiamos nuestra variable de estado
void ComportamientoJugador::noPrimerPaso()
{
	primer_paso = false;
}


/**
 * Este método devuelve true diferenciando entre si temeos bikini o zapatillas para ahorrar el máximo de batería
 * posible.
*/
bool ComportamientoJugador::puedeAvanzar(Sensores sensores)
{ //Método para según el nivel y los objetos que tengamos, nuestro objeto avance o no

	//aniadir el no pasar por casillas ya visitadas (meter un giro aleatorio y voy probando comportamiento)
	//añadir lo visto hasta posicionarse

	if (!bikini && !zapatillas)
	{

		if ((sensores.terreno[2] == 'T' || sensores.terreno[2] == 'X' || sensores.terreno[2] == 'S' || sensores.terreno[2] == 'G' || sensores.terreno[2] == 'K' || sensores.terreno[2] == 'D') && sensores.superficie[2] == '_')
		{
			return true;
		}
		else if ((sensores.terreno[0] == 'A' || sensores.terreno[0] == 'B') && (sensores.terreno[2] != 'P' && sensores.superficie[2] != 'l'))
		{
			return true;
		}
	}

	else
	{

		if (bikini && !zapatillas)
		{

			if ((sensores.terreno[2] == 'T' || sensores.terreno[2] == 'X' || sensores.terreno[2] == 'S' || sensores.terreno[2] == 'G' || sensores.terreno[2] == 'A' || sensores.terreno[2] == 'D') && sensores.superficie[2] == '_')
			{
				return true;
			}
			else
			{
				return false;
			}
		}
		if (zapatillas && !bikini)
		{

			if ((sensores.terreno[2] == 'T' || sensores.terreno[2] == 'X' || sensores.terreno[2] == 'S' || sensores.terreno[2] == 'G' || sensores.terreno[2] == 'B' || sensores.terreno[2] == 'K') && sensores.superficie[2] == '_')
			{
				return true;
			}
			else
			{
				return false;
			}
		}
		if (zapatillas && bikini)
		{

			if ((sensores.terreno[2] == 'T' || sensores.terreno[2] == 'X' || sensores.terreno[2] == 'S' || sensores.terreno[2] == 'G' || sensores.terreno[2] == 'A' || sensores.terreno[2] == 'B') && sensores.superficie[2] == '_')
			{
				return true;
			}
			else
			{
				return false;
			}
		}
	}
}

//Métodos para establecer algunas de nuestras variables de estado
void ComportamientoJugador::setBik(bool estado)
{
	this->bikini = estado;
}

void ComportamientoJugador::setZapas(bool estado)
{
	this->zapatillas = estado;
}
