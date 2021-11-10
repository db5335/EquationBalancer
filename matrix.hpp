///
/// file: matrix.hpp
/// Header file for the Matrix class
///
/// @author Dominick Banasik

#ifndef _MATRIX_H_
#define _MATRIX_H_

#include "fraction.hpp"
#include "solution.hpp"

/// The Matrix class represents a matrix where
/// each row corresponds to an atom and each
/// column corresponds to a molecule.

class Matrix {
    private:
        int rows;
        int cols;
        char** atoms;
        Fraction** matrix;

        /// Swaps two rows in the matrix.
        ///
        /// @param row1 the first row
        /// @param row2 the second row

        void swapRows(int row1, int row2);
        
        /// Multiplies a row by a scalar.
        ///
        /// @param row the row to multiply
        /// @param scalar the scalar to multiply by

        void multiplyRow(int row, Fraction scalar);

        /// Adds a multiple of one row to another.
        ///
        /// @param row1 the row to add to
        /// @param row2 the row to multiply and add
        /// @oaram scalar the scalar to multiply by

        void addRow(int row1, int row2, Fraction scalar);

    public:
        /// Constructor for the Matrix class.
        ///
        /// @param atoms the list of atoms
        /// @oaram rows the number of rows
        /// @param cols the number of columns

        Matrix(char** atoms, int rows, int cols);
        
        /// Returns the value of an entry in the matrix.
        ///
        /// @param row the row of the entry
        /// @param col the column of the entry
        /// @return the Fraction held in the cell

        Fraction getValue(int row, int col);

        /// Sets the value of an entry in the matrix.
        ///
        /// @param atom the atom row to set
        /// @param col the column to set
        /// @param quantity the value to set the cell to

        void setValue(char* atom, int col, int quantity);
        
        /// Row reduces the matrix to rref.

        void reduce();

        /// Returns the simplest non-zero solution to the matrix.
        ///
        /// @return the solution for the matrix

        Solution solve();

        /// Prints a representation of the entire matrix.

        void printMatrix();
};

#endif
