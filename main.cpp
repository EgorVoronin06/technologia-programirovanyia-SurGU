#include <iostream>
#include <string>
#include <cstring>
#include <locale>
#include <windows.h> // для SetConsoleCP и SetConsoleOutputCP

using namespace std;

// Функция для проверки UTF-8 кириллических символов
bool isCyrillicUTF8(const char* str, int pos) {
    if (str[pos] == '\0' || str[pos + 1] == '\0') return false;

    unsigned char c1 = (unsigned char)str[pos];
    unsigned char c2 = (unsigned char)str[pos + 1];

    // Проверка диапазонов кириллических символов в UTF-8
    return (c1 == 208 && (c2 >= 144 && c2 <= 191)) || // А-Я, Ё
        (c1 == 209 && (c2 >= 128 && c2 <= 143));   // а-п, р-я, ё
}

// Функция для проверки, является ли символ буквой (русской или латинской)
bool isLetter(const char* str, int& pos) {
    unsigned char c = (unsigned char)str[pos];

    // Проверка латинских букв
    if ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z')) {
        return true;
    }

    // Проверка кириллицы в UTF-8
    if (isCyrillicUTF8(str, pos)) {
        pos++; // Пропускаем второй байт UTF-8 символа
        return true;
    }

    // Проверка кириллицы в Windows-1251 (на всякий случай)
    if ((c >= 192 && c <= 255) || c == 168 || c == 184) {
        return true;
    }

    return false;
}

// Функция для проверки, состоит ли строка только из букв и дефисов
bool containsOnlyLettersAndHyphens(const char* str) {
    for (int i = 0; str[i] != '\0'; i++) {
        unsigned char c = (unsigned char)str[i];

        // Проверка дефиса
        if (c == '-') continue;

        // Создаем временную переменную для позиции
        int temp_pos = i;
        if (!isLetter(str, temp_pos)) {
            return false;
        }

        // Если это был UTF-8 символ, пропускаем следующий байт
        if (temp_pos != i) {
            i = temp_pos;
        }
    }
    return true;
}

// Функция для подсчета количества букв в слове (игнорируя дефисы)
int countLetters(const char* str) {
    int count = 0;
    for (int i = 0; str[i] != '\0'; i++) {
        int temp_pos = i;
        if (isLetter(str, temp_pos)) {
            count++;
            // Если это был UTF-8 символ, пропускаем следующий байт
            if (temp_pos != i) {
                i = temp_pos;
            }
        }
    }
    return count;
}

int main() {
    // Устанавливаем кодировку консоли для Windows
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    // Устанавливаем локаль
    setlocale(LC_ALL, "Russian");

    const int MAX_LENGTH = 100;
    char word[MAX_LENGTH];

    cout << "введите слово: ";
    cin >> word;

    // Проверяем, содержит ли слово только буквы и дефисы
    if (!containsOnlyLettersAndHyphens(word)) {
        cout << "ОШИБКА: Слово должно содержать только буквы и дефисы" << endl;
        cout << "ERROR2: Недопустимые символы в слове" << endl;
        return 1;
    }

    // Вычисляем количество букв (без учета дефисов)
    int letterCount = countLetters(word);

    cout << "Размер: " << letterCount << " символов" << endl;

    // Проверяем, кратно ли количество букв трем
    if (letterCount % 3 == 0) {
        cout << "кратно(" << letterCount << ")" << endl;
    }
    else {
        cout << "не кратно(" << letterCount << ")" << endl;
    }

    // Дополнительная информация о слове
    cout << "\nДополнительная информация:" << endl;
    cout << "введенное слово: " << word << endl;

    // Определяем тип букв в слове
    bool hasLatin = false;
    bool hasCyrillic = false;

    for (int i = 0; word[i] != '\0'; i++) {
        int temp_pos = i;
        if (isLetter(word, temp_pos)) {
            unsigned char c = (unsigned char)word[i];
            if ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z')) {
                hasLatin = true;
            }
            else {
                hasCyrillic = true;
            }

            // Если это был UTF-8 символ, пропускаем следующий байт
            if (temp_pos != i) {
                i = temp_pos;
            }
        }
    }

    if (hasLatin && hasCyrillic) {
        cout << "слово содержит латинские и русские буквы" << endl;
    }
    else if (hasLatin) {
        cout << "только латинские буквы" << endl;
    }
    else {
        cout << "только русские буквы" << endl;
    }

    return 0;
}
