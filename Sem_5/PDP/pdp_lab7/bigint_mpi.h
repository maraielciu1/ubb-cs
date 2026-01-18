#ifndef BIGINT_MPI_H
#define BIGINT_MPI_H

#include "bigint.h"
#include <mpi.h>

// MPI Regular O(n²) big number multiplication
// Distributes multiplication work across MPI processes
BigInt multiplyBigIntRegularMPI(const BigInt& a, const BigInt& b);

// MPI Karatsuba big number multiplication
// Uses Karatsuba algorithm with MPI distribution
BigInt multiplyBigIntKaratsubaMPI(const BigInt& a, const BigInt& b);

#endif // BIGINT_MPI_H

