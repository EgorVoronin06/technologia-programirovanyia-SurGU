#include <iostream>
#include <cmath>
#include <bitset> // Для вывода чисел в бинарной системе
#include <limits> // Для преобразования double в unsigned long long

using namespace std;

// Функция для вывода double в бинарном представлении
void printBinary(double value) {
    // Преобразуем представление double в целое число для вывода битов
    unsigned long long binaryRepresentation = *reinterpret_cast<unsigned long long*>(&value);
    bitset<64> bits(binaryRepresentation);
    cout << bits;
}

int main() {
    double x = 0. 1; // Значение x по условию
    double eps;     // Точность вычислений
    double a;       // Очередной член ряда
    double s = 0.0; // Сумма ряда
    int i = 1;      // Счётчик итераций

    // Ввод точности
    cout << "Введите точность (например, 0.0001): ";
    cin >> eps;

    // Вычисление суммы ряда
    do {
        // Вычисляем очередной член ряда: (-1)^i * x^(2i+1) / (2i+1)
        a = pow(-1, i) * pow(x, 2 * i + 1) / (2 * i + 1);
        s += a; // Добавляем член к сумме
        i++;    // Увеличиваем счётчик
    } while (abs(a) > eps); // Пока член ряда превышает точность

    // Вывод результатов в десятичной системе
    cout << "Сумма ряда: " << s << endl;
    cout << "Количество итераций: " << i - 1 << endl;
    cout << "Точность: " << eps << endl;

    // Вывод результатов в бинарной системе
    cout << "Сумма ряда в бинарной системе: ";
    printBinary(s);
    cout << endl;

    cout << "Количество итераций в бинарной системе: ";
    bitset<32> iterBits(i - 1); // Предполагаем, что i < 2^32
    cout << iterBits << endl;

    cout << "Точность в бинарной системе: ";
    printBinary(eps);
    cout << endl;

    return 0;
}
