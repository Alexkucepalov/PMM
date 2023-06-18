#include <iostream>
#include <stdlib.h>
#include <conio.h>
#include <math.h>
#include <locale.h>
#include <fstream>
#include <string>

using namespace std;

int main()
{
	//объявляем переменные
	double h = 0.02; // шаг по координате
	int L = 1;// длина отрезка
	double t = 0.0;//время
	double D = 1;// коэффициэнт теплопроводности
	string file;
	double tao = (h * h) / (2.0 * D); // расчет шага по времени по формуле (4)
	// расчитываем кол-во узлов по x (количество шагов на отрезке)
	int N = L / h;
	// вводим массив координат(температур)
	double* T = new double[N + 2];
	double* T0 = new double[N + 2];


	// начальные условия
	for (int i = 0; i < N + 2; i++)
	{
		T0[i] = 0; // в начальный момент времени
	}


	while (t < 0.1)
	{
		// создание файлов для запииси результатов, где имя файла - время
		ofstream of;
		file = to_string(t);
		file += ".txt";
		of.open(file);
		t += tao;

		for (int i = 1; i < N + 1; i++)
		{
			//Расчет теплопроводности
			T[i] = (tao * T0[i + 1]) / (h * h) + (1 - (2 * tao) / (h * h)) * T0[i] + (tao / (h * h)) * T0[i - 1];  //расчет по формуле (3)
		}


		// граничные условия
		T[0] = ( t / (1 + t) );
		T[N+1] = 0;


		//Вывод результатов в файлы
		for (int i = 0; i < N + 2; i++)
		{
			T0[i] = T[i];
			of << i * 0.02 << " " << T[i] << endl;
		}
		of.close();
	}



	system("pause");
	return 0;
}











/*
 T[0] = T[1] + h;
T[N + 1] = 0;
*/