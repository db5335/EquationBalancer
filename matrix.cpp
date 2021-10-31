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
                    f2.multiply(Fraction(-1, 1));
                    f2.multiply(Fraction(f1.getDen(), f1.getNum()));
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
                Fraction reciprocal(f1.getDen(), f1.getNum());
                multiplyRow(i, reciprocal);
                for (int k = i - 1; k >= 0; k--) {
                    Fraction f2(matrix[k][j]);
                    f2.multiply(Fraction(-1, 1));
                    addRow(k, i, f2);
                }
                break;
            }
        }
    }
}

/// Returns the simplest non-zero solution to the matrix.

int* Matrix::solve() {
    int* solution = (int*) malloc((cols - 1) * sizeof(int));
    for (int i = 0; i < cols - 1; i++) {
        solution[i] = -1;
    }

    for (int i = rows - 1; i >= 0; i--) {
        Fraction total(matrix[i][cols - 1]);
        for (int j = cols - 2; j >= 0; j--) {
            bool pivot = true;
            if (solution[j] >= 0) {
                Fraction f(matrix[i][j]);
                f.multiply(Fraction(solution[j], 1));
                total.add(f);
                continue;
            }
            for (int k = j - 1; k >= 0; k--) {
                if (!matrix[i][k].equals(0)) {
                    pivot = false;
                    break;
                }
            }
            if (!pivot) {
                Fraction f(matrix[i][j]);
                if (total.equals(0)) {
                    solution[j] = f.getDen();
                    total.add(Fraction(f.getNum(), 1));
                } else {
                    total.multiply(Fraction(f.getDen(), f.getNum()));
                    solution[j] = total.getNum();
                    int den = total.getDen();
                    if (den != 1) {
                        for (int k = j + 1; k < cols - 1; k++) {
                            solution[k] *= den;
                        }
                    }
                } 
            } else {
                Fraction f(matrix[i][j]);
                if (total.equals(0)) {
                    if (f.equals(0)) break;
                    solution[j] = 0;
                } else if (f.equals(0)) {
                    return NULL;
                } else {
                    total.multiply(Fraction(-1 * f.getDen(), f.getNum()));
                    solution[j] = total.getNum();
                    int den = total.getDen();
                    if (den != 1) {
                        for (int k = j + 1; k < cols - 1; k++) {
                            solution[k] *= den;
                        }
                    }
                }
                break;

            }
        }
    }

    return solution;
}

/// Sets the value of an entry in the matrix.

void Matrix::setValue(char* atom, int col, int quantity) {
    for (int i = 0; i < rows; i++) {
        if (!strcmp(atoms[i], atom)) {
            Fraction f(quantity, 1);
            matrix[i][col] = f;
            return;
        }
    }
}

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
