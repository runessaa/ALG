#проверяет, можно ли дать вершине v цвет color
def is_safe(v, color, graph, colors, n):
    for i in range(n):
        if graph[v][i] == 1 and colors[i] == color: #если сосед имеет тот же цвет
            return False
    return True

#рекурсия для раскраски графа
def backtrack(v, n, k, graph, colors):
    if v == n: #все вершины раскрашены
        return True
    for color in range(1, k + 1): #пробуем каждый цвет
        if is_safe(v, color, graph, colors, n):
            colors[v] = color #назначаем цвет
            if backtrack(v + 1, n, k, graph, colors): #проверяем дальше
                return True
            colors[v] = 0 #сбрасываем цвет, если не получилось
    return False

#основная функция
def solve_graph_coloring():
    n, k = map(int, input().split()) #считываем n и k
    graph = []
    for _ in range(n):
        row = list(map(int, input().strip())) #считываем матрицу смежности
        graph.append(row)
    
    colors = [0] * n #массив для цветов вершин
    if backtrack(0, n, k, graph, colors): #запуск раскраски
        print("YES")
        print(*colors)  #вывод цвета
    else:
        print("NO")

solve_graph_coloring()