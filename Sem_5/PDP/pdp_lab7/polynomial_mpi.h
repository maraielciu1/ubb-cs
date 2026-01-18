#ifndef POLYNOMIAL_MPI_H
#define POLYNOMIAL_MPI_H

#include "polynomial.h"
#include <mpi.h>

// MPI Regular O(n²) polynomial multiplication
// Distributes result coefficients across MPI processes
Polynomial multiplyRegularMPI(const Polynomial& a, const Polynomial& b);

// MPI Karatsuba polynomial multiplication
// Distributes recursive calls across MPI processes
Polynomial multiplyKaratsubaMPI(const Polynomial& a, const Polynomial& b);

#endif // POLYNOMIAL_MPI_H

