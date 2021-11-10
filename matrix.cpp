///
/// file: matrix.cpp
/// Implementation for the Matrix class
///
/// @author Dominick Banasik

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "matrix.hpp"

#ifndef _MATRIX_IMPL_
#define _MATRIX_IMPL_

/// Swaps two rows in the matrix.

void Matrix::swapRows(int row1, int row2) {
    if (row1 == row2) return;

    char* tmp = atoms[row1];
    atoms[row1] = atoms[row2];
    atoms[row2] = tmp;
    for (int i = 0; i < cols; i++) {
        Fraction tmpFrac1(matrix[row1][i]);
        Fraction tmpFrac2(matrix[row2][i]);
        matrix[row1][i] = tmpFrac2;
        matrix[row2][i] = tmpFrac1;
    }
}

/// Multiplies a row by a scalar.

void Matrix::multiplyRow(int row, Fraction scalar) {
    for (int i = 0; i < cols; i++) {
        matrix[row][i].multiply(scalar);
    }
}

/// Adds a multiple of one row to another.

void Matrix::addRow(int row1, int row2, Fraction scalar) {
    for (int i = 0; i < cols; i++) {
        Fraction f(matrix[row2][i]);
        f.multiply(scalar);
        matrix[row1][i].add(f);
    }
}

/// Row reduces the matrix to rref.

void Matrix::reduce() {
    int pivots = 0;
    for (int i = 0; i < cols; i++) {
        for (int j = pivots; j < rows; j++) {
            if (!matrix[j][i].equals(0)) {
                swapRows(pivots, j);
                Fraction f1 = matrix[pivots][i];
                for (int k = pivots + 1; k < rows; k++) {
                    Fraction f2(matrix[k][i]);
                    f2.multiply(-1);
                    f2.multiply(f1.getReciprocal());
                    addRow(k, pivots, f2);
                }
                pivots++;
                break;
            }
        }
    }

    for (int i = rows - 1; i >= 0; i--) {
        for (int j = 0; j < cols; j++) {
            if (!matrix[i][j].equals(0)) {
                Fraction f1 = matrix[i][j];
                multiplyRow(i, f1.getReciprocal());
                for (int k = i - 1; k >= 0; k--) {
                    Fraction f2(matrix[k][j]);
                    f2.multiply(-1);
                    addRow(k, i, f2);
                }
                break;
            }
        }
    }
}

/// Returns the simplest non-zero solution to the matrix

Solution Matrix::solve() {
    Solution solution(cols - 1);
    bool fixed = false;
    
    if (cols == 1) {
        for (int i = 0; i < rows; i++) {
            if (!matrix[i][0].equals(0)) {
                solution.setStatus(UNBALANCED);
                return solution;
            }
        }
        solution.setStatus(BALANCED);
        return solution;
    }

    for (int i = rows - 1; i >= 0; i--) {
        Fraction total(matrix[i][cols - 1]);
        if (!fixed) fixed = !total.equals(0);
        for (int j = cols - 2; j >= 0; j--) {
            Fraction current = solution.getValue(j);
            if (current.getNum() >= 0) {
                Fraction f(matrix[i][j]);
                f.multiply(current);
                total.add(f);
                continue;
            }
            bool pivot = true;
            for (int k = j - 1; k >= 0; k--) {
                if (!matrix[i][k].equals(0)) {
                    pivot = false;
                    break;
                }
            }
            if (!pivot) {
                Fraction f(matrix[i][j]);
                if (total.equals(0)) {
                    solution.setValue(Fraction(f.getDen()), j);
                    total.add(Fraction(f.getNum()));
                } else {
                    total.multiply(f.getReciprocal());
                    solution.setValue(Fraction(total.getNum()), j);
                    if (!fixed) {
                        int den = total.getDen();
                        if (den != 1) {
                            for (int k = j; k < cols - 1; k++) {
                                Fraction f = solution.getValue(k);
                                f.multiply(Fraction(den));
                                solution.setValue(f, k);
                            }
                        }
                    }
                } 
            } else {
                Fraction f(matrix[i][j]);
                if (total.equals(0)) {
                    if (f.equals(0)) break;
                    solution.setValue(Fraction(0), j);
                } else if (f.equals(0)) {
                    solution.setStatus(UNSOLVED);
                    return solution;
                } else {
                    Fraction reciprocal = f.getReciprocal();
                    reciprocal.multiply(-1);
                    total.multiply(reciprocal);
                    solution.setValue(Fraction(total), j);
                    if (!fixed) {
                        int den = total.getDen();
                        if (den != 1) {
                            for (int k = j; k < cols - 1; k++) {
                                Fraction f = solution.getValue(k);
                                f.multiply(Fraction(den));
                                solution.setValue(f, k);
                            }
                        }
                    }
                }
                break;

            }
        }
    }
    
    solution.setStatus(SOLVED);
    return solution;
}

/// Sets the value of an entry in the matrix.

void Matrix::setValue(char* atom, int col, int quantity) {
    for (int i = 0; i < rows; i++) {
        if (!strcmp(atoms[i], atom)) {
            Fraction f(quantity);
            matrix[i][col] = f;
            return;
        }
    }
}

/// Prints a representation of the entire matrix.

void Matrix::printMatrix() {
    printf("==========MATRIX==========\n");
    for (int i = 0; i < rows; i++) {
        printf("%s:\t", atoms[i]);
        for (int j = 0; j < cols; j++) {
            printf("%d/%d\t", matrix[i][j].getNum(), matrix[i][j].getDen());
        }
        printf("\n");
    }
}

/// Returns the value of an entry in the matrix.

Fraction Matrix::getValue(int row, int col) {
    return matrix[row][col];
}

/// Constructor for the Matrix class.

Matrix::Matrix(char** atoms, int rows, int cols) {
    this->rows = rows;
    this->cols = cols;
    this->atoms = atoms;
    matrix = (Fraction**) malloc(rows * sizeof(Fraction*));
    for (int i = 0; i < rows; i++) {
        matrix[i] = (Fraction*) malloc(cols * sizeof(Fraction));
    }
}

#endif
