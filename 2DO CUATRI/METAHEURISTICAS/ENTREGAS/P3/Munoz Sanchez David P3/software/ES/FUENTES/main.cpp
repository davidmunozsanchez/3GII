/*
 * BMB
 * File:   main.cpp
 * Author: davidms
 *
 * Created on 3 de abril de 2022
 */

#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <chrono>
#include "./random.hpp"

using namespace std;

#define EVALUACIONES 100000
#define NUM_ITERACIONES_BL 10

/**
 * @brief Método encargado de calcular la dispersión entre los puntos elegidos.
 *
 * @param elegidos Vector de elegidos hasta el momento
 * @param distancias Matriz con las distancias entre puntos
 * @return vector de distancias acumuladas
 */
vector<double> distancias(vector<int> &elegidos, vector<vector<double>> &distancias);
/**
 * @brief Método encargado de calcular un vecino
 *
 * @param solucion_actual
 * @param i Elemento a meter
 * @param j Elemento a sacar
 * @param total_elementos n
 * @return vecino a partir de solución actual
 */
vector<int> genera_vecino(const vector<int> &solucion_actual, int i, int j, int total_elementos, const vector<int> &seleccionables);
/**
 * @brief Método encargado de devolver el índice diferente entre dos vecinos
 *
 * @param solucion_actual
 * @param solucion_vecina
 * @return Entero correspondiente a la posición donde difieren ambas soluciones
 */
int indice_cambio(vector<int> solucion_actual, vector<int> solucion_vecina);
/**
 * @brief Método encargado de calcular la dispersión entre los puntos elegidos.
 *
 * @param elegidos Vector de elegidos hasta el momento
 * @param distancias Matriz con las distancias entre puntos
 * @return double dispersión
 */
double nueva_dispersion(vector<int> &elegidos, vector<vector<double>> &distancias);
/**
 * @brief Método encargado de calcular la dispersión entre los puntos elegidos.
 *
 * @param solucion_actual
 * @param solucion_vecina
 * @param distancias_acumuladas Vector de las distancias acumuladas de la solucion actual
 * @param indice_cambio
 * @param matriz_distancias
 * @return double Dispersión en el vector elegidos
 */
double fitness_factorizado(vector<int> solucion_actual, vector<int> solucion_vecina, vector<double> &distancias_acumuladas, int indice_cambio, vector<vector<double>> &matriz_distancias);
/**
 * @brief Método encargado de calcular el enfriamiento
 *
 * @param temperatura Actual
 * @param beta
 */
void t_scheduler(double &temperatura, double beta);
/**
 * @brief
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
    int fila, columna;
    double valor;
    int m, n;
    double min_disp, new_disp;
    int suma;
    int elegido = -1;
    double resto, resto_anterior;
    int indice_borrar, indice;
    bool first_ite = true;
    int seleccionado, seleccionado_anterior;
    double mejor_fitness, fitness_inicial, fitness_maximo;
    int contador_soluciones = 0;

    /*Hay que introducir como argumento el nombre del fichero
      con los datos de la matriz de distancias (la diagonal superior) y la semilla a usar,
      que será un número entre 1 y 5
      Los dos primeros datos de los ficheros proporcionados son la n (la matriz será
      nxn y la m, el subconjunto con el que trabajaremos */

    semilla = atoi(argv[2]); // pasamos char* a int
    if (argc != 3)           // 3 argumentos, el programa, el fichero a leer y la semilla para inicializar la aleatoriedad
    {
        cerr << "Argumentos incorrectos.";
        exit(1);
    }

    entrada.open(argv[1]);

    entrada >> n >> m;

    vector<vector<double>> matriz_dist(n, vector<double>(n, 0));
    vector<int> solucion_inicial;
    vector<vector<int>> soluciones_iniciales;
    vector<int> solucion_actual;
    vector<int> mejor_solucion;
    vector<int> solucion_vecina;
    vector<int> seleccionables(n);
    vector<int> seleccionables_anterior;
    vector<int> seleccionables_nuevos;
    vector<int> seleccionables_quitados;
    vector<double> distancias_acumuladas;

    for (int i = 0; i < n; i++)
    {
        seleccionables[i] = i;
    }

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
    Random::shuffle(seleccionables);
    // Seleccionamos el pirmer nodo solución
    auto primer_sel = seleccionables.back(); // devuelve entero aleatorio on el numero de elemento
    seleccionables.pop_back();               //Quitamos el elemento que hemos cogido de seleccionables
    solucion_inicial.push_back(primer_sel);  // el primer seleccionado va a las soluciones
    solucion_actual.push_back(primer_sel);
    mejor_solucion.push_back(primer_sel);

    while (solucion_inicial.size() < m)
    {
        Random::shuffle(seleccionables);
        auto sel = seleccionables.back();
        seleccionables.pop_back();
        solucion_inicial.push_back(sel);
        solucion_actual.push_back(sel);
        mejor_solucion.push_back(sel);
    }
    //Generamos solucion inicial totalmente aleatoria
    //Al principio, solucion actual y mejor solucion se corresponden con la solucion inicial
    mejor_fitness = nueva_dispersion(mejor_solucion, matriz_dist); //Este fitness también es el de la solución inicial

    //Generamos temperatura inicial
    double t_inicial = (0.3 * mejor_fitness) / (-1.0 * log(0.3));
    double t_final = 0.001;
    double temperatura = t_inicial;
    


    distancias_acumuladas = distancias(solucion_actual, matriz_dist);

    //Comenzamos a medir tiempo
    auto inicio = std::chrono::high_resolution_clock::now();
    if (t_inicial != 0)
    {

        //La t_inicial no puede ser menor a la final nunca
        while (t_inicial < t_final){
            t_final /= 100;
        }

        int max_vecinos = 10 * n;
        int max_exitos = 0.1 * max_vecinos;
        int exitos;
        double beta = (t_inicial - t_final) / ((EVALUACIONES / max_vecinos) * t_final * t_inicial);

        double fitness_original = mejor_fitness;
        vector<double> distancias_originales;

        int cuenta_ev = 0;
        do
        {
            exitos = 0;
            
            distancias_originales = distancias_acumuladas;

            for (int i = 0; i < max_vecinos and exitos < max_exitos; i++)
            {

                Random::shuffle(seleccionables);
                auto elemento2 = seleccionables.back();
                auto indice1 = Random::get(0, m - 1);
                auto elemento1 = solucion_actual[indice1];

                solucion_vecina = genera_vecino(solucion_actual, elemento1, elemento2, n, seleccionables);

                auto indice = indice_cambio(solucion_actual, solucion_vecina);
                auto fitness_nuevo = fitness_factorizado(solucion_actual, solucion_vecina, distancias_acumuladas, indice, matriz_dist);
                cuenta_ev += 1;

                auto variacion_fitness = fitness_nuevo - fitness_original;

                auto valor_random = Random::get(0.0, 1.0);
                double valor_prob = exp((-1.0 * variacion_fitness) / temperatura);

                if (variacion_fitness < 0 or valor_random <= valor_prob)
                {

                    exitos += 1;
                    solucion_actual = solucion_vecina;
                    seleccionables.pop_back();
                    seleccionables.push_back(elemento1);
                    fitness_original = fitness_nuevo;
                    distancias_originales = distancias_acumuladas;

                    if (fitness_nuevo < mejor_fitness)
                    {
                        mejor_solucion = solucion_actual;
                        mejor_fitness = fitness_nuevo;
                    }
                }
                else
                {
                    distancias_acumuladas = distancias_originales;
                }
            }

            t_scheduler(temperatura, beta);

        } while (cuenta_ev < EVALUACIONES and exitos != 0);
    }

    auto final = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> total = final - inicio;

    cout << "El mejor fitness encontrado es " << mejor_fitness << endl;
    cout << "Para la solucion: " << endl;

    for (int i = 0; i < mejor_solucion.size(); i++)
    {
        cout << mejor_solucion[i] << " ";
    }
    cout << endl;
    cout << "El algoritmo tardó " << total.count() << " milisegundos en ejecutarse." << endl
         << "FIN DEL PROGRAMA" << endl;

    return 0;

    /*cout << "Problema: " << argv[1] << " con semilla " << argv[2] << endl
         << endl;

    cout << "La solucion inicial, con valor de fitness : " << fitness_inicial << endl;
    for (int i = 0; i < m; i++)
        cout << solucion_inicial[i] << " ";
    cout << endl;
    cout << "La solucion final tiene fitness: " << mejor_fitness << endl
         << endl;
    for (int i = 0; i < m; i++)
        cout << mejor_solucion[i] << " ";

    cout << endl
         << endl;
    cout << "El algoritmo tardó " << total.count() << " milisegundos en ejecutarse." << endl
         << "FIN DEL PROGRAMA" << endl;*/
}

/**
 * @brief Metodo encargado de calcular la dispersión dado un conjunto de valores y las distancias entre ellos
 *
 */
vector<double> distancias(vector<int> &elegidos, vector<vector<double>> &distancias)
{
    double suma = 0;
    vector<double> resultado_distancias;

    for (int i = 0; i < elegidos.size(); i++)
    {
        for (int j = 0; j < elegidos.size(); j++)
        {
            if (elegidos[i] != elegidos[j])
            {
                suma += distancias[elegidos[i]][elegidos[j]];
            }
        }

        resultado_distancias.push_back(suma);

        suma = 0;
    }

    return resultado_distancias;
}

vector<int> genera_vecino(const vector<int> &solucion_actual, int i, int j, int total_elementos, const vector<int> &seleccionables)
{
    //sumar numero aleatorio mejor
    vector<int> resultado(solucion_actual);
    static int times = 1;
    int pos_found = -1;

    for (int p = 0; p < resultado.size() && pos_found < 0; p++)
    {
        if (resultado[p] == i)
        {
            pos_found = p;
        }
    }

    if (pos_found < 0)
    {
        cout << "Debug: " << times << endl;
    }
    assert(pos_found >= 0);
    resultado[pos_found] = j;
    times++;
    return resultado;
}

int indice_cambio(vector<int> solucion_actual, vector<int> solucion_vecina)
{
    //Ambos vectores tendrán la misma longitud
    int tamanio = solucion_actual.size();
    int retorno = -1;
    //Comparamos par a par los vectores
    for (int i = 0; i < tamanio; i++)
    {
        if (solucion_actual[i] != solucion_vecina[i])
            retorno = i;
    }
    return retorno;
}

double fitness_factorizado(vector<int> solucion_actual, vector<int> solucion_vecina, vector<double> &distancias_acumuladas, int indice_cambio, vector<vector<double>> &matriz_distancias)
{
    //vector<int> solucion;
    //vector<double> distancias_acumuladas_nuevas(distancias_acumuladas);
    double suma = 0;
    double maximo_nuevo = 0, minimo_nuevo = 0;
    //double maximo_anterior = 0, minimo_anterior = 0;

    //solucion = solucion_actual;
    for (int i = 0; i < distancias_acumuladas.size(); i++)
    {
        if (i != indice_cambio)
            distancias_acumuladas[i] -= matriz_distancias[solucion_actual[indice_cambio]][solucion_actual[i]];
    }

    for (int i = 0; i < distancias_acumuladas.size(); i++)
    {
        if (i != indice_cambio)
        {
            //cout << solucion_vecina[indice_cambio] << endl;
            //cout << "Hola" << endl;
            distancias_acumuladas[i] += matriz_distancias[solucion_vecina[indice_cambio]][solucion_actual[i]];

            suma += matriz_distancias[solucion_vecina[indice_cambio]][solucion_actual[i]];
        }
    }

    distancias_acumuladas.at(indice_cambio) = suma;

    for (int i = 0; i < distancias_acumuladas.size(); i++)
    {
        double distancia = distancias_acumuladas[i];
        if (distancia > maximo_nuevo)
        {
            maximo_nuevo = distancia;
        }

        if (minimo_nuevo == 0)
        {
            minimo_nuevo = distancia;
        }

        else
        {
            if (distancia < minimo_nuevo)
            {
                minimo_nuevo = distancia;
            }
        }
    }
    return (maximo_nuevo - minimo_nuevo);
}

double nueva_dispersion(vector<int> &elegidos, vector<vector<double>> &distancias)
{
    double maximo = 0, minimo = 0;
    double suma = 0;

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


void t_scheduler(double &temperatura, double beta)
{
    auto nueva_temperatura = temperatura / (1 + beta * temperatura);

    temperatura = nueva_temperatura;
}
