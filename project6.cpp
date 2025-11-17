#include <iostream>
#include <vector>

using namespace std;

// Метод 1: Динамическое программирование
unsigned long long calculateRoutesDP(int gridSize) {
    vector<vector<unsigned long long>> dp(gridSize + 1,
        vector<unsigned long long>(gridSize + 1, 0));

    // Базовые случаи:
    // - только 1 способ добраться до любой точки на верхней строке (только вправо)
    // - только 1 способ добраться до любой точки на левом столбце (только вниз)
    for (int i = 0; i <= gridSize; i++) {
        dp[i][0] = 1;
        dp[0][i] = 1;
    }

    // Заполняем таблицу:
    // количество путей до [i][j] = пути сверху + пути слева
    for (int i = 1; i <= gridSize; i++) {
        for (int j = 1; j <= gridSize; j++) {
            dp[i][j] = dp[i - 1][j] + dp[i][j - 1];
        }
    }

    return dp[gridSize][gridSize];
}

// Метод 2: Комбинаторика (биномиальные коэффициенты)
unsigned long long calculateRoutesCombinatorics(int gridSize) {
    unsigned long long result = 1;
    int n = gridSize;

    // Вычисляем C(2n, n) = (2n)! / (n! × n!)
    // Упрощенная формула: ∏(i=1 to n) [(n + i) / i]
    for (int i = 1; i <= n; i++) {
        result = result * (n + i) / i;
    }

    return result;
}

int main() {
    int gridSize = 20;

    cout << "=== Grid Routes Calculation ===" << endl;
    cout << "Grid size: " << gridSize << "x" << gridSize << endl;
    cout << endl;

    // Метод 1: Динамическое программирование
    cout << "Method 1 - Dynamic Programming:" << endl;
    unsigned long long routesDP = calculateRoutesDP(gridSize);
    cout << "Number of routes: " << routesDP << endl;
    cout << endl;

    // Метод 2: Комбинаторика
    cout << "Method 2 - Combinatorics:" << endl;
    unsigned long long routesComb = calculateRoutesCombinatorics(gridSize);
    cout << "Number of routes: " << routesComb << endl;
    cout << endl;

    // Проверка
    cout << "=== Verification ===" << endl;
    cout << "Both methods give identical result: "
        << (routesDP == routesComb ? "YES" : "NO") << endl;
    cout << endl;

    // Демонстрация для маленьких сеток
    cout << "=== Demonstration for smaller grids ===" << endl;
    for (int i = 1; i <= 5; i++) {
        unsigned long long smallResult = calculateRoutesCombinatorics(i);
        cout << i << "x" << i << " grid: " << smallResult << " routes" << endl;
    }
    cout << endl;

    // Финальный ответ
    cout << "=== Final Answer ===" << endl;
    cout << "Number of routes in a " << gridSize << "x" << gridSize
        << " grid: " << routesComb << endl;

    return 0;
}
