#include "NumbersInASpiral.h"

std::vector<std::vector<int>> numbersInASpiral(int n)
{
    std::vector<std::vector<int>> matrix(n);
    for (int i = 0; i < matrix.size(); ++i) {
        matrix[i] = std::vector<int>(n);
    }

    int number = 1;
    for (int cycle = 0; cycle < (n / 2); ++cycle) {
        const int amortizeN = n - cycle;
        for (int i = n - amortizeN; i < amortizeN; ++i)
            matrix[cycle][i] = number++;
        for (int i = cycle + 1; i < amortizeN; ++i)
            matrix[i][amortizeN - 1] = number++;
        for (int i = amortizeN - 2; i >= n - amortizeN; --i)
            matrix[amortizeN - 1][i] = number++;
        for (int i = (amortizeN - 2); i > n - amortizeN; --i)
            matrix[i][cycle] = number++;
    }
    if (n % 2 == 1) {
        const int pos = n / 2;
        matrix[pos][pos] = number;
    }

    return matrix;
}
