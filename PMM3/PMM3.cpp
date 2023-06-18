#include <iostream>
#include <fstream>
#include <string>
#include <math.h>

#define PI 3.1415926535


using namespace std;


int main()
{
    double c = 1.0,
    h = 0.02, // шаг по координате
    t = 0.0, //  время
    tau = h / c; // шаг по времени (9) условие Куранта
    int a = 0, b = 1, N = int(double(b - a) / h); // Размер промежутка на Х и количество точек (5)
    double* U0 = new double[N + 1]; // Дополнительный массив из начального условия
    double* U1 = new double[N + 1]; // U1 определяется из второго начального условия
    double* U2 = new double[N + 1]; // Верхний слой
    double* x = new double[N + 1]; // Разбиение х-координаты на h 


    for (int i = 0; i <= N; i++)
    {
        x[i] = a + (h * i); //Высчитывание координат точек на Х

        //Начальные условия
        if (x[i] > (1.0 / 3.0))
        {
            U0[i] = 0;
            U1[i] = 0;
        }
        else if (x[i] <= (1.0 / 3.0))
        {
            U0[i] = 0.2 * pow(sin(3 * PI * x[i]), 2);
            U1[i] = U0[i] * tau * ( - 0.6 * PI * sin(6 * PI * x[i]) );
        }
    }


    while (t < 1)
    {
        string file;
        ofstream of;
        file = to_string(t);
        file += ".txt";
        of.open(file);
        t += tau;


        U1[0] = 0; // Граничные условия
        U1[N] = 0;

        for (int i = 1; i < N; i++)
        {
            U2[i] = ( pow(c, 2) * pow(tau, 2) * (U1[i + 1] - 2.0 * U1[i] + U1[i - 1]) / (h * h) ) + 2.0 * U1[i] - U0[i]; // Пятиточечная схема (5)
            of << i * h << " " << U2[i] << endl; // запись результатов в файл
        }

        for (int i = 1; i < N + 1; i++)
        {
            U0[i] = U1[i];
            U1[i] = U2[i];
        }

        t += tau;
        of.close();
    }

    system("pause");
    return 0;
}