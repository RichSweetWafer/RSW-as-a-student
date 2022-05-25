#include "boost.h"

Matrix transpose(Matrix& A)
{
    Matrix B(A.size2(), A.size1());
    for (size_t i = 0; i < A.size1(); i++)
        for (size_t j = 0; j < A.size2(); j++)
            B(j, i) = A(i, j);
    return B;
}
