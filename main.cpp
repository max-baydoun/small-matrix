#include <iostream>
#include "SmallMatrix.hpp"

int main() {
    auto m1 = smallMatrix::SmallMatrix({{1 ,2, 3}, {4, 5, 6}});
    auto m2 = smallMatrix::SmallMatrix({{4}, {5}, {6}});
    auto mCrossMultiply = m1 * m2;
    std::cout << mCrossMultiply << std::endl;
}