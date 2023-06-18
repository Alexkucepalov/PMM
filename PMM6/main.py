import numpy as np
import matplotlib.pyplot as plt


def OutputGraphic(listX, listY, U):
    fig = plt.figure()
    ax = plt.subplot(111, projection="3d")
    listX, listY = np.meshgrid(listX, listY)
    ax.plot_surface(listX, listY, np.array(U))

    plt.show()


def Elliptic():
    # Количество яйчеек сетки
    countNodes = 50

    # Размер сетки
    left = 0
    right = 1
    bot = 0
    top = 1

    # Шаг, определяемый из количества яйчеек
    dx = (right - left) / countNodes
    dy = (top - bot) / countNodes

    # Величина, при достижении которой срабатывает условие выхода
    sigma = 0.00001

    # Набор значений временных и пространственных координат
    listX = [i for i in np.arange(left, right, dx)]
    listY = [i for i in np.arange(bot, top, dy)]

    U0 = [[0 for listX in range(countNodes)] for listY in range(countNodes)]
    U1 = [[0 for listX in range(countNodes)] for listY in range(countNodes)]

    alpha = dy ** 2 / (2 * (dx ** 2 + dy ** 2))
    betha = dx ** 2 / (2 * (dx ** 2 + dy ** 2))
    gamma = -(dx ** 2 * dy ** 2) / (2 * (dx ** 2 + dy ** 2))

    count = 0
    out = sigma + 1
    while (out > sigma):
        # Начальное и конечное условия
        for m in range(countNodes):
            U0[0][m] = StartCondition(m * dx)
            U0[countNodes - 1][m] = EndCondition(m * dx)

        # Граничные условия
        for n in range(countNodes):
            U0[n][0] = leftCondition(n * dy)
            U0[n][countNodes - 1] = rightCondition(n * dy)
        out = 0
        for i in range(1, countNodes - 1):
            for j in range(1, countNodes - 1):
                secDerivUX = (U0[i + 1][j] + U0[i - 1][j]) * alpha
                secDerivUY = (U0[i][j + 1] + U0[i][j - 1]) * betha
                func = Func(i * dx, j * dy) * gamma
                u0 = U1[i][j]
                U1[i][j] = secDerivUX + secDerivUY + func
                u1 = U1[i][j]

                out += abs(u0 - u1)
        count = count + 1
        out *= 1 / (countNodes + 1) ** 2
        U0 = U1.copy()

    OutputGraphic(listX, listY, U0)
    print(count)


def Func(x, y):
    return abs(x - y)


def leftCondition(y):
    return 2 * y * (1 - y)


def rightCondition(y):
    return 2 * y * (1 - y)


def StartCondition(x):
    return np.sin(np.pi * x) ** 2


def EndCondition(x):
    return np.sin(np.pi * x) ** 2


if __name__ == "__main__":
    Elliptic()