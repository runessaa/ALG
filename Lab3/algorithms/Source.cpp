#include "Header.h"
#include <utility>

//сортировка Шелла
void shellSort(std::vector<int>& arr) {
    int n = arr.size();
    if (n < 2) {
        return;
    }

    for (int gap = n / 2; gap > 0; gap /= 2) {
        for (int i = gap; i < n; i++) {
            int temp = arr[i];
            int j;

            for (j = i; j >= gap && arr[j - gap] > temp; j -= gap) {
                arr[j] = arr[j - gap];
            }

            arr[j] = temp;
        }
    }
}

//сортировка вставками
void insertionSort(std::vector<int>& arr) {
    int n = arr.size();
    if (n < 2) {
        return;
    }

    for (int i = 1; i < n; i++) {
        int key = arr[i];
        int j = i - 1;

        while (j >= 0 && arr[j] > key) {
            arr[j + 1] = arr[j];
            j = j - 1;
        }
        arr[j + 1] = key;
    }
}

//быстрая сортировка
int partition(std::vector<int>& arr, int low, int high) {
    int pivot = arr[(low + high) / 2];
    int i = low;
    int j = high;

    while (true) {
        while (arr[i] < pivot) {
            i++;
        }
        while (arr[j] > pivot) {
            j--;
        }

        if (i >= j) {
            return j;
        }

        std::swap(arr[i++], arr[j--]);
    }
}

void quickSortRecursive(std::vector<int>& arr, int low, int high) {
    if (low < high) {
        int pi = partition(arr, low, high);

        quickSortRecursive(arr, low, pi);
        quickSortRecursive(arr, pi + 1, high);
    }
}

void quickSort(std::vector<int>& arr) {
    if (arr.empty()) {
        return;
    }
    quickSortRecursive(arr, 0, arr.size() - 1);
}