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

using namespace std;

/**
 * @brief Método encargado de calcular la dispersión entre los puntos elegidos.
 *
 * @param elegidos Vector de elegidos hasta el momento
 * @param distancias Matriz con las distancias entre puntos
 * @return double Dispersión en el vector elegidos
 */
double nueva_dispersion(vector<int> &elegidos, vector<vector<double>> &distancias);

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
    int indice_borrar;
    bool first_ite = true;

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
    vector<int> solucion;
    vector<int> elegidos;
    vector<int> seleccionables(n);

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

    // Seleccionamos el pirmer nodo solución
    auto primer_sel = Random::get(0, n - 1); // devuelve entero aleatorio on el numero de elemento
    solucion.push_back(primer_sel);          // el primer seleccionado va a la solucion
    // La elección del segundo nodo no es determinante dado que dejará diff a 0.
    auto segundo_sel = Random::get(0, n - 1);

    while (segundo_sel == primer_sel)
    {
        segundo_sel = Random::get(0, n - 1); // Si son iguales lo cambiamos
    }

    // Rellenamos vectores de elegidos y solucion con los dos nodos que ya tenemos
    elegidos.push_back(primer_sel);
    solucion.push_back(segundo_sel);
    elegidos.push_back(segundo_sel);

    seleccionables[primer_sel] = -1;
    seleccionables[segundo_sel] = -1; // Fuera de nuestros seleccionables

    //Comenzamos a medir tiempo
    auto inicio = std::chrono::high_resolution_clock::now();
    while (solucion.size() < m)
    {
        min_disp = nueva_dispersion(solucion, matriz_dist); // siempre dará 0 en la primera iteración
        for (int i = 0; i < n; i++)
        {  
            if (seleccionables[i] != -1)
            { //Ver bien considerando siempre min_disp como nueva_dispersion(solucion)
                elegidos.push_back(seleccionables[i]);
                new_disp = nueva_dispersion(elegidos, matriz_dist);
                resto = new_disp - min_disp;

                if (first_ite)
                { //Estamos en la primera iteración del for
                    resto_anterior = resto;
                    elegido = seleccionables[i];
                    indice_borrar = i;
                    first_ite = false;
                }

                if (resto < resto_anterior || new_disp < min_disp)
                {
                    elegido = seleccionables[i];
                    indice_borrar = i;
                    resto_anterior = resto;
                    //min_disp = nueva_dispersion(elegidos, matriz_dist);
                }


                elegidos.pop_back(); //Quitamos el que acabamos de meter
            }
        }

        solucion.push_back(elegido);
        elegidos.push_back(elegido);
        seleccionables[indice_borrar] = -1;
        first_ite = true;
    }

    auto final = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> total = final - inicio;
    cout << "Problema: " << argv[1] << " con semilla " << argv[2] << endl
         << endl;
    ;
    cout << "La dispersión mínima es: " << min_disp << endl
         << endl;
    ;
    cout << "La solución para los nodos iniciales: " << primer_sel << " " << segundo_sel << " es" << endl
         << endl;
    for (int i = 0; i < solucion.size(); i++)
        cout << solucion[i] << " ";

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
