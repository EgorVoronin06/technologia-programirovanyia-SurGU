#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <locale>
#include <windows.h>
#include <algorithm>

using namespace std;

// Функция для проверки, является ли символ концом предложения
bool isSentenceEnd(char c) {
    return c == '.' || c == '!' || c == '?';
}

// Функция для проверки, является ли символ разделителем
bool isDelimiter(char c) {
    return c == ' ' || c == '\t' || c == '\n' || c == '\r' || c == ',' || c == ';' || c == ':';
}

// Функция для изменения файла согласно ТЗ
void modifyFile(const string& filename, const vector<string>& questions, const vector<string>& exclamations, const vector<string>& other) {
    string output_filename = "modified_" + filename;

    ofstream out_file(output_filename);
    if (!out_file.is_open()) {
        cout << "Ошибка: Не удалось создать файл '" << output_filename << "'" << endl;
        return;
    }

    // Записываем заголовок
    out_file << " РЕЗУЛЬТАТЫ АНАЛИЗА ФАЙЛА: " << filename << " ===" << endl;
    out_file << "Сначала выводятся вопросительные предложения, затем восклицательные" << endl;
    out_file << "                                                                 " << endl;

    // Записываем вопросительные предложения
    out_file << "\n ВОПРОСИТЕЛЬНЫЕ ПРЕДЛОЖЕНИЯ (" << questions.size() << ") ---" << endl;
    for (size_t i = 0; i < questions.size(); i++) {
        out_file << questions[i] << endl;
    }

    // Записываем восклицательные предложения
    out_file << "\n ВОСКЛИЦАТЕЛЬНЫЕ ПРЕДЛОЖЕНИЯ (" << exclamations.size() << ") ---" << endl;
    for (size_t i = 0; i < exclamations.size(); i++) {
        out_file << exclamations[i] << endl;
    }

    // Записываем остальные предложения (опционально)
    out_file << "\n ОСТАЛЬНЫЕ ПРЕДЛОЖЕНИЯ (" << other.size() << ") ---" << endl;
    for (size_t i = 0; i < other.size(); i++) {
        out_file << other[i] << endl;
    }

    // Статистика в конце
    out_file << "\nСТАТИСТИКА" << endl;
    out_file << "Всего предложений: " << (questions.size() + exclamations.size() + other.size()) << endl;
    out_file << "Вопросительных: " << questions.size() << endl;
    out_file << "Восклицательных: " << exclamations.size() << endl;
    out_file << "Остальных: " << other.size() << endl;

    out_file.close();
    cout << "Файл успешно изменен и сохранен как: " << output_filename << endl;
}

// Функция для обработки текста и разделения на предложения
void processText(const string& filename) {
    ifstream file(filename, ios::binary); // Открываем файл в бинарном режиме

    if (!file.is_open()) {
        cout << "Ошибка: Не удалось открыть файл '" << filename << "'" << endl;
        return;
    }

    vector<string> questions;    // Вопросительные предложения
    vector<string> exclamations; // Восклицательные предложения
    vector<string> other;        // Остальные предложения

    string line;
    string currentSentence;
    bool inQuote = false;
    int lineCount = 0;

    // Чтение файла построчно
    while (getline(file, line)) {
        lineCount++;

        // Пропускаем пустые строки и римские цифры (разделы)
        if (line.empty() ||
            (line.length() <= 3 &&
                (line.find("I.") != string::npos ||
                    line.find("II.") != string::npos ||
                    line.find("III.") != string::npos ||
                    line.find("IV.") != string::npos ||
                    line.find("V.") != string::npos ||
                    line.find("VI.") != string::npos ||
                    line.find("VII.") != string::npos ||
                    line.find("VIII.") != string::npos ||
                    line.find("IX.") != string::npos ||
                    line.find("X.") != string::npos ||
                    line.find("XI.") != string::npos ||
                    line.find("XII.") != string::npos ||
                    line.find("XIII.") != string::npos ||
                    line.find("XIV.") != string::npos ||
                    line.find("XV.") != string::npos ||
                    line.find("XVI.") != string::npos ||
                    line.find("XVII.") != string::npos))) {
            continue;
        }

        // Обработка многоточий в начале строк (пропускаем их)
        if (line.find("...") == 0) {
            continue;
        }

        for (size_t i = 0; i < line.length(); i++) {
            char c = line[i];

            // Обработка кавычек
            if (c == '\"' || c == '\'' || c == '«' || c == '»') {
                inQuote = !inQuote;
            }

            currentSentence += c;

            // Проверка конца предложения (только если не внутри кавычек)
            if (!inQuote && isSentenceEnd(c)) {
                // Удаляем лишние пробелы в начале и конце
                size_t start = currentSentence.find_first_not_of(" \t\n\r");
                size_t end = currentSentence.find_last_not_of(" \t\n\r");

                if (start != string::npos && end != string::npos) {
                    string cleanSentence = currentSentence.substr(start, end - start + 1);

                    // Классифицируем предложение по последнему символу
                    if (!cleanSentence.empty()) {
                        char lastChar = cleanSentence.back();
                        if (lastChar == '?') {
                            questions.push_back(cleanSentence);
                        }
                        else if (lastChar == '!') {
                            exclamations.push_back(cleanSentence);
                        }
                        else {
                            other.push_back(cleanSentence);
                        }
                    }
                }

                currentSentence.clear();
            }
        }

        // Если предложение продолжается на следующей строке, добавляем пробел
        if (!currentSentence.empty() && !isDelimiter(currentSentence.back())) {
            currentSentence += ' ';
        }
    }

    // Обработка последнего предложения, если файл не заканчивается знаком препинания
    if (!currentSentence.empty()) {
        size_t start = currentSentence.find_first_not_of(" \t\n\r");
        size_t end = currentSentence.find_last_not_of(" \t\n\r");

        if (start != string::npos && end != string::npos) {
            string cleanSentence = currentSentence.substr(start, end - start + 1);
            if (!cleanSentence.empty()) {
                other.push_back(cleanSentence);
            }
        }
    }

    file.close();

    // Вывод результатов на экран
    cout << "ВОПРОСИТЕЛЬНЫЕ ПРЕДЛОЖЕНИЯ " << endl;
    if (questions.empty()) {
        cout << "Вопросительные предложения не найдены." << endl;
    }
    else {
        for (size_t i = 0; i < questions.size(); i++) {
            cout << (i + 1) << ". " << questions[i] << endl;
        }
    }

    cout << "\nВОСКЛИЦАТЕЛЬНЫЕ ПРЕДЛОЖЕНИЯ " << endl;
    if (exclamations.empty()) {
        cout << "Восклицательные предложения не найдены." << endl;
    }
    else {
        for (size_t i = 0; i < exclamations.size(); i++) {
            cout << (i + 1) << ". " << exclamations[i] << endl;
        }
    }

    // Статистика
    cout << "\n СТАТИСТИКА " << endl;
    cout << "Вопросительных предложений: " << questions.size() << endl;
    cout << "Восклицательных предложений: " << exclamations.size() << endl;
    cout << "Других предложений: " << other.size() << endl;
    cout << "Всего предложений: " << (questions.size() + exclamations.size() + other.size()) << endl;

    // Предлагаем изменить файл
    char choice;
    cout << "\nХотите сохранить результат в файл? (y/n): ";
    cin >> choice;

    if (choice == 'y' || choice == 'Y') {
        modifyFile(filename, questions, exclamations, other);
    }
}

int main() {
    // Настройка консоли для поддержки русского языка
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    setlocale(LC_ALL, "Russian");

    cout << "=== ПРОГРАММА ДЛЯ АНАЛИЗА ПРЕДЛОЖЕНИЙ ===" << endl;
    cout << "Программа выводит сначала вопросительные, затем восклицательные предложения" << endl;
    cout << "Анализ текста 'Евгения Онегина'" << endl;

    string filename;
    cout << "\nВведите путь к файлу: ";
    getline(cin, filename);

    // Удаляем кавычки если они есть (на случай drag&drop)
    if (!filename.empty() && filename[0] == '\"' && filename[filename.length() - 1] == '\"') {
        filename = filename.substr(1, filename.length() - 2);
    }

    // Если путь не введен, используем файл по умолчанию
    if (filename.empty()) {
        filename = "index.txt";
    }

    processText(filename);

    cout << "\nДля выхода нажмите Enter...";
    cin.ignore(); // Игнорируем предыдущий ввод
    cin.get();

    return 
