#include <iostream>
#include <fstream>
#include <string>
#include <math.h>

using namespace std;



int main()
{
    double h = 0.02;// шаг по координате
    double a = 0; // начало отрезка
    double b = 1; // конец отрезка
    double t = 0; // время
    double ts = 0;
    double dts = 0.00002;
    double D = 1; // коэффициэнт теплопроводности
    double tao = (h * h) / (2 * D);  // расчет шага по времени по формуле (4)
    int N = int(((b - a) / h)); // расчитываем кол-во узлов по x (количество шагов на отрезке)
    string file;
    double* T0 = new double[N + 2];//предыдущий
    double* T1 = new double[N + 2];//текущий
    double* A = new double[N + 1]; // Коэффициент прогонки А
    double* B = new double[N + 1]; // Коэффициент прогонки B


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

        // граничные условия
        A[0] = 0;
        B[0] = (t / (1 + t));

        for (int i = 0; i < N + 1; i++)
        {
            // определение прогоночных коэффициентов
            A[i + 1] = (tao) / (h * h + tao * (2 - A[i])); // (9) Рекурентое уравнение для определения коэффициента A
            B[i + 1] = (h * h * T0[i] + tao * B[i]) / (h * h + tao * (2 - A[i])); // Рекурентое уравнение для определения коэффициента В
        }

        T1[N + 1] = 0;

        for (int i = N; i >= 0; i--)
        {
            T1[i] = A[i + 1] * T1[i + 1] + B[i + 1]; // (6)
            T0[i] = T1[i];
        }

        //Вывод результатов в файлы
        for (int i = 0; i < N + 2; i++)
        {
            ts += dts;
            of << i * 0.02 << " " << T0[i] << endl;
        }

        of.close();
        t += tao;
    }

    system("pause");

    return 0;
}