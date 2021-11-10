///
/// file: solution.cpp
/// Implementation for the Solution class
///
/// @author Dominick Banasik

#include <stdio.h>
#include <stdlib.h>

#include "solution.hpp"
#include "fraction.hpp"

#ifndef _SOLUTION_IMPL_
#define _SOLUTION_IMPL_

/// Constructor for the Solution class.

Solution::Solution(int size) {
    solution = (Fraction*) malloc(size * sizeof(Fraction));

    for (int i = 0; i < size; i++) {
        solution[i] = Fraction(-1, 1);
    }
}

/// Sets the status of the solution.

void Solution::setStatus(Status status) {
    this->status = status;
}

/// Sets a value for a coefficient in the solution.

void Solution::setValue(Fraction value, int index) {
    solution[index] = value;
}

/// Returns the status of the solution.

Status Solution::getStatus() {
    return status;
}

/// Returns a coefficient value from the solution.

Fraction Solution::getValue(int index) {
    return solution[index];
}

#endif
