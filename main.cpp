#include <iostream>
#include <string>
#include <vector>
#include <cctype>
#include <algorithm>
#include <windows.h> // Для работы с кодировкой в Windows

using namespace std;

// Установка кодировки для Windows
void setRussianEncoding() {
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
}

// Функция для проверки, является ли символ русской заглавной буквой (Windows-1251)
bool isRussianUpper(char c) {
    unsigned char uc = static_cast<unsigned char>(c);
    // Русские заглавные буквы в Windows-1251: А(192)-Я(223), Ё(168)
    return (uc >= 192 && uc <= 223) || uc == 168;
}

// Функция для проверки, является ли символ русской строчной буквой (Windows-1251)
bool isRussianLower(char c) {
    unsigned char uc = static_cast<unsigned char>(c);
    // Русские строчные буквы в Windows-1251: а(224)-я(255), ё(184)
    return (uc >= 224 && uc <= 255) || uc == 184;
}

// Функция для проверки, является ли символ русской буквой (любого регистра)
bool isRussianLetter(char c) {
    return isRussianUpper(c) || isRussianLower(c);
}

// Функция для проверки, является ли символ английской буквой
bool isEnglishLetter(char c) {
    return (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z');
}

// Функция для проверки, является ли символ буквой (русской или английской)
bool isLetter(char c) {
    return isRussianLetter(c) || isEnglishLetter(c);
}

// Функция для приведения русской буквы к нижнему регистру (Windows-1251)
char toLowerRussian(char c) {
    unsigned char uc = static_cast<unsigned char>(c);

    // Русские заглавные буквы А-Я (кроме Ё)
    if (uc >= 192 && uc <= 223 && uc != 168) {
        return static_cast<char>(uc + 32);
    }
    // Буква Ё
    else if (uc == 168) {
        return static_cast<char>(184);
    }
    // Английские заглавные буквы
    else if (c >= 'A' && c <= 'Z') {
        return c + 32;
    }

    return c;
}

// Функция для приведения строки к нижнему регистру (с поддержкой русского)
string toLowerString(const string& str) {
    string result = str;
    for (char& c : result) {
        c = toLowerRussian(c);
    }
    return result;
}

// Проверка, является ли строка фамилией
bool isSurname(const string& word) {
    if (word.empty() || word.length() < 2) {
        return false;
    }

    // Первый символ должен быть заглавной буквой
    char first = word[0];
    bool firstIsUpper = isRussianUpper(first) || (first >= 'A' && first <= 'Z');

    if (!firstIsUpper) {
        return false;
    }

    // Проверяем остальные символы
    for (size_t i = 1; i < word.length(); i++) {
        char c = word[i];
        // Разрешаем буквы, дефис и апостроф
        if (!isLetter(c) && c != '-' && c != '\'') {
            return false;
        }
    }

    return true;
}

// Проверка, начинается ли фамилия на слог "Ма"
bool startsWithMa(const string& surname) {
    if (surname.length() < 2) return false;

    // Приводим к нижнему регистру для проверки
    string lower = toLowerString(surname);

    // Проверяем первые два символа
    if (lower.length() >= 2) {
        // Для русского
        if (lower.substr(0, 2) == "ма") {
            return true;
        }
        // Для английского
        if (lower.substr(0, 2) == "ma") {
            return true;
        }
    }

    return false;
}

// Функция для отображения кодов символов (для отладки)
void debugString(const string& str) {
    cout << "Строка: " << str << endl;
    cout << "Коды символов: ";
    for (char c : str) {
        cout << static_cast<int>(static_cast<unsigned char>(c)) << " ";
    }
    cout << endl;

    cout << "Первый символ: '" << str[0] << "' (код: "
        << static_cast<int>(static_cast<unsigned char>(str[0])) << ")" << endl;
    cout << "isRussianUpper: " << (isRussianUpper(str[0]) ? "ДА" : "нет") << endl;
    cout << "isEnglishLetter: " << (isEnglishLetter(str[0]) ? "ДА" : "нет") << endl;
}

int main() {
    // Устанавливаем русскую кодировку для Windows
    setRussianEncoding();

    cout << "=== ЛАБОРАТОРНАЯ РАБОТА №7 - ФИЛЬТР ФАМИЛИЙ ===\n\n";
    cout << "Задание: Найти в списке из 5 фамилий все фамилии, начинающиеся со слога 'Ма'\n\n";

    vector<string> allSurnames;
    vector<string> maSurnames;

    int totalSurnames = 0;
    const int REQUIRED = 5;

    cout << "Введите 5 фамилий (русские или английские):\n";
    cout << "Правила:\n";
    cout << "1. Первая буква должна быть заглавной\n";
    cout << "2. Можно использовать буквы, дефис (-) и апостроф (')\n";
    cout << "3. Минимальная длина - 2 символа\n\n";

    cout << "Примеры правильных фамилий:\n";
    cout << "✓ Иванов, Петров, Сидоров, Макаров, Мамаев\n";
    cout << "✓ Martin, Mason, O'Connor, van-Dyck\n\n";

    // Для отладки: показываем коды символов для примера
    cout << "Для справки: коды русских заглавных букв в Windows-1251:\n";
    cout << "И: " << static_cast<int>(static_cast<unsigned char>('И')) << endl;
    cout << "М: " << static_cast<int>(static_cast<unsigned char>('М')) << endl;
    cout << "А: " << static_cast<int>(static_cast<unsigned char>('А')) << endl;
    cout << endl;

    while (totalSurnames < REQUIRED) {
        cout << "[" << (totalSurnames + 1) << "/" << REQUIRED << "] ";
        cout << "Фамилия: ";

        string input;
        getline(cin, input);

        if (input.empty()) {
            cout << "⚠ Ввод пустой. Пожалуйста, введите фамилию.\n\n";
            continue;
        }

        // Отладка (раскомментировать при необходимости)
        // debugString(input);

        // Проверяем, является ли ввод фамилией
        if (isSurname(input)) {
            allSurnames.push_back(input);
            totalSurnames++;

            // Проверяем, начинается ли на "Ма"
            if (startsWithMa(input)) {
                maSurnames.push_back(input);
                cout << "✓ Принято (начинается на 'Ма')\n\n";
            }
            else {
                cout << "✓ Принято\n\n";
            }
        }
        else {
            cout << "✗ ОШИБКА: Введенная строка не соответствует критериям фамилии\n";

            // Детальная диагностика
            if (input.length() < 2) {
                cout << "  Причина: слишком короткая (минимум 2 символа)\n";
            }
            else {
                char first = input[0];
                unsigned char firstCode = static_cast<unsigned char>(first);

                cout << "  Первый символ: '" << first << "'\n";
                cout << "  Код первого символа: " << static_cast<int>(firstCode) << "\n";

                if (!isRussianUpper(first) && !(first >= 'A' && first <= 'Z')) {
                    cout << "  Причина: первая буква должна быть заглавной\n";
                    cout << "  Проверка isRussianUpper: " << (isRussianUpper(first) ? "ДА" : "нет") << "\n";
                    cout << "  Проверка isEnglishLetter (заглавная): "
                        << ((first >= 'A' && first <= 'Z') ? "ДА" : "нет") << "\n";
                }
            }

            cout << "  Пожалуйста, попробуйте снова.\n\n";
        }
    }

    // Вывод результатов
    cout << "===============================================\n";
    cout << "РЕЗУЛЬТАТЫ:\n";
    cout << "===============================================\n\n";

    cout << "1. Все введенные фамилии (" << allSurnames.size() << "):\n";
    for (size_t i = 0; i < allSurnames.size(); i++) {
        cout << "   " << (i + 1) << ") " << allSurnames[i];
        if (startsWithMa(allSurnames[i])) {
            cout << " ← начинается на 'Ма'";
        }
        cout << endl;
    }

    cout << "\n2. Фамилии, начинающиеся на слог 'Ма':\n";
    if (maSurnames.empty()) {
        cout << "   Не найдено ни одной фамилии, начинающейся на 'Ма'\n";
    }
    else {
        cout << "   Найдено " << maSurnames.size() << " фамилий:\n";
        for (size_t i = 0; i < maSurnames.size(); i++) {
            cout << "   " << (i + 1) << ") " << maSurnames[i] << endl;
        }
    }

    cout << "\n3. Статистика:\n";
    cout << "   - Всего фамилий: " << allSurnames.size() << endl;
    cout << "   - Начинаются на 'Ма': " << maSurnames.size() << endl;
    cout << "   - Не начинаются на 'Ма': " << (allSurnames.size() - maSurnames.size()) << endl;

    return 0;
}
