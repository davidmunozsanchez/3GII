#include "AIPlayer.h"
#include "Parchis.h"

const double masinf = 9999999999.0, menosinf = -9999999999.0;
const double gana = masinf - 1, pierde = menosinf + 1;
const int num_pieces = 4;
const int PROFUNDIDAD_MINIMAX = 4;  // Umbral maximo de profundidad para el metodo MiniMax
const int PROFUNDIDAD_ALFABETA = 6; // Umbral maximo de profundidad para la poda Alfa_Beta

bool AIPlayer::move()
{
    cout << "Realizo un movimiento automatico" << endl;

    color c_piece;
    int id_piece;
    int dice;
    think(c_piece, id_piece, dice);

    cout << "Movimiento elegido: " << str(c_piece) << " " << id_piece << " " << dice << endl;

    actual->movePiece(c_piece, id_piece, dice);
    return true;
}

void AIPlayer::think(color &c_piece, int &id_piece, int &dice) const
{
    // IMPLEMENTACIÓN INICIAL DEL AGENTE
    // Esta implementación realiza un movimiento aleatorio.
    // Se proporciona como ejemplo, pero se debe cambiar por una que realice un movimiento inteligente
    // como lo que se muestran al final de la función.

    // OBJETIVO: Asignar a las variables c_piece, id_piece, dice (pasadas por referencia) los valores,
    // respectivamente, de:
    // - color de ficha a mover
    // - identificador de la ficha que se va a mover
    // - valor del dado con el que se va a mover la ficha.

    // El color de ficha que se va a mover
    /*c_piece = actual->getCurrentColor();

    // Vector que almacenará los dados que se pueden usar para el movimiento
    vector<int> current_dices;
    // Vector que almacenará los ids de las fichas que se pueden mover para el dado elegido.
    vector<int> current_pieces;

    // Se obtiene el vector de dados que se pueden usar para el movimiento
    current_dices = actual->getAvailableDices(c_piece);
    // Elijo un dado de forma aleatoria.
    dice = current_dices[rand() % current_dices.size()];

    // Se obtiene el vector de fichas que se pueden mover para el dado elegido
    current_pieces = actual->getAvailablePieces(c_piece, dice);

    // Si tengo fichas para el dado elegido muevo una al azar.
    if (current_pieces.size() > 0)
    {
        id_piece = current_pieces[rand() % current_pieces.size()];
    }
    else
    {
        // Si no tengo fichas para el dado elegido, pasa turno (la macro SKIP_TURN me permite no mover).
        id_piece = SKIP_TURN;
    }*/

    // El siguiente código se proporciona como sugerencia para iniciar la implementación del agente.

    double valor;                           // Almacena el valor con el que se etiqueta el estado tras el proceso de busqueda.
    double alpha = menosinf, beta = masinf; // Cotas iniciales de la poda AlfaBeta
    // Llamada a la función para la poda (los parámetros son solo una sugerencia, se pueden modificar).
    /*valor = Poda_AlfaBeta(*actual, jugador, 0, PROFUNDIDAD_ALFABETA, c_piece, id_piece, dice, alpha, beta, ValoracionTest);
    cout << "Valor MiniMax: " << valor << "  Accion: " << str(c_piece) << " " << id_piece << " " << dice << endl;*/

    // ----------------------------------------------------------------- //

    // Si quiero poder manejar varias heurísticas, puedo usar la variable id del agente para usar una u otra.
    switch (id)
    {
    case 0:
        valor = Poda_AlfaBeta(*actual, jugador, 0, PROFUNDIDAD_ALFABETA, c_piece, id_piece, dice, alpha, beta, heuristica);
        break;
    case 1:
        valor = Poda_AlfaBeta(*actual, jugador, 0, PROFUNDIDAD_ALFABETA, c_piece, id_piece, dice, alpha, beta, ValoracionTest);
        break;
        /*case 2:
            valor = Poda_AlfaBeta(*actual, jugador, 0, PROFUNDIDAD_ALFABETA, c_piece, id_piece, dice, alpha, beta, MiValoracion1);
            break;*/
    }
    cout << "Valor MiniMax: " << valor << "  Accion: " << str(c_piece) << " " << id_piece << " " << dice << endl;
}

double AIPlayer::ValoracionTest(const Parchis &estado, int jugador)
{
    // Heurística de prueba proporcionada para validar el funcionamiento del algoritmo de búsqueda.

    int ganador = estado.getWinner();
    int oponente = (jugador + 1) % 2;

    // Si hay un ganador, devuelvo más/menos infinito, según si he ganado yo o el oponente.
    if (ganador == jugador)
    {
        return gana;
    }
    else if (ganador == oponente)
    {
        return pierde;
    }
    else
    {
        // Colores que juega mi jugador y colores del oponente
        vector<color> my_colors = estado.getPlayerColors(jugador);
        vector<color> op_colors = estado.getPlayerColors(oponente);

        // Recorro todas las fichas de mi jugador
        int puntuacion_jugador = 0;
        // Recorro colores de mi jugador.
        for (int i = 0; i < my_colors.size(); i++)
        {
            color c = my_colors[i];
            // Recorro las fichas de ese color.
            for (int j = 0; j < num_pieces; j++)
            {
                // Valoro positivamente que la ficha esté en casilla segura o meta.
                if (estado.isSafePiece(c, j))
                {
                    puntuacion_jugador++;
                }
                else if (estado.getBoard().getPiece(c, j).type == goal)
                {
                    puntuacion_jugador += 5;
                }
            }
        }

        // Recorro todas las fichas del oponente
        int puntuacion_oponente = 0;
        // Recorro colores del oponente.
        for (int i = 0; i < op_colors.size(); i++)
        {
            color c = op_colors[i];
            // Recorro las fichas de ese color.
            for (int j = 0; j < num_pieces; j++)
            {
                if (estado.isSafePiece(c, j))
                {
                    // Valoro negativamente que la ficha esté en casilla segura o meta.
                    puntuacion_oponente++;
                }
                else if (estado.getBoard().getPiece(c, j).type == goal)
                {
                    puntuacion_oponente += 5;
                }
            }
        }

        // Devuelvo la puntuación de mi jugador menos la puntuación del oponente.
        return puntuacion_jugador - puntuacion_oponente;
    }
}

//***********IMPLEMENTACION PODA ALFA-BETA con la cabecera propuesta******************
double AIPlayer::Poda_AlfaBeta(const Parchis &st_actual, int jugador, int profundidad, int profundidad_max, color &c_piece, int &id_piece, int &dice, double alpha, double beta, double (*heuristic)(const Parchis &, int)) const
{
    double valor_heuristica;
    //Si estamos en la profundidad máxima o si el juego ha terminado
    //quiere decir que estamos en un nodo hoja y lo tenemos que evaluar con la
    //heurística correspondiente
    if (profundidad == profundidad_max or st_actual.gameOver())
        return heuristic(st_actual, jugador);
    
    //Variables para generar los hijos de cada nodo
    int id_piece_auxiliar = -1;
    int dice_auxiliar = -1;
    color c_piece_auxiliar = none;

    //Se genera siguiente hijo
    Parchis nextMove = st_actual.generateNextMoveDescending(c_piece_auxiliar, id_piece_auxiliar, dice_auxiliar);

    while (!(nextMove == st_actual))
    {
        //Comprobacion para los casos en que aparezcan nodos max o min seguidos
        if (this->jugador != st_actual.getCurrentPlayerId())
        {
            //Llamamos recursivamente a la poda para evaluar el hijo que hemos generado
            valor_heuristica = Poda_AlfaBeta(nextMove, jugador, profundidad + 1, profundidad_max, c_piece_auxiliar, id_piece_auxiliar, dice_auxiliar, alpha, beta, heuristic);

            beta = min(valor_heuristica, beta);

            if (beta <= alpha)
            {
                break;
            }
        }
        else
        {
            valor_heuristica = Poda_AlfaBeta(nextMove, jugador, profundidad + 1, profundidad_max, c_piece_auxiliar, id_piece_auxiliar, dice_auxiliar, alpha, beta, heuristic);

            double aux = alpha;

            alpha = max(valor_heuristica, alpha);

            if (profundidad == 0 && aux != alpha)
            {

                dice = dice_auxiliar;
                c_piece = c_piece_auxiliar;
                id_piece = id_piece_auxiliar;
            }
            if (alpha >= beta)
            {
                break;
            }
        }
        nextMove = st_actual.generateNextMoveDescending(c_piece_auxiliar, id_piece_auxiliar, dice_auxiliar);
    }

    if (jugador == st_actual.getCurrentPlayerId())
        return alpha;
    else
        return beta;
}

// Heuristica que mejora el comportamiento aleatorio que se nos proporciona al principio
// y los diferentes metodos think del tutorial de esta practica
// Esta
double AIPlayer::heuristica(const Parchis &st_actual, int jugador)
{
    double asignado = 0;
    int oponente = (jugador + 1) % 2;

    int ganador = st_actual.getWinner();
    if (ganador == jugador)
    {
        return gana;
    }
    else if (ganador == oponente)
    {
        return pierde;
    }
    else
    {

        vector<color> my_colors = st_actual.getPlayerColors(jugador);
        vector<color> op_colors = st_actual.getPlayerColors(oponente);

        //Por cada pieza que tengamos segura sumamos 1 a la valoración del tablero
        for (int i = 0; i < my_colors.size(); i++)
        {
            color c = my_colors[i];
            for (int j = 0; j < num_pieces; j++)
            {
                if (st_actual.isSafePiece(c, j))
                {
                    asignado += 1;
                }
            }
        }

        //Por cada pieza asegurada del oponente le quitamos 1, ya que el tablero no es tan beneficioso
        //para nosotros.
        for (int i = 0; i < op_colors.size(); i++)
        {
            color c = op_colors[i];
            for (int j = 0; j < num_pieces; j++)
            {
                if (st_actual.isSafePiece(c, j))
                    asignado -= 1;
            }
        }

        // valor_asignado = valor_asignado + ValoracionTest(st_actual,jugador);

        //Le quito la media de distancias de mis fichas a la meta, mientras mas lejos estén,
        //más se restara y el tablero será peor.
        asignado -= calcular_media(st_actual, jugador, my_colors[0]);
        asignado -= calcular_media(st_actual, jugador, my_colors[1]);
        //Mientras más piezas tenga en casa el oponente mejor es el tablero
        asignado += 4 * st_actual.piecesAtHome(op_colors[0]);
        asignado += 4 * st_actual.piecesAtHome(op_colors[1]);
        //Mientras más piezas tenga en la meta el oponente peor es el tablero
        asignado -= 3 * st_actual.piecesAtGoal(op_colors[0]);
        asignado -= 3 * st_actual.piecesAtGoal(op_colors[1]);
        //Si tengo piezas en casa quito el núero de piezas por 3, el tablero es peor
        asignado -= 4 * st_actual.piecesAtHome(my_colors[0]);
        asignado -= 4 * st_actual.piecesAtHome(my_colors[1]);
        //Si las tengo justo en la meta hago lo contrario, ya que mi tablero es mejor
        asignado += 3 * st_actual.piecesAtGoal(my_colors[0]);
        asignado += 3 * st_actual.piecesAtGoal(my_colors[1]);
        //Mientras más lejos estén de la meta las fichas del oponente mejor sera el tablero
        asignado += calcular_media(st_actual, jugador, op_colors[0]);
        asignado += calcular_media(st_actual, jugador, op_colors[1]);
        

        return asignado;
    }
}

double AIPlayer::calcular_media(const Parchis &st_actual, int jugador, color color)
{
    double media = 0;
    // De este modo calculamos la media de la distancias de las fichas hasta la meta
    for (int i = 0; i < num_pieces; i++)
        media += st_actual.distanceToGoal(color, i);

    media /= num_pieces;

    return media;
}