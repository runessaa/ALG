#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <queue>
#include <cmath>
#include <algorithm>
#include <random>
#include <clocale>

using namespace std;
using namespace sf;

const int GRID_SIZE = 30; // Размер поля NxN
const int CELL_SIZE = 25; // Размер одной ячейки в пикселях
const int WINDOW_WIDTH = GRID_SIZE * CELL_SIZE;
const int WINDOW_HEIGHT = GRID_SIZE * CELL_SIZE;

// Типы ячеек
enum CellType { EMPTY, OBSTACLE, START, END, PATH, VISITED, IN_QUEUE };
enum Mode { STATIC, RANDOM_FIELD, RANDOM_MAP };

// Структура для представления ячейки
struct Cell {
    int x, y;
    int weight = 1; // Вес клетки для алгоритма
    CellType type = EMPTY; // Тип клетки

    // Для алгоритма A*
    int f = INT_MAX; // g + h
    int g = INT_MAX; // Расстояние от старта
    int h = 0; // Эвристика до цели
    Cell* parent = nullptr; // Родительская клетка

    Cell(int x = 0, int y = 0) : x(x), y(y) {}
    // Перезагрузка оператора == для сравнения ячеек
    bool operator==(const Cell& other) const { return x == other.x && y == other.y; }
};

// Для сравнения клеток в очереди
struct CompareCell {
    bool operator()(const Cell* a, const Cell* b) const { return a->f > b->f; }
};

// Функции
int heuristic(const Cell& a, const Cell& b); // Расчет эвристики
bool isValid(int x, int y, int currentGridSize); // Проверка координат
void reconstructPath(Cell* end); // Восстановление пути
void clearGridState(vector<vector<Cell>>& grid); // Очистка сетки
void generateStaticGrid(vector<vector<Cell>>& grid, Cell*& start, Cell*& end, int& currentGridSize); // Статичное поле
void generateRandomField(vector<vector<Cell>>& grid, Cell*& start, Cell*& end, int currentGridSize); // Случайное поле
void generateRandomMap(vector<vector<Cell>>& grid, Cell*& start, Cell*& end, int currentGridSize); // Случайная карта с весами
bool a_star_algorithm(vector<vector<Cell>>& grid, Cell& startCell, Cell& endCell, RenderWindow& window, Mode currentMode); // Алгоритм A*
void draw(RenderWindow& window, const vector<vector<Cell>>& grid, Mode currentMode); // Отрисовка

int main() {
    setlocale(LC_ALL, "Russian"); // + русский язык в консоли

    // Создание окна
    RenderWindow window(VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Laba6");
    window.setFramerateLimit(60);

    vector<vector<Cell>> grid; // Сетка клеток
    Cell* start = nullptr; // Стартовая клетка
    Cell* end = nullptr; // Конечная клетка
    int currentGridSize = GRID_SIZE; // Текущий размер сетки
    Mode currentMode = STATIC; // Текущий режим

    // Вывод инструкции
    cout << "Управление:\n"
        << "F1: Поле по варианту\n"
        << "R: Случайное поле\n"
        << "F2: Случайная карта с весом\n"
        << "V: Очистить путь\n"
        << "Пробел: Запустить алгоритм A*\n";

    // Создание начального поля
    generateStaticGrid(grid, start, end, currentGridSize);

    // Главный цикл программы
    while (window.isOpen()) {
        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed) window.close(); // Закрытие окна

            // Обработка клавиш
            if (event.type == Event::KeyPressed) {
                if (event.key.code == Keyboard::F1) {
                    // Статичное поле
                    currentMode = STATIC;
                    currentGridSize = 10;
                    generateStaticGrid(grid, start, end, currentGridSize);
                }
                else if (event.key.code == Keyboard::F2) {
                    // Случайная карта с весами
                    currentMode = RANDOM_MAP;
                    currentGridSize = GRID_SIZE;
                    generateRandomMap(grid, start, end, currentGridSize);
                }
                else if (event.key.code == Keyboard::R) {
                    // Случайное поле
                    currentMode = RANDOM_FIELD;
                    currentGridSize = GRID_SIZE;
                    generateRandomField(grid, start, end, currentGridSize);
                }
                else if (event.key.code == Keyboard::V) {
                    // Очистка пути
                    clearGridState(grid);
                }
                else if (event.key.code == Keyboard::Space) {
                    // Запуск A*
                    if (start && end) {
                        a_star_algorithm(grid, *start, *end, window, currentMode);
                    }
                }
            }
        }
        draw(window, grid, currentMode); // Отрисовка поля
    }
    return 0;
}

// Функция для вычисления эвристики (манхэттенское расстояние)
int heuristic(const Cell& a, const Cell& b) { return abs(a.x - b.x) + abs(a.y - b.y); }

// Проверка, что координаты в пределах сетки
bool isValid(int x, int y, int currentGridSize) { return x >= 0 && x < currentGridSize && y >= 0 && y < currentGridSize; }

// Восстановление пути от конца к началу
void reconstructPath(Cell* end) {
    Cell* current = end;
    while (current != nullptr) {
        if (current->type != START && current->type != END) {
            current->type = PATH; // Помечаем путь
        }
        current = current->parent;
    }
}

// Очистка состояния клеток(не типов)
void clearGridState(vector<vector<Cell>>& grid) {
    if (grid.empty()) return;
    for (auto& row : grid) {
        for (auto& cell : row) {
            cell.f = INT_MAX; cell.g = INT_MAX; cell.parent = nullptr;
            if (cell.type == VISITED || cell.type == PATH || cell.type == IN_QUEUE) {
                cell.type = EMPTY;
            }
        }
    }
}

// Создание поля по варианту
void generateStaticGrid(vector<vector<Cell>>& grid, Cell*& start, Cell*& end, int& currentGridSize) {
    currentGridSize = 10;
    grid.assign(currentGridSize, vector<Cell>(currentGridSize));
    for (int i = 0; i < currentGridSize; ++i) {
        for (int j = 0; j < currentGridSize; ++j) {
            grid[i][j] = Cell(i, j);
        }
    }

    // Устанавка старта и финиша
    start = &grid[0][0];
    start->type = START;
    end = &grid[9][9];
    end->type = END;

    // Препятствия
    vector<pair<int, int>> obstacles = {
        {2, 0}, {3, 0}, {7, 0}, {8, 0}, {9, 0},
        {2, 1}, {9, 1},
        {0, 2}, {2, 2}, {4, 2},
        {2, 3}, {4, 3}, {7, 3},
        {8, 4}, {9, 4},
        {1, 5}, {4, 5}, {5, 5}, {6, 5},
        {2, 6},
        {1, 7},
        {2, 8}, {7, 8}, {8, 8},
        {3, 9}, {4, 9}, {5, 9}
    };

    for (const auto& p : obstacles) {
        grid[p.first][p.second].type = OBSTACLE;
    }
}

// Функция для создания случайных полей
void generateRandomGridBase(vector<vector<Cell>>& grid, Cell*& start, Cell*& end, int currentGridSize, bool isWeighted) {
    grid.assign(currentGridSize, vector<Cell>(currentGridSize));
    random_device rd; mt19937 gen(rd());
    uniform_real_distribution<> dis(0.0, 1.0);
    uniform_int_distribution<> weight_dis(1, 9);

    // Заполнение сетки
    for (int i = 0; i < currentGridSize; ++i) {
        for (int j = 0; j < currentGridSize; ++j) {
            grid[i][j] = Cell(i, j);
            if (dis(gen) < 0.25) grid[i][j].type = OBSTACLE; // 25% шанс препятствия
            if (isWeighted) grid[i][j].weight = weight_dis(gen); // Случайный вес
            else grid[i][j].weight = 1; // Вес 1
        }
    }

    // Выбор случайных старта и финиша
    uniform_int_distribution<> pos_dis(0, currentGridSize - 1);
    do { start = &grid[pos_dis(gen)][pos_dis(gen)]; } while (start->type == OBSTACLE);
    do { end = &grid[pos_dis(gen)][pos_dis(gen)]; } while (end->type == OBSTACLE || start == end);
    start->type = START; end->type = END;
}

// Создание случайного поля
void generateRandomField(vector<vector<Cell>>& grid, Cell*& start, Cell*& end, int currentGridSize) {
    generateRandomGridBase(grid, start, end, currentGridSize, false);
}

// Создание случайной карты с весами
void generateRandomMap(vector<vector<Cell>>& grid, Cell*& start, Cell*& end, int currentGridSize) {
    generateRandomGridBase(grid, start, end, currentGridSize, true);
}

// Отрисовка сетки
void draw(RenderWindow& window, const vector<vector<Cell>>& grid, Mode currentMode) {
    window.clear(Color(50, 50, 50));
    if (grid.empty()) { window.display(); return; }

    int currentGridSize = grid.size();
    float currentCellSize = (float)WINDOW_WIDTH / currentGridSize;

    // Рисуем каждую клетку
    for (int i = 0; i < currentGridSize; ++i) {
        for (int j = 0; j < currentGridSize; ++j) {
            RectangleShape cellShape(Vector2f(currentCellSize - 1, currentCellSize - 1));
            cellShape.setPosition(i * currentCellSize, j * currentCellSize);
            const auto& cell = grid[i][j];

            // Разные цвета для разных типов клеток
            switch (cell.type) {
            case EMPTY:
                if (currentMode == RANDOM_MAP) {
                    // Для карты с весами оттенки серого
                    Uint8 intensity = 255 - (cell.weight - 1) * 25;
                    cellShape.setFillColor(Color(intensity, intensity, intensity));
                }
                else {
                    cellShape.setFillColor(Color::White);
                }
                break;
            case OBSTACLE: cellShape.setFillColor(Color::Black); break;
            case START:    cellShape.setFillColor(Color(255, 165, 0)); break; // Оранжевый
            case END:      cellShape.setFillColor(Color(64, 224, 208)); break; // Бирюзовый
            case PATH:     cellShape.setFillColor(Color(128, 0, 128)); break; // Фиолетовый
            case VISITED:  cellShape.setFillColor(Color::Red); break;
            case IN_QUEUE: cellShape.setFillColor(Color::Green); break;
            }
            window.draw(cellShape);
        }
    }
    window.display();
}

// Алгоритм A*
bool a_star_algorithm(vector<vector<Cell>>& grid, Cell& startCell, Cell& endCell, RenderWindow& window, Mode currentMode) {
    priority_queue<Cell*, vector<Cell*>, CompareCell> open_list;
    clearGridState(grid);

    // Начальные значения для старта
    startCell.g = 0;
    startCell.h = heuristic(startCell, endCell);
    startCell.f = startCell.g + startCell.h;
    open_list.push(&startCell);

    while (!open_list.empty()) {
        Cell* current = open_list.top();
        open_list.pop();

        if (current->type == VISITED) continue;

        // Помечаем как посещенную
        if (current->type != START && current->type != END) {
            current->type = VISITED;
        }

        // Если дошли до конца
        if (*current == endCell) {
            reconstructPath(current);
            draw(window, grid, currentMode);
            return true;
        }

        // Проверяем соседей
        int dx[] = { 0, 0, 1, -1 };
        int dy[] = { 1, -1, 0, 0 };
        for (int i = 0; i < 4; ++i) {
            int newX = current->x + dx[i];
            int newY = current->y + dy[i];

            if (isValid(newX, newY, grid.size())) {
                Cell& neighbor = grid[newX][newY];

                if (neighbor.type == OBSTACLE || neighbor.type == VISITED) continue;

                int tentative_g = current->g + neighbor.weight;

                // Если нашли лучший путь
                if (tentative_g < neighbor.g) {
                    neighbor.parent = current;
                    neighbor.g = tentative_g;
                    neighbor.h = heuristic(neighbor, endCell);
                    neighbor.f = neighbor.g + neighbor.h;

                    open_list.push(&neighbor);
                    if (neighbor.type != END) {
                        neighbor.type = IN_QUEUE; // Помечаем как в очереди
                    }
                }
            }
        }
        draw(window, grid, currentMode); // Обновление отрисовки
        sleep(milliseconds(5)); // Задержка для анимации
    }
    return false; // Путь не найден
}