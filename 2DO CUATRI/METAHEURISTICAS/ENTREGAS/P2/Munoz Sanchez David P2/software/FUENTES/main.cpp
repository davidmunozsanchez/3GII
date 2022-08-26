/*
 * File:   main.cpp
 * Author: davidms
 *
 * Created on 3 de abril de 2022
 */

#include <iostream>
#include <fstream>
#include <vector>
#include <chrono>
#include "./random.hpp"

#define NUM_CROMOSOMAS 50
#define ITERACIONES 100000
#define PROB_CRUCE_AGG 0.7
#define PROB_CRUCE_AGE 1
#define PROB_MUTACION 0.1

using namespace std;

/**
 * @brief Método encargado de devoler las 2 posiciones menos fitness de una lista de fitness
 *
 * @param lista_fitness Vector de elegidos hasta el momento
 * @return Las posiciones de los dos fitness más altos.
 */
vector<int> posicionesMenosFitness(vector<double> lista_fitness);

/**
 * @brief Método encargado de transformar un vector de bool en uno de enteros.
 * Se encarga de cambiar cada 1 del vector en binario por su posicion y la pasa 
 * al vector de enteros.
 *
 * @param a_transformar Vector binario a transformar
 * @param n Tamanio del vector binario
 * @return Vector de enteros segun lo expuesto en brief.
 */
vector<int> devolverEntero(vector<bool> a_transformar, int n);

/**
 * @brief Método encargado de transformar un vector de enteros en uno binario.
 * Se encarga de crear un vector binario con 1 en las posiciones que esten en el 
 * vector de enteros.
 *
 * @param a_transformar Vector binario a transformar
 * @param n Tamanio del vector binario.
 * @param m Tamanio del vector de enteros
 * @return Vector binario segun lo expuesto en brief.
 */
vector<bool> devolverBinario(vector<int> a_transformar, int n, int m);
/**
 * @brief Método encargado de hacer el cruce por posición entre dos padres
 *
 * @param padre1 Cromosoma correspondiente con el padre1
 * @param padre2 Cromosoma correspondiente con el padre2
 * @param n Tamanio de los cromosomas
 * @return Vector de vectores de bool con dos hijos.
 */
vector<vector<bool>> crucePosicion(vector<bool> padre1, vector<bool> padre2, int semilla1, int semilla2, int n);
/**
 * @brief Método encargado de implementar el torneo binario, es decir, que dados dos cromosomas, devuelve el de mejor
 * fitness.
 *
 * @param cromosoma1 Indice del cromosoma1 en la poblacion
 * @param cromosoma2 Indice del cromosoma2 en la poblacion
 * @param distancias Lista de fitness
 * @return Entero con el índice del cromosoma ganador en la población
 */
int torneoBinario(int cromosoma1, int cromosoma2, vector<double> distancias);
/**
 * @brief Método encargado de implementar la mutacion
 *
 * @param cromosoma
 * @param pos1 Posicion primera
 * @param pos2 Posicion segunda
 * @param n Tamanio del cromosoma
 * @param m Tamanio de la solucion
 * @pre cromosoma[pos1] != cromosoma[pos2]
 */
void mutacion(vector<bool> &cromosoma, int pos1, int pos2, int n, int m);
/**
 * @brief Método encargado de hacer el cruce por posición entre dos padres
 *
 * @param padre1 Cromosoma correspondiente con el padre1
 * @param padre2 Cromosoma correspondiente con el padre2
 * @param n Tamanio de los cromosomas
 * @param m Tamanio de la solucion
 * @param matriz_dist Matriz de distancias
 * @return Devuelve un hijo.
 */
vector<bool> cruceUniforme(vector<bool> padre1, vector<bool> padre2, int n, int m, vector<vector<double>> &matriz_dist);
/**
 * @brief Método encargado de  reparar las soluciones del cruce uniforme, puesto que le pueden faltar o sobrar 1's.
 *
 * @param a_reparar Cromosoma a reparar.
 * @param n Tamanio de los cromosomas
 * @param m Tamanio de la solucion
 * @param matriz_dist Matriz de distancias
 * @return Devuelve un hijo reparado.
 */
vector<bool> reparador(vector<bool> a_reparar, int n, int m, vector<vector<double>> &matriz_dist);
/**
 * @brief Método encargado de calcular la dispersión de un cromosoma en binario
 *
 * @param cromosoma Vector de bool
 * @param distancias Matriz con las distancias entre puntos
 * @param n Tamanio del cromosoma
 * @return double Dispersión en el vector elegidos
 */
double nueva_dispersion(vector<bool> cromosoma, vector<vector<double>> &distancias, int n);
/**
 * @brief Método encargado de calcular el vector de distancias acumuladas de un cromosoma
 *
 * @param cromosoma Vector de bool
 * @param distancias Matriz con las distancias entre puntos
 * @param n Tamanio del cromosoma
 * @return double Dispersión en el vector elegidos
 */
vector<double> nueva_dispersion2(vector<bool> cromosoma, vector<vector<double>> &distancias, int n);
/**
 * @brief Funcion main donde se usa una técnica greedy para hallar la mejor solución posible
 * dados dos puntos iniciales aleatorios.
 *
 * @param argc Número de parámetros de entrada.
 * @param argv Array de cadena de caracteres correspondientes con los parámetros de entrada
 * @return int 0 si se ejecuta exitosamente
 */
int main(int argc, char **argv)
{
    using Random = effolkronium::random_static;
    ifstream entrada;
    int semilla;
    int tipo_algoritmo;
    double prob_cruce_AGG;
    const double prob_cruce_AGE = 1;
    double mutacion_valor;
    int fila, columna;
    double valor;
    int m, n;
    double min_disp, new_disp;
    int suma;
    int elegido = -1;
    double resto, resto_anterior;
    int indice_borrar;
    bool first_ite = true;

    /*Hay que introducir como argumento el nombre del fichero
      con los datos de la matriz de distancias (la diagonal superior) y la semilla a usar,
      que será un número entre 1 y 5
      Los dos primeros datos de los ficheros proporcionados son la n (la matriz será
      nxn y la m, el subconjunto con el que trabajaremos. Ademas, se añadirá un numero entre 1 y 4 para indicar el tipo de algoritmo.*/

    semilla = atoi(argv[2]); // pasamos char* a int
    tipo_algoritmo = atoi(argv[3]);
    if (argc != 4 || semilla < 1 || semilla > 5 || tipo_algoritmo < 1 || tipo_algoritmo > 4) // 4 argumentos, el programa, el fichero a leer, la semilla para inicializar la aleatoriedad y el algoritmo que queremos usar
    {
        cerr << "Argumentos incorrectos.";
        exit(1);
    }

    entrada.open(argv[1]);

    entrada >> n >> m;

    vector<vector<double>> matriz_dist(n, vector<double>(n, 0));
    vector<vector<bool>> cromosomas; // Todos los elementos del cromosoma son por defecto 0
    vector<double> fitness_cromosomas;
    vector<bool> cromosoma(n);
    vector<int> seleccionables(n);
    vector<int> seleccionables_original(n);
    vector<int> numeros_cromosoma;
    vector<bool> padre1_AGG;
    int indice_padre1_AGG;
    vector<bool> padre2_AGG;
    int indice_padre2_AGG;
    vector<bool> padre1_AGE;
    int indice_padre1_AGE;
    int indice2_padre1_AGE;
    vector<bool> padre2_AGE;
    int indice_padre2_AGE;
    int indice2_padre2_AGE;
    vector<vector<bool>> intermedia_AGG;
    vector<vector<bool>> hijos_AGG(NUM_CROMOSOMAS);
    vector<vector<bool>> hijos_AGG_anterior(NUM_CROMOSOMAS);
    vector<vector<bool>> hijos_AGE;
    vector<bool> hijo1_AGE;
    vector<bool> hijo2_AGE;
    vector<bool> hijo1_AGG;
    vector<bool> hijo2_AGG;
    vector<int> minimos;
    double fitness_hijo1, fitness_hijo2;
    int mutacion_posicion1, mutacion_posicion2;
    int semilla_cruce1, semilla_cruce2;

    for (int i = 0; i < seleccionables.size(); i++)
    {
        seleccionables[i] = i;
    }

    seleccionables_original = seleccionables;

    while (!entrada.eof())
    {
        entrada >> fila >> columna >> valor;
        matriz_dist[fila][columna] = valor;
    }

    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            if (i > j)
            {
                matriz_dist[i][j] = matriz_dist[j][i];
            }
        }
    }

    // ACTUALIZAMOS LA SEMILLA
    Random::seed(semilla);

    // Seleccionamos el pirmer nodo solución
    int primer_sel = Random::get(0, n - 1); // devuelve entero aleatorio on el numero de elemento
    int seleccionado = primer_sel;          // Si son iguales lo cambiamos
    seleccionables[primer_sel] = -1;

    numeros_cromosoma.push_back(seleccionado);

    //Se generan exactamente 50 cromosomas aleatorios
    while (cromosomas.size() < NUM_CROMOSOMAS)
    {

        while (numeros_cromosoma.size() < m)
        {

            while (seleccionables[seleccionado] == -1)
            {
                seleccionado = Random::get(0, n - 1);
            }

            seleccionables[seleccionado] = -1;
            numeros_cromosoma.push_back(seleccionado);
        }

        cromosoma = devolverBinario(numeros_cromosoma, n, m);
        cromosomas.push_back(cromosoma);
        cromosoma.clear();
        numeros_cromosoma.clear();
        seleccionables = seleccionables_original;
    }

    for (int i = 0; i < cromosomas.size(); i++)
    {
        fitness_cromosomas.push_back(nueva_dispersion(cromosomas[i], matriz_dist, n));
    }


    for (int i = 0; i < NUM_CROMOSOMAS; i++)
    {
        for (int j = 0; j < n; j++)
        {
            cout << cromosomas[i][j] << " ";
        }

        cout << endl;
    }

   


    int mutaciones_esperadas = (NUM_CROMOSOMAS * 0.1);

    vector<bool> cromosomas_seleccionados(NUM_CROMOSOMAS, false);
    vector<bool> cromosomas_seleccionados_anterior(NUM_CROMOSOMAS, false);
    vector<int> indices_seleccionables;
    vector<int> indices_seleccionables_anterior;
    vector<vector<bool>> hijos;
    int esperanza_cruce, esperanza_mutacion;
    int indice;
    int ev_suma = 0;

    for (int i = 0; i < NUM_CROMOSOMAS; i++)
    {
        indices_seleccionables.push_back(i);
        indices_seleccionables_anterior.push_back(i);
    }

    // Comenzamos a medir tiempo
    auto inicio = std::chrono::high_resolution_clock::now();

    switch (tipo_algoritmo)
    {
    case 1:
        //AGG_Uni
        esperanza_cruce = 0.7 * NUM_CROMOSOMAS;

        esperanza_mutacion = 0.1 * (NUM_CROMOSOMAS * n);

        int hijos_faltan;

        for (int i = 0; ev_suma < ITERACIONES; i++)
        {
            double minimo;
            int posicion_minimo;
            bool prim_ite = true;
            
            //Cogemos el mejor de la poblacion inicial
            for (int i = 0; i < fitness_cromosomas.size(); i++)
            {
                if (prim_ite)
                {
                    minimo = fitness_cromosomas[0];
                    posicion_minimo = i;
                    prim_ite = false;
                }

                if (fitness_cromosomas[i] < minimo)
                {
                    minimo = fitness_cromosomas[i];
                    posicion_minimo = i;
                }
            }
            
            //Aqui seleccionamos la poblacion intermedia con 50 torneos binarios, igual que el número de cromosomas
            for (int j = 0; j < NUM_CROMOSOMAS; j++)
            {

                Random::shuffle(indices_seleccionables);
                indice_padre1_AGG = indices_seleccionables.back();
                Random::shuffle(indices_seleccionables);
                indice_padre2_AGG = indices_seleccionables.back();
                
                indice = torneoBinario(indice_padre1_AGG, indice_padre2_AGG, fitness_cromosomas);
                intermedia_AGG.push_back(cromosomas[indice]);

                if (indice == indice_padre1_AGG)
                {
                    indices_seleccionables.push_back(indice_padre2_AGG);
                }

                else
                {
                    indices_seleccionables.push_back(indice_padre1_AGG);
                }

                indices_seleccionables = indices_seleccionables_anterior;
            }

            //Hacemos tantos cruces de seguido segun la esperanza calculada anteriormente
            for (int j = 0; j < esperanza_cruce; j += 2)
            {
                hijos.clear();
                
                hijo1_AGG = cruceUniforme(intermedia_AGG[j], intermedia_AGG[j+1], n, m, matriz_dist);
                
                hijos.push_back(hijo1_AGG);
                hijo2_AGG = cruceUniforme(intermedia_AGG[j], intermedia_AGG[j+1], n, m, matriz_dist);
                
                hijos.push_back(hijo2_AGG);
                hijos_AGG[j] = hijos[0];
                hijos_AGG[j + 1] = hijos[1];
            }

            hijos_faltan = NUM_CROMOSOMAS - esperanza_cruce;

            //Los hijos que nos faltan para completar la población los tomamos directamente de la población intermedia de forma aleatoria
            for (int j = hijos_faltan; j < NUM_CROMOSOMAS; j++)
            {
                int falta;
                falta = Random::get(0, NUM_CROMOSOMAS - 1);
                hijos_AGG[j] = intermedia_AGG[falta];
            }

            intermedia_AGG.clear();
            vector<int> a_mutar;
            int cromosoma_a_mutar;

            //Mutamos según la esperanza de mutación
            for (int j = 0; j < esperanza_mutacion; j++)
            {
                cromosoma_a_mutar = Random::get(0, NUM_CROMOSOMAS - 1); // Este sera el cromosoma que mutemos
                a_mutar.push_back(cromosoma_a_mutar);
                mutacion_posicion1 = Random::get(0, n - 1);
                mutacion_posicion2 = Random::get(0, n - 1);
                while (mutacion_posicion2 == mutacion_posicion1 || hijos_AGG[cromosoma_a_mutar][mutacion_posicion1] == hijos_AGG[cromosoma_a_mutar][mutacion_posicion2])
                {
                    mutacion_posicion2 = Random::get(0, n - 1);
                }
                mutacion(hijos_AGG[cromosoma_a_mutar], mutacion_posicion1, mutacion_posicion2, n, m);
            }

            // Actualizamos fitness
            for (int i = 0; i < hijos_AGG.size(); i++)
            {
                fitness_cromosomas[i] = nueva_dispersion(hijos_AGG[i], matriz_dist, n);
            }
            ev_suma += 50;


            //Obtenemos el peor hijo
            double maximo;
            int posicion_maximo;
            prim_ite = true;
            for (int i = 0; i < fitness_cromosomas.size(); i++)
            {
                if (prim_ite)
                {
                    maximo = fitness_cromosomas[0];
                    posicion_maximo = i;
                    prim_ite = false;
                }

                if (fitness_cromosomas[i] > maximo)
                {
                    maximo = fitness_cromosomas[i];
                    posicion_maximo = i;
                }
            }


            //Si el mejor padre es mejor que el peor hijo, los intercambiamos.
            if (minimo < maximo)
            {
                hijos_AGG[posicion_maximo] = cromosomas[posicion_minimo];
                fitness_cromosomas[posicion_maximo] = minimo;
            }

            cromosomas = hijos_AGG;
            
        }
        break;

    case 2:
        //AGG_Pos
        //Este caso es exactamente igual que el anterior solo que cambiando el operador de cruce uniforme
        //por el de cruce por posicion
        esperanza_cruce = 0.7 * NUM_CROMOSOMAS;

        esperanza_mutacion = 0.1 * (NUM_CROMOSOMAS * n);

        int hijos_faltantes;

        for (int i = 0; ev_suma < ITERACIONES; i++)
        {
            double minimo;
            int posicion_minimo;
            bool prim_ite = true;

            for (int i = 0; i < fitness_cromosomas.size(); i++)
            {
                if (prim_ite)
                {
                    minimo = fitness_cromosomas[0];
                    posicion_minimo = i;
                    prim_ite = false;
                }

                if (fitness_cromosomas[i] < minimo)
                {
                    minimo = fitness_cromosomas[i];
                    posicion_minimo = i;
                }
            }
            

            for (int j = 0; j < NUM_CROMOSOMAS; j++)
            {

                Random::shuffle(indices_seleccionables);
                indice_padre1_AGG = indices_seleccionables.back();
                Random::shuffle(indices_seleccionables);
                indice_padre2_AGG = indices_seleccionables.back();
                
                indice = torneoBinario(indice_padre1_AGG, indice_padre2_AGG, fitness_cromosomas);
                intermedia_AGG.push_back(cromosomas[indice]);

                if (indice == indice_padre1_AGG)
                {
                    indices_seleccionables.push_back(indice_padre2_AGG);
                }

                else
                {
                    indices_seleccionables.push_back(indice_padre1_AGG);
                }

                indices_seleccionables = indices_seleccionables_anterior;
            }

            for (int j = 0; j < esperanza_cruce; j += 2)
            {
                hijos = crucePosicion(intermedia_AGG[j], intermedia_AGG[j + 1], 0, 0, n);
                hijos_AGG[j] = hijos[0];
                hijos_AGG[j + 1] = hijos[1];
            }

            hijos_faltantes = NUM_CROMOSOMAS - esperanza_cruce;

            for (int j = hijos_faltantes; j < NUM_CROMOSOMAS; j++)
            {
                int falta;
                falta = Random::get(0, NUM_CROMOSOMAS - 1);
                hijos_AGG[j] = intermedia_AGG[falta];
            }

            intermedia_AGG.clear();
            vector<int> a_mutar;
            int cromosoma_a_mutar;
            for (int j = 0; j < esperanza_mutacion; j++)
            {
                cromosoma_a_mutar = Random::get(0, NUM_CROMOSOMAS - 1); // Este sera el cromosoma que mutemos
                a_mutar.push_back(cromosoma_a_mutar);
                mutacion_posicion1 = Random::get(0, n - 1);
                mutacion_posicion2 = Random::get(0, n - 1);
                while (mutacion_posicion2 == mutacion_posicion1 || hijos_AGG[cromosoma_a_mutar][mutacion_posicion1] == hijos_AGG[cromosoma_a_mutar][mutacion_posicion2])
                {
                    mutacion_posicion2 = Random::get(0, n - 1);
                }
                mutacion(hijos_AGG[cromosoma_a_mutar], mutacion_posicion1, mutacion_posicion2, n, m);
            }

            // Actualizamos fitness
            for (int i = 0; i < hijos_AGG.size(); i++)
            {
                fitness_cromosomas[i] = nueva_dispersion(hijos_AGG[i], matriz_dist, n);
            }
            ev_suma += 50;

            double maximo;
            int posicion_maximo;
            prim_ite = true;
            for (int i = 0; i < fitness_cromosomas.size(); i++)
            {
                if (prim_ite)
                {
                    maximo = fitness_cromosomas[0];
                    posicion_maximo = i;
                    prim_ite = false;
                }

                if (fitness_cromosomas[i] > maximo)
                {
                    maximo = fitness_cromosomas[i];
                    posicion_maximo = i;
                }
            }

            if (minimo < maximo)
            {
                hijos_AGG[posicion_maximo] = cromosomas[posicion_minimo];
                fitness_cromosomas[posicion_maximo] = minimo;
            }

            cromosomas = hijos_AGG;
            
        }
        break;

    case 3:
        //AGE_Uni
        for (int i = 0; ev_suma < ITERACIONES; i++)
        {

            //Escogemos dos padres diferentes
            indice_padre1_AGE = Random::get(0, NUM_CROMOSOMAS - 1);
            indice2_padre1_AGE = Random::get(0, NUM_CROMOSOMAS - 1);
            while (indice_padre1_AGE == indice2_padre1_AGE)
            {
                indice2_padre1_AGE = Random::get(0, NUM_CROMOSOMAS - 1);
            }
            padre1_AGE = cromosomas[torneoBinario(indice_padre1_AGE, indice2_padre1_AGE, fitness_cromosomas)];

            indice_padre2_AGE = Random::get(0, NUM_CROMOSOMAS - 1);
            while (indice_padre2_AGE == indice_padre1_AGE || indice_padre2_AGE == indice2_padre1_AGE)
            {
                indice_padre2_AGE = Random::get(0, NUM_CROMOSOMAS - 1);
            }
            indice2_padre2_AGE = Random::get(0, NUM_CROMOSOMAS - 1);
            while (indice_padre2_AGE == indice2_padre2_AGE || indice2_padre2_AGE == indice2_padre1_AGE || indice_padre2_AGE == indice2_padre2_AGE)
            {
                indice2_padre2_AGE = Random::get(0, NUM_CROMOSOMAS - 1);
            }
            padre2_AGE = cromosomas[torneoBinario(indice_padre2_AGE, indice2_padre2_AGE, fitness_cromosomas)];

            //Esta igualdad siempre se da, porque se cruzan siempre
            if (prob_cruce_AGE == PROB_CRUCE_AGE)
            {
                semilla_cruce1 = Random::get(0, n - 1);
                semilla_cruce2 = Random::get(0, n - 1);
                while (semilla_cruce2 == semilla_cruce1)
                {

                    semilla_cruce2 = Random::get(0, n - 1);
                }

                hijo1_AGE = cruceUniforme(padre1_AGE, padre2_AGE, n, m, matriz_dist);
                hijo2_AGE = cruceUniforme(padre1_AGE, padre2_AGE, n, m, matriz_dist);
                // Por aprovechar la estructura
                hijos_AGE.clear();
                hijos_AGE.push_back(hijo1_AGE);
                hijos_AGE.push_back(hijo2_AGE);
            }

            //Vemos si muta cada hijo generando un valor entre 0 y 1.
            for (int i = 0; i < hijos_AGE.size(); i++)
            {

                mutacion_valor = Random::get(0.0, 1.0);
                mutacion_posicion1 = Random::get(0, n - 1);
                mutacion_posicion2 = Random::get(0, n - 1);
                while (hijos_AGE[i][mutacion_posicion1] == hijos_AGE[i][mutacion_posicion2])
                {
                    mutacion_posicion1 = Random::get(0, n - 1);
                    mutacion_posicion2 = Random::get(0, n - 1);
                }

                if (mutacion_valor <= PROB_MUTACION)
                    mutacion(hijos_AGE[i], mutacion_posicion1, mutacion_posicion2, n, m);

            }

            minimos = posicionesMenosFitness(fitness_cromosomas);

            // Primer hijo
            fitness_hijo1 = nueva_dispersion(hijos_AGE[0], matriz_dist, n);
            fitness_hijo2 = nueva_dispersion(hijos_AGE[1], matriz_dist, n);
            ev_suma += 2;

            //Comparamos los dos nuevos fitness con los dos peores de la población para ver si se intercambia.
            //En caso de que los nuevos fitness sean mejores que los dos peores de la población, se sustituyen.
            //Igualmente si solo 1 es mejor que otro.
            if (fitness_cromosomas[minimos[0]] > fitness_hijo1)
            {
                cromosomas[minimos[0]] = hijos_AGE[0];
                fitness_cromosomas[minimos[0]] = fitness_hijo1;

                if (fitness_cromosomas[minimos[1]] > fitness_hijo2)
                {
                    cromosomas[minimos[1]] = hijos_AGE[1];
                    fitness_cromosomas[minimos[1]] = fitness_hijo2;
                }
            }

            else
            {
                if (fitness_cromosomas[minimos[1]] > fitness_hijo1)
                {
                    cromosomas[minimos[1]] = hijos_AGE[0];
                    fitness_cromosomas[minimos[1]] = fitness_hijo1;
                }

                if (fitness_cromosomas[minimos[0]] > fitness_hijo2)
                {
                    cromosomas[minimos[0]] = hijos_AGE[1];
                    fitness_cromosomas[minimos[0]] = fitness_hijo2;
                }
            }

        }

        break;
    
    case 4:
        //AGE_Pos
        //Exactamente igual que el caso AGE_Uni pero cambiando el cruce uniforme por el cruce por posición.
        for (int i = 0; ev_suma < ITERACIONES; i++)
        {
            indice_padre1_AGE = Random::get(0, NUM_CROMOSOMAS - 1);
            indice2_padre1_AGE = Random::get(0, NUM_CROMOSOMAS - 1);
            while (indice_padre1_AGE == indice2_padre1_AGE)
            {
                indice2_padre1_AGE = Random::get(0, NUM_CROMOSOMAS - 1);
            }
            padre1_AGE = cromosomas[torneoBinario(indice_padre1_AGE, indice2_padre1_AGE, fitness_cromosomas)];

            indice_padre2_AGE = Random::get(0, NUM_CROMOSOMAS - 1);
            while (indice_padre2_AGE == indice_padre1_AGE || indice_padre2_AGE == indice2_padre1_AGE)
            {
                indice_padre2_AGE = Random::get(0, NUM_CROMOSOMAS - 1);
            }
            indice2_padre2_AGE = Random::get(0, NUM_CROMOSOMAS - 1);
            while (indice_padre2_AGE == indice2_padre2_AGE || indice2_padre2_AGE == indice2_padre1_AGE || indice_padre2_AGE == indice2_padre2_AGE)
            {
                indice2_padre2_AGE = Random::get(0, NUM_CROMOSOMAS - 1);
            }
            padre2_AGE = cromosomas[torneoBinario(indice_padre2_AGE, indice2_padre2_AGE, fitness_cromosomas)];


            if (prob_cruce_AGE == PROB_CRUCE_AGE)
            {
                semilla_cruce1 = Random::get(0, n - 1);
                semilla_cruce2 = Random::get(0, n - 1);
                while (semilla_cruce2 == semilla_cruce1)
                {

                    semilla_cruce2 = Random::get(0, n - 1);
                }

                hijos_AGE = crucePosicion(padre1_AGE, padre2_AGE, semilla_cruce1, semilla_cruce2, n);
            }

            for (int i = 0; i < hijos_AGE.size(); i++)
            {
                mutacion_valor = Random::get(0.0, 1.0);
                mutacion_posicion1 = Random::get(0, n - 1);
                mutacion_posicion2 = Random::get(0, n - 1);
                while (mutacion_posicion2 == mutacion_posicion1 || hijos_AGE[i][mutacion_posicion1] == hijos_AGE[i][mutacion_posicion2])
                {
                    mutacion_posicion2 = Random::get(0, n - 1);
                }
                if (mutacion_valor <= PROB_MUTACION)
                    mutacion(hijos_AGE[i], mutacion_posicion1, mutacion_posicion2, n, m);
            }
            minimos = posicionesMenosFitness(fitness_cromosomas);

            // Primer hijo
            fitness_hijo1 = nueva_dispersion(hijos_AGE[0], matriz_dist, n);
            fitness_hijo2 = nueva_dispersion(hijos_AGE[1], matriz_dist, n);
            ev_suma += 2;

            if (fitness_cromosomas[minimos[0]] > fitness_hijo1)
            {
                cromosomas[minimos[0]] = hijos_AGE[0];
                fitness_cromosomas[minimos[0]] = fitness_hijo1;

                if (fitness_cromosomas[minimos[1]] > fitness_hijo2)
                {
                    cromosomas[minimos[1]] = hijos_AGE[1];
                    fitness_cromosomas[minimos[1]] = fitness_hijo2;
                }
            }

            else
            {
                if (fitness_cromosomas[minimos[1]] > fitness_hijo1)
                {
                    cromosomas[minimos[1]] = hijos_AGE[0];
                    fitness_cromosomas[minimos[1]] = fitness_hijo1;
                }

                if (fitness_cromosomas[minimos[0]] > fitness_hijo2)
                {
                    cromosomas[minimos[0]] = hijos_AGE[1];
                    fitness_cromosomas[minimos[0]] = fitness_hijo2;
                }
            }
        }
        break;

    default:
        break;
    }

    double minimo;
    int posicion_minimo;
    bool prim_ite = true;
    for (int i = 0; i < fitness_cromosomas.size(); i++)
    {
        if (prim_ite)
        {
            minimo = fitness_cromosomas[0];
            posicion_minimo = i;
            prim_ite = false;
        }

        if (fitness_cromosomas[i] < minimo)
        {
            minimo = fitness_cromosomas[i];
            posicion_minimo = i;
        }
    }

    //Terminamos de medir tiempo
    auto final = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> total = final - inicio;

    cout << endl
         << endl;
    for (int i = 0; i < cromosomas[posicion_minimo].size(); i++)
    {
        cout << cromosomas[posicion_minimo][i] << " ";
    }

    cout << endl;

    cout << endl
         << minimo << endl;
    
    cout << "El algoritmo tardó " << total.count() << " milisegundos en ejecutarse." << endl
         << "FIN DEL PROGRAMA" << endl;
    return 0;
}


double nueva_dispersion(vector<bool> cromosoma, vector<vector<double>> &distancias, int n)
{
    double maximo = 0, minimo = 0;
    double suma = 0;

    vector<int> elegidos;
    elegidos = devolverEntero(cromosoma, n); // El tamanio de la matriz distancias ya es n

    for (int i = 0; i < elegidos.size(); i++)
    {
        for (int j = 0; j < elegidos.size(); j++)
        {
            if (elegidos[i] != elegidos[j])
            {
                suma += distancias[elegidos[i]][elegidos[j]];
            }
        }

        if (suma > maximo)
        {
            maximo = suma;
        }

        if (minimo == 0)
        {
            minimo = suma;
        }
        else
        {
            if (suma < minimo)
            {
                minimo = suma;
            }
        }

        suma = 0;
    }

    return maximo - minimo;
}
vector<double> nueva_dispersion2(vector<bool> cromosoma, vector<vector<double>> &distancias, int n)
{
    double suma = 0;

    vector<int> elegidos;
    elegidos = devolverEntero(cromosoma, n); // El tamanio de la matriz distancias ya es n
    vector<double> solucion(elegidos.size());
    for (int i = 0; i < elegidos.size(); i++)
    {
        for (int j = 0; j < elegidos.size(); j++)
        {
            if (elegidos[i] != elegidos[j])
            {
                suma += distancias[elegidos[i]][elegidos[j]];
            }
        }

        solucion[i] = suma;

        suma = 0;
    }

    return solucion;
}

vector<bool> devolverBinario(vector<int> a_transformar, int n, int m)
{
    vector<bool> resultado(n, 0);

    for (int i = 0; i < m; i++)
    {
        resultado[a_transformar[i]] = 1;
    }

    return resultado;
}

vector<int> devolverEntero(vector<bool> a_transformar, int n)
{
    vector<int> resultado;

    for (int i = 0; i < n; i++)
    {
        if (a_transformar[i] == 1)
        {
            resultado.push_back(i);
        }
    }

    return resultado;
}

vector<vector<bool>> crucePosicion(vector<bool> padre1, vector<bool> padre2, int semilla1, int semilla2, int n)
{
    using Random = effolkronium::random_static;
    vector<bool> solucion(padre1.size());
    vector<bool> solucion_escritos(padre1.size(), false);
    vector<bool> restantes_padre2;
    vector<bool> hijo;
    vector<vector<bool>> hijos;
    bool continuar = true;
    int contador_hijos = 0;
    int valor;

    for (int i = 0; i < padre1.size(); i++)
    {
        if (padre1[i] == padre2[i])
        {
            solucion[i] = padre1[i];
            solucion_escritos[i] = true;
        }

        else
        {
            restantes_padre2.push_back(padre2[i]);
        }
    }

    vector<bool> solucion_anterior = solucion;
    vector<bool> solucion_escritos_anterior = solucion_escritos;

    int tamanio_restantes = restantes_padre2.size();

    while (contador_hijos < 2)
    {
        Random::shuffle(restantes_padre2);

        vector<bool> tomados_restantes(tamanio_restantes, false);

        for (int i = 0; i < restantes_padre2.size(); i++)
        {

            tomados_restantes[i] = true;
            hijo.push_back(restantes_padre2[i]);
        }
        
        for (int j = 0; j < hijo.size(); j++)
        {
            continuar = true;
            for (int i = 0; i < solucion.size() && continuar; i++)
            {
                if (solucion_escritos[i] == false)
                {
                    continuar = false;
                    solucion[i] = hijo[j];
                    solucion_escritos[i] = true;
                }
            }
        }

        hijos.push_back(solucion);
        contador_hijos++;
        hijo.clear();
        solucion = solucion_anterior;
        solucion_escritos = solucion_escritos_anterior;
    }

    return hijos;
}

//¿Devolver posicion para marcarla como no seleccionable?  ¿Cambiar semilla para cada iteracion ?
int torneoBinario(int cromosoma1, int cromosoma2, vector<double> distancias)
{

    if (distancias[cromosoma1] < distancias[cromosoma2])
    {
        return cromosoma1;
    }

    else
    {
        return cromosoma2;
    }
}

void mutacion(vector<bool> &cromosoma, int pos1, int pos2, int n, int m)
{
    bool intercambio = cromosoma[pos1];
    cromosoma[pos1] = cromosoma[pos2];
    cromosoma[pos2] = intercambio;
}

vector<int> posicionesMenosFitness(vector<double> lista_fitness)
{
    vector<int> retorno;
    double minimo = lista_fitness[0];
    int minima_pos = 0;
    double minimo2 = lista_fitness[0];
    int minima_pos2 = 0;
    bool primera_ite = true;
    for (int i = 0; i < lista_fitness.size(); i++)
    {
        if (lista_fitness[i] > minimo)
        {
            minimo = lista_fitness[i];
            minima_pos = i;
        }
    }

    for (int i = 0; i < lista_fitness.size(); i++)
    {
        if (i != minima_pos)
        {
            if (lista_fitness[i] > minimo2)
            {
                minimo2 = lista_fitness[i];
                minima_pos2 = i;
            }
        }
    }

    retorno.push_back(minima_pos);
    retorno.push_back(minima_pos2);

    return retorno;
}

vector<bool> cruceUniforme(vector<bool> padre1, vector<bool> padre2, int n, int m, vector<vector<double>> &matriz_dist)
{
    using Random = effolkronium::random_static;
    double prob_padre;
    int posicion;
    vector<bool> padre;
    vector<bool> solucion(n);
    vector<bool> solucion_escritos(padre1.size(), false);
    vector<bool> retorno;

    for (int i = 0; i < padre1.size(); i++)
    {
        if (padre1[i] == padre2[i])
        {
            solucion[i] = padre1[i];
            solucion_escritos[i] = true;
        }
    }

    for (int i = 0; i < solucion.size(); i++)
    {
        prob_padre = Random::get(0.0, 1.0);
        if (prob_padre < 0.5)
            padre = padre1;
        else
            padre = padre2;

        posicion = Random::get(0, n - 1);
        if (solucion_escritos[i] == false)
        {
            solucion_escritos[i] = true;
            solucion[i] = padre[posicion];
        }
    }

    retorno = reparador(solucion, n, m, matriz_dist); // Me quedo aqui

    return retorno;
}

vector<bool> reparador(vector<bool> a_reparar, int n, int m, vector<vector<double>> &matriz_dist)
{
    vector<bool> solucion(a_reparar.size(), 0);
    vector<double> solucion_sumas_orig;
    vector<double> solucion_sumas;
    int cuenta_de_unos = 0;
    bool first_ite = true;
    bool quitar = false;
    vector<int> posiciones;
    int unos_faltantes;

    // Primero contamos los 1 que hay en el a_reparar y los dejamos en la solucion
    for (int i = 0; i < a_reparar.size(); i++)
    {
        if (a_reparar[i] == 1)
        {
            posiciones.push_back(i);
            solucion[i] = 1;
            cuenta_de_unos += 1;
        }
        else
        {
            solucion[i] = 0;
        }
    }
    

    unos_faltantes = m - cuenta_de_unos; // Si negativo es que sobran 1s
    double media, media_orig;
    double suma = 0; 
    double suma_orig;
    double minima_anterior;
    double media_anterior;
    double minima, maximo;
    double maximo_anterior;
    
    int minima_pos;
    int maxima_pos;
    if (unos_faltantes == 0)
    {
        return a_reparar;
    }

    if (unos_faltantes > 0)
    {
        solucion_sumas_orig = nueva_dispersion2(a_reparar, matriz_dist, n);

        while (cuenta_de_unos < m)
        {
            first_ite = true;
            suma = 0;

            for (int i = 0; i < solucion_sumas_orig.size(); i++)
            {
                suma_orig += solucion_sumas_orig[i];
            }

            media_orig = suma_orig / solucion_sumas_orig.size();

            for (int i = 0; i < solucion.size(); i++)
            {

                if (solucion[i] == 0)
                {
                    for (int j = 0; j < posiciones.size(); j++){
                        suma += matriz_dist[i][posiciones[j]];
                    }
                    //for (int j = 0; j < solucion_sumas_orig.size(); j++)
                    //{
                        if (first_ite)
                        {
                            minima = fabs(suma - media_orig);
                            minima_pos = i;
                            first_ite = false;
                        }

                        if ((suma - media_orig) < minima)
                        {

                            minima_pos = i;
                            minima = fabs(suma - media_orig);
                        }
                        
                    //}
                }
            }

            solucion[minima_pos] = 1;
            posiciones.push_back(minima_pos);
            cuenta_de_unos += 1;
            solucion_sumas_orig = nueva_dispersion2(solucion, matriz_dist, n);
            

        }
        return solucion;
    }


    if (unos_faltantes < 0)
    {
        solucion_sumas_orig = nueva_dispersion2(a_reparar, matriz_dist, n);
        while (cuenta_de_unos > m)
        {
            suma = 0;
            first_ite = true;
            for (int i = 0; i < solucion_sumas_orig.size(); i++)
            {
                suma_orig += solucion_sumas_orig[i];
            }

            media_orig = suma_orig / solucion_sumas_orig.size();

            for (int i = 0; i < solucion.size(); i++)
            {

                if (solucion[i] == 1)
                {
                    for (int j = 0; j < posiciones.size(); j++){
                        suma += matriz_dist[i][posiciones[j]];
                    }
                    //for (int j = 0; j < solucion_sumas_orig.size(); j++)
                    //{
                        if (first_ite)
                        {
                            maximo = fabs(suma - media_orig);
                            maxima_pos = i;
                            first_ite = false;
                        }
                        if (fabs(suma - media_orig) > maximo)
                        {

                            maxima_pos = i;
                            maximo = fabs(suma - media_orig);
                        }
                    //}
                }
            }

            solucion[maxima_pos] = 0;
            int eliminar;
            for (int i = 0; i < posiciones.size(); i++){
                if (posiciones[i] == maxima_pos){
                    eliminar = i;
                }
            }
            auto borrar = posiciones.begin() + eliminar;
            posiciones.erase(borrar);
            cuenta_de_unos -= 1;
            solucion_sumas_orig = nueva_dispersion2(solucion, matriz_dist, n);

        }
        return solucion;
    }

}
