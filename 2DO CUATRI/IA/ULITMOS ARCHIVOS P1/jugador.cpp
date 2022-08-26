#include "../Comportamientos_Jugador/jugador.hpp"
#include <iostream>
#include <stdlib.h>
#include <time.h>
using namespace std;

Action ComportamientoJugador::think(Sensores sensores)
{

	Action accion = actIDLE;

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

	if (sensores.terreno[0] == 'X' && sensores.bateria < 4990)
	{
		sensores.bateria += 10;
	}

	if (sensores.superficie[0] == 'l' || sensores.terreno[0] == 'P')
	{
		sensores.reset = true;
	}

	if (sensores.superficie[0] == 'K')
	{
		bikini = true;
	}

	if (sensores.superficie[0] == 'D')
	{
		zapatillas = true;
	}

	if (bien_situado)
	{
		copiarResultado(sensores);
	}

	//Decidir nueva accion
	/* Al principio, nos deberíamos enfocar en posicionarnos correctamente si no lo estamos gastando
	   la menos batería posible en base a nuestro sensores y sin deliberar.
	   Una zona observada por nuestro jugador, será prometedora si hay en alguna de las casillas de nuestro
	   vector de sensores una casilla de posicionamiento. Si la hay vamos avanzando. Si no giramos, si tras cuatro giros
	   (es decir, volvemos a la orientación original), no se ve ninguna zona prometedora, nuestro agente avanza 3 casillas seguidas. */

	if (!bien_situado)
	{ //Vamos a evitar el agua, por lo general es un gran gasto de batería si no tenemos bikini. Hay que tener en cuenta que lo primero que tenemos que hacer es posicionarnos

		if ((zonaPrometedoraPos(sensores) || contador_giros >= 4) && ((sensores.terreno[2] == 'T' || sensores.terreno[2] == 'S' || sensores.terreno[2] == 'G') && sensores.superficie[2] == '_'))
		{
			//Aquí tengo que tener en cuenta los precipicios
			accion = actFORWARD;
			contador_giros = 0;
		}
		else
		{
			accion = giro_aleatorio();
			contador_giros += 1;
		}
	}
	else
	{
		//en el caso en el que tenga la posicion bien ya juego con sentido
		if (!bikini)
		{
			//
		}

		//Vamos a evitar el agua, por lo general es un gran gasto de batería si no tenemos bikini. Hay que tener en cuenta que lo primero que tenemos que hacer es posicionarnos
		if ((sensores.terreno[2] == 'T' || sensores.terreno[2] == 'S' || sensores.terreno[2] == 'G') && sensores.superficie[2] == '_')
		{
			accion = actFORWARD;
		}
		else if (!girar_derecha)
		{
			accion = actTURN_L;
		}
		else
		{
			accion = actTURN_R;
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

bool ComportamientoJugador::zonaPrometedoraPos(Sensores sensores)
{
	bool promete = false;
	int contador_peligros = 0;

	//En principio, si una zona promete será porque podemos optar a posicionarnos en una casilla
	//de posicionamiento.
	for (int i = 0; i < 16; i++)
	{
		if (sensores.terreno[i] == 'G')
			promete = true;
	}

	return promete;

	//Los peligros se tendran en cuenta a la hora de avanzar a la siguiente casilla, este metodo
	//unicamente interesa para ver si una zona merece ser avanzada.
}

void ComportamientoJugador::copiarResultado(Sensores sensores)
{
	int contador = 0;		   //para el bucle
	int contador_escritos = 0; //En el que estamos situados se escribe si o si

	switch (brujula)
	{
	case 0:
		for (int i = fil; i > fil - 4; i--)
		{
			for (int j = col - contador; j <= col + contador; j++)
			{
				mapaResultado[i][j] = sensores.terreno[contador_escritos];
				contador_escritos += 1;
			}

			contador += 1;
		}
	case 1:
		for (int i = col; i < col + 4; i++)
		{
			for (int j = fil - contador; j <= fil + contador; j++)
			{
				mapaResultado[j][i] = sensores.terreno[contador_escritos];
				contador_escritos += 1;
			}

			contador += 1;
		}
	case 2:
		for (int i = fil; i < fil + 4; i++)
		{
			for (int j = col + contador; j >= col - contador; j--)
			{
				mapaResultado[i][j] = sensores.terreno[contador_escritos];
				contador_escritos += 1;
			}

			contador += 1;
		}

	case 3:
		for (int i = col; i > col - 4; i--)
		{
			for (int j = fil + contador; j >= fil - contador; j--)
			{
				mapaResultado[j][i] = sensores.terreno[contador_escritos];
				contador_escritos += 1;
			}

			contador += 1;
		}
	}
}

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