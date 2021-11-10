///
/// file: solution.hpp
/// Header file for the Solution class and Status enum
///
/// @author Dominick Banasik

#ifndef _SOLUTION_H_
#define _SOLUTION_H_

#include "fraction.hpp"

/// The Status enum represents the type of solution
/// that a chemical equation has.

enum Status {
    BALANCED,
    UNBALANCED,
    SOLVED,
    UNSOLVED
};

/// The Solution class represents the solution to
/// a chemical equation.

class Solution {
    private:
        Fraction* solution;
        Status status;

    public:
        /// Constructor for the Solution class.
        ///
        /// @param size the number of coefficients in the solution

        Solution(int size);
        
        /// Sets the status of the solution.
        ///
        /// @param status the updated status of the solution

        void setStatus(Status status);

        /// Sets a value for a coefficient in the solution.
        ///
        /// @param value the value to set
        /// @param index the index of the coefficient to set

        void setValue(Fraction value, int index);

        /// Returns a coefficient value from the solution.
        ///
        /// @param index the index of the coefficient to return
        /// @return the coefficient at the specified index

        Fraction getValue(int index);

        /// Returns the status of the solution.
        ///
        /// @return the status

        Status getStatus();
};

#endif
