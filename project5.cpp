#include <iostream>
#include <cstdlib>
#include <ctime>
#include <iomanip>

using namespace std;

// Функция для обработки матрицы
int processMatrix(int matrix[8][8], int threshold) {
    int sum = 0;

    // Проходим по диагональным элементам (главная диагональ)
    for (int i = 0; i < 8; i++) {
        // Проверяем, больше ли элемент заданного числа
        if (matrix[i][i] > threshold) {
            sum += matrix[i][i];
        }
    }

    return sum;
}

int main() {
    // Устанавливаем русскую локаль
    setlocale(LC_ALL, "Rus");

    // Инициализируем генератор случайных чисел
    srand(time(NULL));

    const int SIZE = 8;
    const int MAX_VALUE = 150;
    int matrix[SIZE][SIZE];
    int threshold;

    // Заполняем матрицу случайными числами в диапазоне [0; 150]
    cout << "Исходная матрица" << endl;
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            matrix[i][j] = rand() % (MAX_VALUE + 1); // [0; 150]
            cout << setw(4) << matrix[i][j];
        }
        cout << endl;
    }

    // Запрашиваем у пользователя пороговое значение
    cout << "\nзаданное число: ";
    cin >> threshold;

    // Вызываем функцию обработки матрицы
    int diagonalSum = processMatrix(matrix, threshold);

    // Выводим результат
    cout << "\nРезультаты:" << endl;
    cout << "Сумма диагональных элементов, больших " << threshold << ": " << diagonalSum << endl;

    // Дополнительно выводим диагональные элементы для проверки
    cout << "\nДиагональные элементы матрицы:" << endl;
    for (int i = 0; i < SIZE; i++) {
        cout << matrix[i][i] << " ";
    }
    cout << endl;

    return 0;
}
