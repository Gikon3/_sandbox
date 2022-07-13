#include <iostream>
#include <iomanip>
#include "numbersInASpiral.h"

static void printMatrix(const std::vector<std::vector<int>>& matrix)
{
    int div = matrix.size() * matrix.size() / 10;
    int w = 1;
    while (div != 0) {
        div /= 10;
        ++w;
    }

    for (int j = 0; j < matrix.size(); ++j) {
        for (int i = 0; i < matrix[0].size(); ++i) {
            std::cout << std::setw(w) << matrix[j][i] << " ";
        }
        std::cout << std::endl;
    }
}

int main()
{
    for (int i = 0; i <= 10; ++i) {
        printMatrix(numbersInASpiral(i));
        std::cout << std::endl;
    }

    return 0;
}