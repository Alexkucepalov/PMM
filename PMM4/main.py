import math
import numpy as np
import matplotlib.pyplot as plt

t_start = 0  # начальное время
t_end = 3  # конечное время
x_start = 0  # начальная координата по x
x_end = 100  # конечная координата по x
y_start = 0  # начальная координата по y
y_end = 100  # конечная координата по y

Q_x1 = x_end / 2  # x координата 1-го источника загрязнений
Q_y1 = y_end * 1 / 4  # y координата 1-го источника загрязнений

Q_x2 = x_end / 2  # x координата 2-го источника загрязнений
Q_y2 = y_end * 3 / 4  # y координата 2-го источника загрязнений

K = 0.5  # число Куранта, используется для рассчёта переменного шага
d = 1  # коэффициент турбулентной диффузии в тропосфере
alpha = 10 * np.pi / 180  # параметр, описываемый вымывание примеси из атмосферы

dx = 10  # шаг по x
dy = 10  # шаг по y

N_x = int((x_end - x_start) / dx)
N_y = int((y_end - y_start) / dy)
# print(N_x, N_y)


C = 10  # начальная скорость ветра
U = []  # скорость ветра по x
V = []  # скорость ветра по y

# кол-во примеси в определённой точке пространства
P0 = np.zeros((N_x, N_y))
P1 = np.zeros((N_x, N_y))


def MassFlow(i, j, n, dt):
    # Перенос масс
    if U[n] > 0:
        M1 = dx * dt * P0[i][j] * U[n]
        M2 = dx * dt * P0[i - 1][j] * U[n]
    elif U[n] < 0:
        M1 = dx * dt * P0[i + 1][j] * U[n]
        M2 = dx * dt * P0[i][j] * U[n]
    else:
        M1 = 0
        M2 = 0

    if V[n] > 0:
        M3 = dy * dt * P0[i][j] * V[n]
        M4 = dy * dt * P0[i][j - 1] * V[n]
    elif V[n] < 0:
        M3 = dy * dt * P0[i][j + 1] * V[n]
        M4 = dy * dt * P0[i][j] * V[n]
    else:
        M3 = 0
        M4 = 0

    return (1 / (dx * dy) * (M1 - M2 + M3 - M4))


def DiffuseTransfer(i, j, n, dt):
    # Диффузионный перенос
    D_dx = d * U[n]
    D_dy = d * V[n]
    D_x = D_dx * dt / pow(dx, 2) * (P0[i + 1][j] - 2 * P0[i][j] + P0[i - 1][j])
    D_y = D_dy * dt / pow(dy, 2) * (P0[i][j + 1] - 2 * P0[i][j] + P0[i][j - 1])

    return D_x + D_y


def Pollution(p, i, j, dt):
    # Мощность выбросов
    x = x_start + i * dx
    y = y_start + j * dy

    if (abs(Q_x1 - x) < dx) and (abs(Q_y1 - y) < dy):
        Q_orig = 100
    elif (abs(Q_x2 - x) < dx) and (abs(Q_y2 - y) < dy):
        Q_orig = 100
    else:
        return 0
    return dt * (Q_orig - alpha * p)


def main():
    n = 0  # Счётчик для U и V
    dt = 0.1  # Шаг по времени

    for t in np.arange(t_start, t_end + dt, dt):
        if t <= (t_end / 2):
            # U.append(C * math.cos(t))
            # V.append(C * math.sin(t))
            U.append(0)
            V.append(C)
        elif t > (t_end / 2) and t <= t_end:
            # U.append(C * math.sin(t))
            # V.append(C * math.cos(t))
            U.append(-C)
            V.append(0)
    t = t_start

    listX = [x for x in np.arange(x_start, x_end, dx)]
    listY = [y for y in np.arange(y_start, y_end, dy)]

    while (t < t_end):
        # dt = K * min((dx*dx/(2*d) + dx/U[n]), (dy*dy/(2*d) + dy/V[n]))

        # Граничные условия
        for j in range(N_y):
            P0[0, j] = 0  # Верхняя граница
            P0[N_x - 1, j] = P0[N_x - 2, j]  # Нижняя граница
        # P1[N_x-1, j] = 0
        for i in range(N_x):
            P0[i, N_y - 1] = P0[i, N_y - 2]  # Правая граница
            P0[i, 0] = 0  # Левая граница
        # P1[i, 0] = 0

        # Основные расчёты
        for i in range(1, N_x - 1):
            for j in range(1, N_y - 1):
                # Ветровой снос
                M = MassFlow(i, j, n, dt)

                # Диффузионное расплывание
                D = DiffuseTransfer(i, j, n, dt)

                # Совокупность источников и стоков рассматриваемой примеси
                Q = Pollution(P0[i][j], i, j, dt)

                # Распространение примесей
                P1[i][j] = P0[i][j] - M + D + Q

            # print(M, D, Q, P1[i][j])

        # переопределяем слои
        for i in range(N_x):
            for j in range(N_y):
                P0[i, j] = P1[i, j]

        t += dt
        n = n + 1

        plt.clf()
        plt.contourf(listX, listY, P1)
        plt.colorbar()
        plt.pause(0.05)
    plt.show()


main()