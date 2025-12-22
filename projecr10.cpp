#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <algorithm>
#include <iomanip>

using namespace std;

// ================ 1. ОПРЕДЕЛЕНИЕ СТРУКТУРЫ ================
struct Establishment {
    string type;      // тип заведения: "ресторан", "гостиница", "магазин"
    string name;      // название
    string address;   // адрес
    string phone;     // телефон

    // Конструктор для удобного создания объектов
    Establishment(string t = "", string n = "", string a = "", string p = "")
        : type(t), name(n), address(a), phone(p) {
    }
};

// ================ 2. ГЛОБАЛЬНЫЕ ПЕРЕМЕННЫЕ ================
vector<Establishment> database;  // основная база данных
vector<Establishment> queryResult; // результаты последнего запроса

// ================ 3. ВСПОМОГАТЕЛЬНЫЕ ФУНКЦИИ ================

// Функция 1: Вывод одной записи
// Вход: константная ссылка на Establishment (чтение без изменения)
// Выход: нет (void)
// Задача: форматированный вывод одной записи
void printRecord(const Establishment& record) {
    cout << left << setw(15) << record.type
        << setw(25) << record.name
        << setw(35) << record.address
        << setw(15) << record.phone << endl;
}

// Функция 2: Вывод заголовка таблицы
// Вход: нет
// Выход: нет (void)
// Задача: вывод заголовков колонок
void printHeader() {
    cout << "\n" << string(90, '=') << endl;
    cout << left << setw(15) << "Тип"
        << setw(25) << "Название"
        << setw(35) << "Адрес"
        << setw(15) << "Телефон" << endl;
    cout << string(90, '-') << endl;
}

// ================ 4. ОСНОВНЫЕ ФУНКЦИИ РАБОТЫ С БД ================

// Функция 3: Добавление новой записи
// Вход: нет (данные запрашиваются у пользователя)
// Выход: нет (void), но изменяет глобальную базу данных
// Задача: добавление одной записи в конец вектора
void addRecord() {
    Establishment newRecord;

    cout << "\n=== ДОБАВЛЕНИЕ НОВОЙ ЗАПИСИ ===" << endl;

    // Выбор типа заведения с валидацией
    int choice;
    do {
        cout << "Выберите тип заведения:\n";
        cout << "1. Ресторан\n2. Гостиница\n3. Магазин\n";
        cout << "Ваш выбор (1-3): ";
        cin >> choice;
        cin.ignore(); // очистка буфера

        switch (choice) {
        case 1: newRecord.type = "Ресторан"; break;
        case 2: newRecord.type = "Гостиница"; break;
        case 3: newRecord.type = "Магазин"; break;
        default: cout << "Неверный выбор! Попробуйте снова.\n";
        }
    } while (choice < 1 || choice > 3);

    // Ввод остальных данных с проверкой на пустоту
    do {
        cout << "Название: ";
        getline(cin, newRecord.name);
        if (newRecord.name.empty()) cout << "Название не может быть пустым!\n";
    } while (newRecord.name.empty());

    do {
        cout << "Адрес: ";
        getline(cin, newRecord.address);
        if (newRecord.address.empty()) cout << "Адрес не может быть пустым!\n";
    } while (newRecord.address.empty());

    do {
        cout << "Телефон: ";
        getline(cin, newRecord.phone);
        if (newRecord.phone.empty()) cout << "Телефон не может быть пустым!\n";
    } while (newRecord.phone.empty());

    // Добавление в базу
    database.push_back(newRecord);
    cout << "Запись успешно добавлена!\n";
}

// Функция 4: Удаление записи
// Вход: индекс записи для удаления (int)
// Выход: true если удаление успешно, false если нет
// Задача: удаление записи по индексу с проверкой границ
bool deleteRecord(int index) {
    if (index < 0 || index >= (int)database.size()) {
        cout << "Неверный индекс записи!\n";
        return false;
    }

    // Подтверждение удаления
    cout << "\nВы действительно хотите удалить запись?\n";
    printHeader();
    printRecord(database[index]);

    char confirm;
    cout << "Подтвердите удаление (y/n): ";
    cin >> confirm;

    if (confirm == 'y' || confirm == 'Y') {
        database.erase(database.begin() + index);
        cout << "Запись удалена!\n";
        return true;
    }
    cout << "Удаление отменено.\n";
    return false;
}

// Функция 5: Показать все записи
// Вход: нет
// Выход: нет (void)
// Задача: вывод всей базы данных с нумерацией
void showAllRecords() {
    if (database.empty()) {
        cout << "\nБаза данных пуста!\n";
        return;
    }

    cout << "\n=== ВСЕ ЗАПИСИ В БАЗЕ ДАННЫХ ===";
    printHeader();

    for (size_t i = 0; i < database.size(); i++) {
        cout << setw(3) << right << i + 1 << ". ";
        printRecord(database[i]);
    }
    cout << string(90, '=') << endl;
}

// ================ 5. ФУНКЦИИ ЗАПРОСОВ (ФИЛЬТРАЦИИ) ================

// Функция 6: Запрос 1 - Все гостиницы (ОСНОВНОЙ ЗАПРОС)
// Вход: нет
// Выход: вектор Establishment (результаты запроса)
// Задача: фильтрация записей по типу "Гостиница"
vector<Establishment> getAllHotels() {
    vector<Establishment> result;

    // Анализ входных данных: 
    // - база данных должна содержать записи
    // - тип сравнивается без учета регистра

    // Фильтрация
    for (const auto& record : database) {
        // Приведение к нижнему регистру для сравнения
        string typeLower = record.type;
        transform(typeLower.begin(), typeLower.end(), typeLower.begin(), ::tolower);

        if (typeLower == "гостиница") {
            result.push_back(record);
        }
    }

    return result;
}

// Функция 7: Запрос 2 - Заведения по улице (ДОПОЛНИТЕЛЬНЫЙ ЗАПРОС 1)
// Вход: строка с названием улицы (const string&)
// Выход: вектор Establishment
// Задача: поиск записей, адрес которых содержит указанную улицу
vector<Establishment> getByStreet(const string& streetName) {
    vector<Establishment> result;

    // Анализ входных данных:
    // - streetName не должна быть пустой
    // - поиск подстроки без учета регистра

    if (streetName.empty()) {
        cout << "Ошибка: название улицы не может быть пустым!\n";
        return result;
    }

    // Приведение к нижнему регистру для поиска
    string streetLower = streetName;
    transform(streetLower.begin(), streetLower.end(), streetLower.begin(), ::tolower);

    for (const auto& record : database) {
        string addressLower = record.address;
        transform(addressLower.begin(), addressLower.end(), addressLower.begin(), ::tolower);

        // Поиск подстроки в адресе
        if (addressLower.find(streetLower) != string::npos) {
            result.push_back(record);
        }
    }

    return result;
}

// Функция 8: Запрос 3 - Заведения по типу (ДОПОЛНИТЕЛЬНЫЙ ЗАПРОС 2)
// Вход: тип заведения (const string&)
// Выход: вектор Establishment
// Задача: фильтрация по точному совпадению типа
vector<Establishment> getByType(const string& type) {
    vector<Establishment> result;

    // Анализ входных данных:
    // - type должен быть одним из: "Ресторан", "Гостиница", "Магазин"
    // - проверка валидности типа

    vector<string> validTypes = { "Ресторан", "Гостиница", "Магазин",
                                 "ресторан", "гостиница", "магазин" };

    bool valid = false;
    for (const auto& validType : validTypes) {
        if (type == validType) {
            valid = true;
            break;
        }
    }

    if (!valid) {
        cout << "Ошибка: неверный тип заведения!\n";
        cout << "Допустимые типы: Ресторан, Гостиница, Магазин\n";
        return result;
    }

    // Фильтрация
    for (const auto& record : database) {
        // Сравнение без учета регистра
        string recordTypeLower = record.type;
        string searchTypeLower = type;
        transform(recordTypeLower.begin(), recordTypeLower.end(), recordTypeLower.begin(), ::tolower);
        transform(searchTypeLower.begin(), searchTypeLower.end(), searchTypeLower.begin(), ::tolower);

        if (recordTypeLower == searchTypeLower) {
            result.push_back(record);
        }
    }

    return result;
}

// Функция 9: Вывод результатов запроса
// Вход: вектор Establishment (результаты запроса) и заголовок (const string&)
// Выход: нет (void)
// Задача: форматированный вывод результатов и сохранение в queryResult
void printQueryResult(const vector<Establishment>& result, const string& title) {
    if (result.empty()) {
        cout << "\nПо вашему запросу ничего не найдено.\n";
        return;
    }

    // Сохраняем результаты для возможного сохранения в файл
    queryResult = result;

    cout << "\n=== " << title << " ===";
    printHeader();

    for (size_t i = 0; i < result.size(); i++) {
        cout << setw(3) << right << i + 1 << ". ";
        printRecord(result[i]);
    }

    cout << "\nНайдено записей: " << result.size() << endl;
    cout << string(90, '=') << endl;
}

// ================ 6. ФУНКЦИИ РАБОТЫ С ФАЙЛАМИ ================

// Функция 10: Сохранение всей БД в файл
// Вход: имя файла (const string&)
// Выход: true если успешно, false если ошибка
// Задача: сохранение всех записей в текстовый файл
bool saveDatabaseToFile(const string& filename) {
    ofstream file(filename);

    if (!file.is_open()) {
        cout << "Ошибка: не удалось открыть файл для записи!\n";
        return false;
    }

    // Заголовок файла
    file << "Тип;Название;Адрес;Телефон\n";

    // Сохранение записей
    for (const auto& record : database) {
        file << record.type << ";"
            << record.name << ";"
            << record.address << ";"
            << record.phone << "\n";
    }

    file.close();
    cout << "База данных сохранена в файл: " << filename << endl;
    cout << "Сохранено записей: " << database.size() << endl;
    return true;
}

// Функция 11: Загрузка БД из файла
// Вход: имя файла (const string&)
// Выход: true если успешно, false если ошибка
// Задача: загрузка записей из файла с проверкой формата
bool loadDatabaseFromFile(const string& filename) {
    ifstream file(filename);

    if (!file.is_open()) {
        cout << "Ошибка: не удалось открыть файл для чтения!\n";
        return false;
    }

    // Очистка текущей базы
    database.clear();

    string line;
    getline(file, line); // пропускаем заголовок

    int loadedCount = 0;
    int errorCount = 0;

    while (getline(file, line)) {
        // Парсинг строки формата: Тип;Название;Адрес;Телефон
        size_t pos1 = line.find(';');
        size_t pos2 = line.find(';', pos1 + 1);
        size_t pos3 = line.find(';', pos2 + 1);

        if (pos1 == string::npos || pos2 == string::npos || pos3 == string::npos) {
            errorCount++;
            continue; // пропускаем некорректные строки
        }

        Establishment record;
        record.type = line.substr(0, pos1);
        record.name = line.substr(pos1 + 1, pos2 - pos1 - 1);
        record.address = line.substr(pos2 + 1, pos3 - pos2 - 1);
        record.phone = line.substr(pos3 + 1);

        // Проверка на пустые поля
        if (record.type.empty() || record.name.empty() ||
            record.address.empty() || record.phone.empty()) {
            errorCount++;
            continue;
        }

        database.push_back(record);
        loadedCount++;
    }

    file.close();
    cout << "Загружено записей: " << loadedCount << endl;
    if (errorCount > 0) {
        cout << "Пропущено некорректных записей: " << errorCount << endl;
    }
    return true;
}

// Функция 12: Сохранение результатов запроса в файл
// Вход: имя файла (const string&)
// Выход: true если успешно, false если ошибка
// Задача: сохранение queryResult в отдельный файл
bool saveQueryToFile(const string& filename) {
    if (queryResult.empty()) {
        cout << "Ошибка: нет результатов запроса для сохранения!\n";
        cout << "Сначала выполните запрос.\n";
        return false;
    }

    ofstream file(filename);

    if (!file.is_open()) {
        cout << "Ошибка: не удалось открыть файл для записи!\n";
        return false;
    }

    // Заголовок файла
    file << "=== РЕЗУЛЬТАТЫ ЗАПРОСА ===\n";
    file << "Тип;Название;Адрес;Телефон\n";

    // Сохранение записей
    for (const auto& record : queryResult) {
        file << record.type << ";"
            << record.name << ";"
            << record.address << ";"
            << record.phone << "\n";
    }

    file.close();
    cout << "Результаты запроса сохранены в файл: " << filename << endl;
    cout << "Сохранено записей: " << queryResult.size() << endl;
    return true;
}

// ================ 7. ГЛАВНОЕ МЕНЮ И ОСНОВНАЯ ФУНКЦИЯ ================

// Функция 13: Отображение главного меню
// Вход: нет
// Выход: выбранный пункт меню (int)
// Задача: вывод меню и валидация ввода
int showMainMenu() {
    int choice;

    cout << "\n=== ГОРОДСКАЯ СПРАВКА ===" << endl;
    cout << "1. Показать все записи" << endl;
    cout << "2. Добавить новую запись" << endl;
    cout << "3. Удалить запись" << endl;
    cout << "4. Запросы (фильтрация):" << endl;
    cout << "   41. Все гостиницы (основной запрос)" << endl;
    cout << "   42. Заведения на указанной улице" << endl;
    cout << "   43. Заведения по типу" << endl;
    cout << "5. Сохранить всю БД в файл" << endl;
    cout << "6. Загрузить БД из файла" << endl;
    cout << "7. Сохранить результаты запроса в файл" << endl;
    cout << "8. Выход" << endl;
    cout << "Выберите действие: ";

    cin >> choice;
    cin.ignore(); // очистка буфера

    return choice;
}

// Функция 14: Обработка подменю запросов
// Вход: нет
// Выход: нет (void)
// Задача: выполнение выбранного запроса
void handleQueriesMenu() {
    int subChoice;
    cout << "\n=== ВЫБОР ЗАПРОСА ===" << endl;
    cout << "1. Все гостиницы" << endl;
    cout << "2. Заведения на указанной улице" << endl;
    cout << "3. Заведения по типу" << endl;
    cout << "4. Назад в главное меню" << endl;
    cout << "Выберите запрос: ";

    cin >> subChoice;
    cin.ignore();

    switch (subChoice) {
    case 1: {
        vector<Establishment> hotels = getAllHotels();
        printQueryResult(hotels, "ВСЕ ГОСТИНИЦЫ");
        break;
    }
    case 2: {
        string street;
        cout << "Введите название улицы: ";
        getline(cin, street);

        vector<Establishment> byStreet = getByStreet(street);
        printQueryResult(byStreet, "ЗАВЕДЕНИЯ НА УЛИЦЕ: " + street);
        break;
    }
    case 3: {
        string type;
        cout << "Введите тип заведения (Ресторан/Гостиница/Магазин): ";
        getline(cin, type);

        vector<Establishment> byType = getByType(type);
        printQueryResult(byType, "ЗАВЕДЕНИЯ ТИПА: " + type);
        break;
    }
    case 4:
        return; // выход в главное меню
    default:
        cout << "Неверный выбор!\n";
    }
}

// Основная функция
int main() {
    setlocale(LC_ALL, "Russian");

    // Добавим тестовые данные для демонстрации
    database.push_back(Establishment("Гостиница", "Европа", "ул. Ленина, 25", "+7(123)456-78-90"));
    database.push_back(Establishment("Ресторан", "Итальянский дворик", "пр. Мира, 15", "+7(234)567-89-01"));
    database.push_back(Establishment("Магазин", "Продукты 24/7", "ул. Советская, 10", "+7(345)678-90-12"));
    database.push_back(Establishment("Гостиница", "Звездная", "ул. Гагарина, 33", "+7(456)789-01-23"));
    database.push_back(Establishment("Ресторан", "Восточная кухня", "ул. Ленина, 48", "+7(567)890-12-34"));

    cout << "=== ПРОГРАММА 'ГОРОДСКАЯ СПРАВКА' ===\n";
    cout << "Загружено тестовых записей: " << database.size() << endl;

    bool running = true;

    while (running) {
        int choice = showMainMenu();

        switch (choice) {
        case 1:
            showAllRecords();
            break;

        case 2:
            addRecord();
            break;

        case 3: {
            showAllRecords();
            if (!database.empty()) {
                int index;
                cout << "Введите номер записи для удаления: ";
                cin >> index;
                cin.ignore();
                deleteRecord(index - 1); // -1 т.к. пользователь видит с 1
            }
            break;
        }

        case 4:
            handleQueriesMenu();
            break;

        case 5: {
            string filename;
            cout << "Введите имя файла для сохранения (например: database.csv): ";
            getline(cin, filename);
            saveDatabaseToFile(filename);
            break;
        }

        case 6: {
            string filename;
            cout << "Введите имя файла для загрузки: ";
            getline(cin, filename);
            loadDatabaseFromFile(filename);
            break;
        }

        case 7: {
            string filename;
            cout << "Введите имя файла для сохранения результатов: ";
            getline(cin, filename);
            saveQueryToFile(filename);
            break;
        }

        case 8:
            cout << "Выход из программы...\n";
            running = false;
            break;

        default:
            cout << "Неверный выбор! Попробуйте снова.\n";
        }
    }

    return 0;
}
