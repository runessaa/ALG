#include <iostream>
#include <vector>
#include <chrono>
#include <random>
#include <algorithm>
#include <future>
#include <string>
#include <iomanip>

using namespace std;

const int PARALLEL_THRESHOLD = 10000;

template<typename T>
int partition(vector<T>& arr, int low, int high) {
    T pivot = arr[low + (high - low) / 2];
    int i = low - 1;
    int j = high + 1;
    while (true) {
        do {
            i++;
        } while (arr[i] < pivot);
        do {
            j--;
        } while (arr[j] > pivot);
        if (i >= j) {
            return j;
        }
        swap(arr[i], arr[j]);
    }
}

template<typename T>
void sequential_quicksort(vector<T>& arr, int low, int high) {
    if (low < high) {
        int pi = partition(arr, low, high);
        sequential_quicksort(arr, low, pi);
        sequential_quicksort(arr, pi + 1, high);
    }
}

template<typename T>
void parallel_quicksort(vector<T>& arr, int low, int high, int threads) {
    if (low < high) {
        if (threads > 1 && (high - low) > PARALLEL_THRESHOLD) {
            int pi = partition(arr, low, high);
            auto future_obj = async(launch::async, [&]() {
                parallel_quicksort(arr, low, pi, threads / 2);
                });
            parallel_quicksort(arr, pi + 1, high, threads - threads / 2);
            future_obj.get();
        }
        else {
            sequential_quicksort(arr, low, high);
        }
    }
}

vector<int> generate_random_vector(int size) {
    vector<int> vec(size);
    mt19937 gen(1337);
    uniform_int_distribution<> dis(1, 1000000);
    for (int i = 0; i < size; ++i) {
        vec[i] = dis(gen);
    }
    return vec;
}

template<typename Func, typename... Args>
double measure_time(Func func, Args&&... args) {
    auto start = chrono::high_resolution_clock::now();
    func(forward<Args>(args)...);
    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double> duration = end - start;
    return duration.count();
}

int main() {
    setlocale(LC_ALL, "Russian");
    vector<int> sizes = {100, 1000, 10000, 20000, 30000, 40000, 50000, 100000, 500000, 1000000};

    cout << "Размер массива / БС (сек) / БС_П 2 потока (сек) / БС_П 4 потока (сек) / БС_П 8 потока (сек)" << endl;
    cout << fixed << setprecision(8);

    for (int size : sizes) {
        vector<int> original_vec = generate_random_vector(size);

        vector<int> vec_seq = original_vec;
        double time_seq = measure_time(sequential_quicksort<int>, ref(vec_seq), 0, size - 1);

        double time_par_2, time_par_4, time_par_8;

        vector<int> vec_par2 = original_vec;
        time_par_2 = measure_time(parallel_quicksort<int>, ref(vec_par2), 0, size - 1, 2);

        vector<int> vec_par4 = original_vec;
        time_par_4 = measure_time(parallel_quicksort<int>, ref(vec_par4), 0, size - 1, 4);

        vector<int> vec_par8 = original_vec;
        time_par_8 = measure_time(parallel_quicksort<int>, ref(vec_par8), 0, size - 1, 8);

        cout << size << " / " << time_seq << " / " << time_par_2 << " / " << time_par_4 << " / " << time_par_8 << endl;
    }

    return 0;
}