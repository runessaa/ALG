#include <iostream> 
#include <vector>
#include <string>
#include <ctime>
#include <cstdlib>
#include "Header.h" 

std::vector<int> generateRandomVector(int size) {
    std::vector<int> vec;
    for (int i = 0; i < size; ++i) {
        int randomNumber = rand() % 10000 - 5000;
        vec.push_back(randomNumber);
    }
    return vec;
}

int main() {
    setlocale(LC_ALL, "Russian");

    srand(time(NULL));

    std::vector<int> sizes = {1000, 5000, 10000, 25000, 50000};

    for (int size : sizes) {
        std::vector<int> original_data = generateRandomVector(size);

        //замер для сортировки Шелла
        std::vector<int> data_for_shell = original_data;
        clock_t start_shell = clock();
        shellSort(data_for_shell);
        clock_t end_shell = clock();
        double time_shell = (double)(end_shell - start_shell) / CLOCKS_PER_SEC * 1000.0;

        //замер для сортировки вставками
        std::vector<int> data_for_insertion = original_data;
        clock_t start_insertion = clock();
        insertionSort(data_for_insertion);
        clock_t end_insertion = clock();
        double time_insertion = (double)(end_insertion - start_insertion) / CLOCKS_PER_SEC * 1000.0;

        //замер для быстрой сортировки
        std::vector<int> data_for_quick = original_data;
        clock_t start_quick = clock();
        quickSort(data_for_quick);
        clock_t end_quick = clock();
        double time_quick = (double)(end_quick - start_quick) / CLOCKS_PER_SEC * 1000.0;

        std::cout << "Размер: " << size
            << " / Сортировка Шелла: " << time_shell << " мс"
            << " / Сортировка вставками: " << time_insertion << " мс"
            << " / Быстрая сортировка: " << time_quick << " мс"
            << std::endl;
    }
    return 0;
}