#include <iostream>
#include <vector>


using namespace std;


int main(){
    int brujula;
    int contador = 0;		   //para el bucle
	int contador_escritos = 0; //En el que estamos situados se escribe si o si
    int matriz[30][30];
    brujula = 2;
    int fil = 15;
    int col = 11;
    int sensores[16] = {1};

    for (int i = 0; i < 30; i++){
        for (int j = 0; j < 30; j++){
            matriz[i][j] = 0;
        }
    }

    for (int i = 0; i < 16; i++){
        sensores[i] = i+1;
    }

	switch (brujula)
	{
	case 0:
		for (int i = fil; i > fil - 4; i--)
		{

            
			for (int j = col - contador; j <= col + contador; j++)
			{
				matriz[i][j] = sensores[contador_escritos];
				contador_escritos += 1;
			}

			contador += 1;
		}
	case 1:
		for (int i = col; i < col + 4; i++)
		{
			for (int j = fil - contador; j <= fil + contador; j++)
			{
				matriz[j][i] = sensores[contador_escritos];
				contador_escritos += 1;
			}

			contador += 1;
		}
	case 2:
		for (int i = fil; i < fil + 4; i++)
		{
			for (int j = col + contador; j >= col - contador; j--)
			{
				matriz[i][j] = sensores[contador_escritos];
                cout << sensores[contador_escritos] << endl;
                cout << matriz[i][j] << endl;
				contador_escritos += 1;
			}

			contador += 1;
		}

	case 3:
		for (int i = col; i > col - 4; i--)
		{
			for (int j = fil + contador; j >= fil - contador; j--)
			{
				matriz[j][i] = sensores[contador_escritos];
				contador_escritos += 1;
			}

			contador += 1;
		}
	}


    for (int i = 0; i < 30; i++){
        for(int j = 0; j < 30; j++){
            cout << matriz[i][j] << " ";
        }
        cout << endl;
    }
}