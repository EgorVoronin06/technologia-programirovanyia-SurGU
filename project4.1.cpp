#include <iostream>
#include <bitset>

using namespace std;

int main() {
    unsigned long number;

    // Ввод числа
    cout << "Введите число типа unsigned long: ";
    cin >> number;

    // Вывод исходного числа
    cout << "Исходное число: " << number << endl;
    cout << "Исходное число в двоичной системе: " << bitset<32>(number) << endl;

    // Маска для выделения битов, кратных 5 (биты 4, 9, 14, 19, 24, 29)
    unsigned long mask_multiple_5 = 0;
    for (int i = 4; i < 32; i += 5) {
        mask_multiple_5 |= (1UL << i);
    }

    // Маска для выделения соседних справа битов (биты 5, 10, 15, 20, 25, 30)
    unsigned long mask_right_neighbors = 0;
    for (int i = 5; i < 32; i += 5) {
        mask_right_neighbors |= (1UL << i);
    }

    // Копируем значения соседних справа битов
    unsigned long right_bits = number & mask_right_neighbors;

    // Сдвигаем скопированные биты на 1 позицию влево
    right_bits >>= 1;

    // Очищаем биты, кратные 5, в исходном числе
    number &= ~mask_multiple_5;

    // Устанавливаем биты, кратные 5, равными соседним справа
    number |= (right_bits & mask_multiple_5);

    // Вывод результата
    cout << "Преобразованное число: " << number << endl;
    cout << "Преобразованное число в двоичной системе: " << bitset<32>(number) << endl;

    return 0;
}
