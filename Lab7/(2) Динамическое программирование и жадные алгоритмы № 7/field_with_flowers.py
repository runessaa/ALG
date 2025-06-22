import sys

def solve():
    try:
        n, m = map(int, sys.stdin.readline().split()) #читаем размеры поля
        field = [list(map(int, list(sys.stdin.readline().strip()))) for _ in range(n)] #читаем поле
    except (IOError, ValueError):
        print("Ошибка") #если ввод кривой
        return

    dp = [[0] * m for _ in range(n)] #таблица для макс. суммы пути

    for i in range(n - 1, -1, -1): #идём с конца
        for j in range(m):
            if i == n - 1 and j == 0: #начальная клетка
                dp[i][j] = field[i][j]
                continue
            from_down = dp[i + 1][j] if i < n - 1 else -1 #путь снизу
            from_left = dp[i][j - 1] if j > 0 else -1 #путь слева
            dp[i][j] = field[i][j] + max(from_down, from_left) #сумма + лучший путь

    print(dp[0][m-1])  #вывод результата

solve()