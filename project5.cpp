#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <ctime>
#include <limits>
#include <string>

using namespace std;

const int SIZE = 8;

void processMatrix(int matrix[SIZE][SIZE], int threshold, int& sum) {
    sum = 0;
    for (int i = 0; i < SIZE; i++) {
        if (matrix[i][i] > threshold) {
            sum += matrix[i][i];
        }
    }
}

int main() {
    srand(time(0));
    int matrix[SIZE][SIZE];
    int threshold;
    int sum;
    string input;

    // Fill matrix with random values in range [0, 150]
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            matrix[i][j] = rand() % 151;
        }
    }

    cout << "Original Matrix:" << endl;
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            cout << setw(4) << matrix[i][j];
        }
        cout << endl;
    }

    // Robust input validation
    while (true) {
        cout << "\nEnter threshold number: ";
        getline(cin, input);

        try {
            threshold = stoi(input);
            break; // Exit loop if conversion successful
        }
        catch (const invalid_argument& e) {
            cout << "Error: Invalid input. Please enter a valid integer." << endl;
        }
        catch (const out_of_range& e) {
            cout << "Error: Number is out of range. Please enter a smaller number." << endl;
        }
    }

    processMatrix(matrix, threshold, sum);

    cout << "\nResults:" << endl;
    cout << "Sum of diagonal elements greater than " << threshold << ": " << sum << endl;

    // Show which diagonal elements were included
    cout << "Diagonal elements included in sum: ";
    bool found = false;
    for (int i = 0; i < SIZE; i++) {
        if (matrix[i][i] > threshold) {
            cout << "A[" << i << "][" << i << "]=" << matrix[i][i] << " ";
            found = true;
        }
    }
    if (!found) {
        cout << "none";
    }
    cout << endl;

    return 0;
}
