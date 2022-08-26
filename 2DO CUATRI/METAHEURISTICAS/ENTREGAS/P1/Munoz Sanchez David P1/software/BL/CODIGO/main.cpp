/*
 * File:   main.cpp
 * Author: davidms
 *
 * Created on 1 de abril de 2022
 */

#include <iostream>
#include <fstream>
#include <vector>
#include <chrono>
#include "./random.hpp"

using namespace std;

/**
 * @brief Método encargado de calcular la dispersión entre los puntos elegidos.
 *
 * @param elegidos Vector de elegidos hasta el momento
 * @param distancias Matriz con las distancias entre puntos
 * @return double Dispersión en el vector elegidos
 */
vector<double> distancias(vector<int> &elegidos, vector<vector<double>> &distancias, int m);

/**
 * @brief Método encargado de generar un vecino.
 *
 * @return Vecino a la solucion actual
 */
vector<int> genera_vecino(vector<int> solucion_actual, int iteracion, int total_elementos, vector<int> &seleccionables);

/**
 * @brief Método que se encarga de comparar vectores iguales que se diferencien solo en una de sus componentes
 *
 * @return Índice donde difieren ambos vectores
 */
int indice_cambio(vector<int> solucion_actual, vector<int> solucion_vecina, int m);

/**
 * @brief Método que se encarga de calcular la dispersión de un conjunto de elementos
 *
 * @return Devuelve la dispersión de un conjunto de elementos
 */
double nueva_dispersion(vector<int> &elegidos, vector<vector<double>> &distancias, int m);

/**
 * @brief Método que se encarga de calcular la dispersión de un conjunto de elementos pero factorizando
 *
 * @return Devuelve la dispersión de un conjunto de elementos
 */
double fitness_factorizado(vector<int> solucion_actual, vector<int> solucion_vecina, vector<double> &distancias_acumuladas, int indice_cambio, vector<vector<double>> matriz_distancias, int m);

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
    double mejor_fitness, fitness_inicial;

    /*Hay que introducir como argumento el nombre del fichero
      con los datos de la matriz de distancias (la diagonal superior) y la semilla a usar,
      que será un número entre 1 y 5
      Los dos primeros datos de los ficheros proporcionados son la n (la matriz será
      nxn y la m, el subconjunto con el que trabajaremos */

    semilla = atoi(argv[2]);                     // pasamos char* a int
    if (argc != 3 || semilla < 1 || semilla > 5) // 3 argumentos, el programa, el fichero a leer y la semilla para inicializar la aleatoriedad
    {
        cerr << "Argumentos incorrectos.";
        exit(1);
    }

    entrada.open(argv[1]);

    entrada >> n >> m;

    vector<vector<double>> matriz_dist(n, vector<double>(n, 0));
    vector<int> solucion_inicial;
    vector<int> solucion_actual;
    vector<int> mejor_solucion;
    vector<int> solucion_vecina;
    vector<int> seleccionables(n);
    vector<double> distancias_acumuladas;

    

    for (int i = 0; i < n; i++)
    {
        seleccionables.at(i) = i;
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

    // Seleccionamos el pirmer nodo soluciónvector::_M_range_check: __n (which is 10
    auto primer_sel = Random::get(0, n - 1); // devuelve entero aleatorio on el numero de elemento
    solucion_inicial.push_back(primer_sel);  // el primer seleccionado va a la solucion
    // La elección del segundo nodo no es determinante dado que dejará diff a 0.
    auto segundo_sel = Random::get(0, n - 1);
    //Asignamos para poder rellenar con el bucle aleatoriamente nuestra primera solución
    seleccionado = segundo_sel;
    seleccionado_anterior = primer_sel;

    //Generamos solucion inicial totalmente aleatoria
    while (seleccionado == seleccionado_anterior || solucion_inicial.size() < m)
    {
        seleccionado = Random::get(0, n - 1); // Si son iguales lo cambiamos
        if (seleccionado != seleccionado_anterior && seleccionables.at(seleccionado) != -1)
        {
            solucion_inicial.push_back(seleccionado);
            solucion_actual.push_back(seleccionado);
            mejor_solucion.push_back(seleccionado);
            seleccionado_anterior = seleccionado;
            seleccionables.at(seleccionado) = -1;
        }
    }
    
    mejor_fitness = nueva_dispersion(mejor_solucion, matriz_dist, m);
    distancias_acumuladas = distancias(solucion_actual, matriz_dist, m);
    distancias_acumuladas.resize(m);
    fitness_inicial = mejor_fitness;

    // Rellenamos vectores de elegidos y solucion con los dos nodos que ya tenemos
    //elegidos.push_back(primer_sel);
    //solucion.push_back(segundo_sel);
    //elegidos.push_back(segundo_sel);

    //seleccionables[primer_sel] = -1;
    //seleccionables[segundo_sel] = -1; // Fuera de nuestros seleccionables

    //Comenzamos a medir tiempo
    auto inicio = std::chrono::high_resolution_clock::now();
    /*while (solucion.size() < m)
    {
        
    }*/

    //Una iteracion por cada elemento de la solucion actual, criterio de parada
    
    vector<double> distancias_acumuladas_anterior;

    for (int i = 0; i < m; i++)
    {
        bool first_ite = true;
        for (int j = 0; j < n; j++)
        {
            solucion_vecina = genera_vecino(solucion_actual, i, n, seleccionables);
            solucion_vecina.resize(m);
            solucion_actual = solucion_vecina;
            solucion_actual.resize(m);
            
            if (first_ite){
                distancias_acumuladas_anterior = distancias_acumuladas;
                distancias_acumuladas_anterior.resize(m);
                first_ite = false;
            }
            indice = indice_cambio(solucion_actual, solucion_vecina, m);

            double nueva_dispersion = fitness_factorizado(solucion_actual, solucion_vecina, distancias_acumuladas, indice, matriz_dist, m);
            if (nueva_dispersion < mejor_fitness)
            {
                mejor_solucion = solucion_actual;
                mejor_fitness = nueva_dispersion;
                distancias_acumuladas = distancias_acumuladas_anterior;
            }
            else
            {
                distancias_acumuladas_anterior = distancias_acumuladas;
            }
        }
    }

    auto final = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> total = final - inicio;
    
    cout << "Problema: " << argv[1] << " con semilla " << argv[2] << endl
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
         << "FIN DEL PROGRAMA" << endl;
    return 0;
}

/**
 * @brief Metodo encargado de calcular la dispersión dado un conjunto de valores y las distancias entre ellos
 *
 */
vector<double> distancias(vector<int> &elegidos, vector<vector<double>> &distancias, int m)
{
    double suma = 0;
    vector<double> resultado_distancias;

    for (int i = 0; i < m; i++)
    {
        for (int j = 0; j < m; j++)
        {
            if (elegidos.at(i) != elegidos.at(j))
            {
                suma += distancias[elegidos[i]][elegidos[j]];
            }
        }

        resultado_distancias.push_back(suma);

        suma = 0;
    }

    return resultado_distancias;
}

vector<int> genera_vecino(vector<int> solucion_actual, int iteracion, int total_elementos, vector<int> &seleccionables)
{
    //sumar numero aleatorio mejor
    vector<int> resultado;
    resultado = solucion_actual;
    using Random = effolkronium::random_static;
    auto primer_sel = Random::get(0, 1000);
    //aceptacion
    
    while ((resultado[iteracion] == (resultado[iteracion] + primer_sel) % total_elementos) || seleccionables[primer_sel] == -1)
    {
        primer_sel = Random::get(0, 1000);
    }

    seleccionables[solucion_actual[iteracion]] = solucion_actual[iteracion];

    resultado[iteracion] = ((resultado[iteracion] + primer_sel) % total_elementos);

    seleccionables[resultado[iteracion]] = -1;

    return resultado;
}

int indice_cambio(vector<int> solucion_actual, vector<int> solucion_vecina, int m)
{
    //Ambos vectores tendrán la misma longitud
    int devolver;

    //Comparamos par a par los vectores
    for (int i = 0; i < m; i++)
    {
        if (solucion_actual[i] != solucion_actual[i])
            devolver = i;
    }

    return devolver;
}

double fitness_factorizado(vector<int> solucion_actual, vector<int> solucion_vecina, vector<double> &distancias_acumuladas, int indice_cambio, vector<vector<double>> matriz_distancias, int m)
{
    vector<int> solucion;
    //vector<double> distancias_acumuladas_nuevas(distancias_acumuladas);
    double suma;
    double maximo_nuevo = 0, minimo_nuevo = 0;
    //double maximo_anterior = 0, minimo_anterior = 0;

    solucion = solucion_actual;

    for (int i = 0; i < m; i++)
    {
        if (i != indice_cambio)
            distancias_acumuladas[i] -= matriz_distancias[solucion_actual[indice_cambio]][solucion_actual[i]];
    }

    for (int i = 0; i < m; i++)
    {
        if (i != indice_cambio)
        {
            distancias_acumuladas[i] += matriz_distancias[solucion_vecina[indice_cambio]][solucion_vecina[i]];
            suma += distancias_acumuladas[i]; //matriz_distancias[solucion_vecina[indice_cambio]][solucion_vecina[i]];
        }
    }

    distancias_acumuladas.at(indice_cambio) = suma;

    for (int i = 0; i < m; i++)
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

double nueva_dispersion(vector<int> &elegidos, vector<vector<double>> &distancias, int m)
{
    double maximo = 0, minimo = 0;
    double suma = 0;

    for (int i = 0; i < m; i++)
    {
        for (int j = 0; j < m; j++)
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

