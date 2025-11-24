#include <iostream>
#include <string.h>
#include <ctype.h>
#include <conio.h>

using namespace std;

// Функция для проверки, является ли символ буквой (русской или латинской)
bool isLetter(unsigned char c) {
    // Проверка латинских букв
    if ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z'))
        return true;

    // Проверка русских букв (для кодировки Windows-1251)
    if ((c >= 192 && c <= 255) || c == 168 || c == 184)
        return true;

    return false;
}

// Функция для проверки, состоит ли строка только из букв
bool containsOnlyLetters(const char* str) {
    for (int i = 0; str[i] != '\0'; i++) {
        if (!isLetter((unsigned char)str[i])) {
            return false;
        }
    }
    return true;
}

int main() {
    const int MAX_LENGTH = 100;
    char word[MAX_LENGTH];

    cout << "Введите слово: ";
    cin >> word;

    // Проверяем, содержит ли слово только буквы
    if (!containsOnlyLetters(word)) {
        cout << "Ошибка: слово должно содержать только русские и латинские буквы!" << endl;
        cout << "Обнаружены запрещенные символы!" << endl;
        return 1; // Завершаем программу с кодом ошибки
    }

    // Вычисляем длину слова
    int length = strlen(word);

    cout << "Длина слова: " << length << " символов" << endl;

    // Проверяем, кратно ли количество букв трем
    if (length % 3 == 0) {
        cout << "Слово состоит из количества букв, кратного трем (" << length << ")" << endl;
    }
    else {
        cout << "Слово состоит из количества букв, НЕ кратного трем (" << length << ")" << endl;
    }

    // Дополнительная информация о слове
    cout << "\nДополнительная информация:" << endl;
    cout << "Введенное слово: " << word << endl;

    // Определяем тип букв в слове
    bool hasLatin = false;
    bool hasCyrillic = false;

    for (int i = 0; word[i] != '\0'; i++) {
        unsigned char c = (unsigned char)word[i];
        if ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z')) {
            hasLatin = true;
        }
        else if ((c >= 192 && c <= 255) || c == 168 || c == 184) {
            hasCyrillic = true;
        }
    }

    if (hasLatin && hasCyrillic) {
        cout << "Слово содержит как латинские, так и русские буквы" << endl;
    }
    else if (hasLatin) {
        cout << "Слово содержит только латинские буквы" << endl;
    }
    else {
        cout << "Слово содержит только русские буквы" << endl;
    }

    cout << "\nНажмите любую клавишу для выхода...";
    getch();
    return 0;
}
