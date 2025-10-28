#include <iostream>
#include <vector>

using namespace std;

long long countRoutes(int n, int m) {
    vector<vector<long long>> dp(n + 1, vector<long long>(m + 1, 0));

    for (int i = 0; i <= n; i++) {
        dp[i][0] = 1;
    }
    for (int j = 0; j <= m; j++) {
        dp[0][j] = 1;
    }

    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= m; j++) {
            dp[i][j] = dp[i - 1][j] + dp[i][j - 1];
        }
    }

    return dp[n][m];
}

int main() {
    int n, m;

    cout << "Введите размеры таблицы (n m): ";
    cin >> n >> m;

    long long routes = countRoutes(n, m);

    cout << "Количество маршрутов в таблице " << n << "x" << m << ": " << routes << endl;

    // Покажем результат для задания (20x20)
    cout << "\nРезультат для задания (20x20): " << countRoutes(20, 20) << endl;

    return 0;
}
