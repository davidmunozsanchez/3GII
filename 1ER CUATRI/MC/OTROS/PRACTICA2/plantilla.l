%option noyywrap


%{
#include <iostream>
#include <fstream> //Para manejar ficheros E/S
#include <list>
#include <cassert>

using namespace std;

ifstream entrada;
ofstream salida;

list<string> le, ld, lt;

void mostrar_solucion (list<string> le, list<string> ld, list<string> lt );
%}
    
email         [a-zA-Z0-9][_a-zA-Z0-9]*@(gmail.com|hotmail.com|correo.ugr.es|ugr.es|outlook.com|yahoo.com)   
dni                 [0-9]{8}[A-Z]
tfno                [67][0-9]{8}    

%%

.                               {cout << "No reconocido" << endl;}
{dni}                           {ld.push_back(YYText()); cout << "Leido DNI" << endl;}
{tfno}                          {lt.push_back(YYText()); cout << "Leido TFNO" << endl;}
{email}                         {le.push_back(YYText()); cout << "Leido EMAIL" << endl;}


%%
int main (int argc, char *argv[])
{
    if (argc == 3)
    {
        entrada.open(argv[1]);
        salida.open(argv[2]);
        if (entrada.eof() || !entrada)
        {
            cerr << "Error de lectura" << endl;
            exit(1);
        }
    }
    else{
        cerr << "Introduce un número correcto de argumentos." << endl;
        exit(1);
    }

    yyFlexLexer flujo (&entrada, &salida);
    flujo.yylex();
    mostrar_solucion(le,ld,lt);

    return 0;
        
}

void mostrar_solucion (list<string> le, list<string> ld, list<string> lt ){
    assert(le.size() == ld.size() && ld.size() == lt.size() && le.size() == lt.size());
    int tam = le.size();
    int contador = 0;
    salida << tam << ld.size() << lt.size() << endl;
    for (int i = 0; i < tam; i++){
        
        salida << "Persona " << i+1 << endl;
        salida << "\tEmail: " << le.front() << endl;
        salida << "\tDNI: " << ld.front() << endl;
        salida << "\tTFNO: " << lt.front() << endl;
        
        le.pop_front();
        ld.pop_front();
        lt.pop_front();
    }
}
