#include <iostream>
#include <string>
#include <cstring>
#include <locale>
#include <windows.h>

using namespace std;

// Проверка, является ли символ буквой (латиница или кириллица Windows-1251)
bool isLetter(unsigned char c) {
    // Английские буквы
    if ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z'))
        return true;
    // Русские буквы (включая Ёё)
    if ((c >= 192 && c <= 255) || c == 168 || c == 184)
        return true;
    return false;
}

// Проверка на допустимые символы в фамилии (буквы, дефисы, апострофы, пробелы)
bool isValidSurname(const char* str) {
    for (int i = 0; str[i] != '\0'; i++) {
        unsigned char c = (unsigned char)str[i];
        if (!isLetter(c) && c != '-' && c != '\'' && c != ' ') {
            return false;
        }
    }
    return true;
}

// Проверка, начинается ли фамилия со слога "Ма" (русское или английское)
bool startsWithMa(const char* str) {
    if (strlen(str) < 2) {
        return false;
    }
    
    unsigned char first = (unsigned char)str[0];
    unsigned char second = (unsigned char)str[1];
    
    // Русское "Ма" (М=204, м=236, А=192, а=224 в Windows-1251)
    bool russianMa = ((first == 204 || first == 236) && (second == 192 || second == 224));
    
    // Английское "Ma" 
    bool englishMa = ((first == 'M' || first == 'm') && (second == 'A' || second == 'a'));
    
    return russianMa || englishMa;
}

int main() {
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    setlocale(LC_ALL, "Russian");

    const int MAX_SURNAMES = 5;
    const int MAX_LENGTH = 100;
    char surnames[MAX_SURNAMES][MAX_LENGTH];
    bool isValid[MAX_SURNAMES];

    cout << "=== ЗАДАНИЕ ===" << endl;
    cout << "Найти в списке из 5 фамилий все фамилии, начинающиеся со слога «Ма»." << endl;
    cout << "\nВведите " << MAX_SURNAMES << " фамилий:" << endl;
    
    // Ввод фамилий и проверка их валидности
    for (int i = 0; i < MAX_SURNAMES; i++) {
        cout << "Фамилия " << (i + 1) << ": ";
        cin.getline(surnames[i], MAX_LENGTH);
        
        // Проверка на пустую строку
        if (strlen(surnames[i]) == 0) {
            cout << "  ОШИБКА: Пустой ввод недопустим" << endl;
            isValid[i] = false;
            continue;
        }
        
        // Проверка на допустимые символы
        if (!isValidSurname(surnames[i])) {
            cout << "  ОШИБКА: Фамилия содержит недопустимые символы (цифры или специальные символы)" << endl;
            isValid[i] = false;
        } else {
            isValid[i] = true;
        }
    }

    cout << "\n=== РЕЗУЛЬТАТЫ ===" << endl;
    
    // Поиск фамилий, начинающихся со слога "Ма" среди валидных
    cout << "Фамилии, начинающиеся со слога 'Ма':" << endl;
    bool found = false;
    int validCount = 0;
    
    for (int i = 0; i < MAX_SURNAMES; i++) {
        if (isValid[i]) {
            validCount++;
            if (startsWithMa(surnames[i])) {
                cout << "- " << surnames[i] << " (Фамилия " << (i + 1) << ")" << endl;
                found = true;
            }
        }
    }
    
    if (!found) {
        cout << "Фамилий, начинающихся со слога 'Ма', не найдено." << endl;
    }

    // Дополнительная информация о валидных фамилиях
    if (validCount > 0) {
        cout << "\n=== ДОПОЛНИТЕЛЬНАЯ ИНФОРМАЦИЯ ===" << endl;
        cout << "Корректные фамилии (" << validCount << " из " << MAX_SURNAMES << "):" << endl;
        
        for (int i = 0; i < MAX_SURNAMES; i++) {
            if (isValid[i]) {
                int letterCount = 0;
                bool hasLatin = false;
                bool hasCyrillic = false;
                
                // Подсчет букв и определение типа букв
                for (int j = 0; surnames[i][j] != '\0'; j++) {
                    unsigned char c = (unsigned char)surnames[i][j];
                    if (isLetter(c)) {
                        letterCount++;
                        if ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z')) {
                            hasLatin = true;
                        } else {
                            hasCyrillic = true;
                        }
                    }
                }
                
                cout << "Фамилия '" << surnames[i] << "': " << letterCount << " букв, ";
                if (hasLatin && hasCyrillic) {
                    cout << "смешанный алфавит" << endl;
                } else if (hasLatin) {
                    cout << "латинские буквы" << endl;
                } else {
                    cout << "русские буквы" << endl;
                }
            }
        }
    }

    // Информация о некорректных фамилиях
    if (validCount < MAX_SURNAMES) {
        cout << "\n=== НЕКОРРЕКТНЫЕ ФАМИЛИИ ===" << endl;
        for (int i = 0; i < MAX_SURNAMES; i++) {
            if (!isValid[i]) {
                cout << "Фамилия " << (i + 1) << ": '" << surnames[i] << "' - содержит ошибку" << endl;
            }
        }
    }

    return 0;
}
