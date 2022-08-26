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

#define ITERACIONES 100000
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
 * @return double Dispersión en el vector elegidos
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
 * @brief Método encargado de mutar 0.3 * m posiciones de un vector dado
 *
 * @param solucion_actual
 * @param n
 * @param m
 * @return vector mutado
 */
vector<int> mutacion(const vector<int> &solucion_actual, int n, int m);
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
    vector<int> solucion_maxima;
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

   
    mejor_fitness = nueva_dispersion(mejor_solucion, matriz_dist);

    
    //Comenzamos a medir tiempo
    auto inicio = std::chrono::high_resolution_clock::now();

    //Una iteracion por cada elemento de la solucion actual, criterio de parada
    bool primera = true;

    for (int i = 0; i < NUM_ITERACIONES_BL; i++)
    {
        vector<int> solucion_original;
        //vector<int> solucion_maxima;

        double fitness_original;
        solucion_original = solucion_actual;
        auto solucion_actual_mutada = mutacion(solucion_actual, n, m);
        fitness_original = nueva_dispersion(solucion_actual, matriz_dist);

        int pos, pos2;
        int cuenta_ev = 0;
        bool mejor = true;
        bool salir = false;

        vector<int> soluciones;
        vector<int> soluciones_anterior;
        vector<pair<int, int>> vecinos;
        vector<double> distancias_nuevas(m);
        //Random::shuffle(soluciones);

        bool stuck = true;
        if (first_ite)
        {
            solucion_actual = solucion_inicial;
            first_ite = false;
        }
        else
        {
            solucion_actual = solucion_actual_mutada;
        }

        //De la segunda iteracion en adelante hacemos la búsqueda local sobre la mutada

        mejor_fitness = nueva_dispersion(solucion_actual, matriz_dist);
        distancias_acumuladas = distancias(solucion_actual, matriz_dist);

        //Búsqueda local
        do
        {

            vecinos.clear();
            soluciones.clear();
            mejor = false;

            for (int i = 0; i < solucion_actual.size(); i++)
            {
                soluciones.push_back(solucion_actual[i]);
            }

            vector<int> sin_asignar;
            sin_asignar.reserve(n - m);

            for (int i = 0; i < n; i++)
            {
                if (find(soluciones.begin(), soluciones.end(), i) == soluciones.end())
                {
                    sin_asignar.push_back(i);
                }
            }

            for (const auto &val : soluciones)
            {

                for (const auto &newval : sin_asignar)
                {
                    vecinos.push_back(pair<int, int>(val, newval));
                }
            }

            Random::shuffle(vecinos);

            vector<double> distancias_originales = distancias_acumuladas;
            double nueva_dispersion;
            double improved = false;
            stuck = true;

            for (int i = 0; i < vecinos.size() && !improved; i++)
            {

                const auto &cambio = vecinos[i];
                assert(cambio.first != cambio.second);
                solucion_vecina = genera_vecino(solucion_actual, cambio.first, cambio.second, n, seleccionables);
                indice = indice_cambio(solucion_actual, solucion_vecina);
                nueva_dispersion = fitness_factorizado(solucion_actual, solucion_vecina, distancias_acumuladas, indice, matriz_dist);
                cuenta_ev += 1;
                if (nueva_dispersion < mejor_fitness)
                {
                    improved = true;
                    stuck = false;
                    mejor_fitness = nueva_dispersion;
                    solucion_actual = solucion_vecina;
                }
                else
                {
                    distancias_acumuladas = distancias_originales;
                }
            }

            //Si no tenemos mejora
        } while (!stuck && cuenta_ev <= 100'000);

        if (primera)
        {
            mejor_solucion = solucion_actual;
            solucion_maxima = mejor_solucion;
            fitness_maximo = mejor_fitness;
            primera = false;
        }
        else
        {
            //Elegimos entre la actual (la mutada a la que se le ha aplicado BL) y la original
            if (mejor_fitness < fitness_original)
            {
                mejor_solucion = solucion_actual;
                //fitness_maximo = mejor_fitness;
            }

            else
            {
                solucion_actual = solucion_original;
                mejor_fitness = fitness_original;
            }

            //Actualizamos el mejor fitness encontrado y la mejor solucion
            if (mejor_fitness < fitness_maximo)
            {
                solucion_maxima = solucion_actual;
                fitness_maximo = mejor_fitness;
            }
        }
    }
    auto final = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> total = final - inicio;

    cout << "El mejor fitness encontrado es " << fitness_maximo << endl;
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

vector<int> mutacion(const vector<int> &solucion_actual, int n, int m)
{
    using Random = effolkronium::random_static;
    vector<bool> seleccionables(n, true);
    vector<bool> indices_seleccionables(m, true);
    vector<int> indices_seleccionados;
    vector<int> resultado;

    resultado = solucion_actual;
    const int a_seleccionar = 0.3 * m;

    for (int i = 0; i < solucion_actual.size(); i++)
    {
        //No podemos seleccionar la solucion actual
        seleccionables[solucion_actual[i]] = false;
    }

    for (int i = 0; i < a_seleccionar; i++)
    {
        auto indice = Random::get(0, m - 1);
        if (indices_seleccionables[indice] == true)
        {
            indices_seleccionados.push_back(indice);
            indices_seleccionables[indice] == false;
        }
    }

    for (int i = 0; i < indices_seleccionados.size(); i++)
    {
        auto primer_elemento = indices_seleccionados[i];
        auto elemento_a_meter = Random::get(0, n - 1);
        while (seleccionables[elemento_a_meter] == false)
        {
            elemento_a_meter = Random::get(0, n - 1);
        }

        auto traslado = resultado[primer_elemento];
        resultado[primer_elemento] = elemento_a_meter;
        seleccionables[elemento_a_meter] = false;
        seleccionables[traslado] = true;
    }

    return resultado;
}
