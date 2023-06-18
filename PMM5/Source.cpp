#include <iostream>
#include <fstream>
#include <string>
#include <cmath>

using namespace std;

//расчет коэф теплопроводности
double DMax(double* D, int N)
{
	double max = D[0];
	for (int i = 1; i < N + 2; i++)
	{
		if (D[i] > D[i - 1])
		{
			max = D[i];
		}
	}
	return max;
}
double AvgDP(double* D, int i)
{
	return (D[i + 1] + D[i]) / 2;
}
double AvgDM(double* D, int i)
{
	return (D[i - 1] + D[i]) / 2;
}


int main()
{
	setlocale(LC_ALL, "Rus");

	int x_s = 0;			// начальная координата
	int x_e = 1;			// конечная координата
	double h = 0.04;		// шаг по
	double t = 0.01;			// начальное
	double t_n = 2.0;		// конечное

	double D0 = 1;
	double B = 0.2;

	double D_max = 1;
	double k = 1;
	int N = (x_e - x_s) / h;
	double* D = new double[N + 2];
	double* T1 = new double[N + 2];
	double* T0 = new double[N + 2];

	//НУ
	for (int i = 0; i < N + 2; i++)
	{
		T0[i] = 0;
	}

	do
	{	
		string path = "txt\\";
		ofstream stream;

		//ГУ
		T0[0] = t / (1 + t);
		T0[N + 1] = 0;


		// рассчёт D
		for (int i = 0; i < N + 2; i++)
		{
			D[i] = D0 * pow(T0[i], B);
		}
		D_max = DMax(D, N);

		//условие устойчивости
		double tau = 1 * (h * h) / (2.0 * D_max);

		for (int i = 1; i < N + 1; i++)
		{
			T1[i] = (tau * AvgDP(D, i) * T0[i + 1]) / (h * h) +
				(1 - (tau * (AvgDP(D, i) + AvgDM(D, i))) / (h * h)) * T0[i] +
				(tau * AvgDM(D, i) / (h * h)) * T0[i - 1];
		}

		for (int i = 0; i < N + 2; i++)
			T0[i] = T1[i];


		// Запись в файл
		int var = t * 1000;
		if (var % 100 == 0)
		{
			path += to_string(t) + ".txt";
			stream.open(path);
			if (!stream.is_open()) {
				cout << "error!" << endl;
				return 0;
			}
			cout << "\nt = " << t << endl << endl;
			for (int i = 0; i < N + 2; i++)
			{

				stream << i * h << "\t" << T1[i] << endl;
				cout << "\t" << i * h << "\t" << T1[i] << endl;
			}
			stream.close();
		}
		t += tau;

	} while (t <= t_n);

	cout << t << endl;

	delete[] T0;
	delete[] T1;

	return 0;
}