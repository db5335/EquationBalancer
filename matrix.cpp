
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "matrix.hpp"

void Matrix::swapRows(int row1, int row2) {
    if (row1 == row2) return;

    printf("R%d <-> Ra%d\n", row1, row2);
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

void Matrix::multiplyRow(int row, Fraction scalar) {
    printf("(%d/%d) R%d\n", scalar.getNum(), scalar.getDen(), row);
    for (int i = 0; i < cols; i++) {
        matrix[row][i].multiply(scalar);
    }
}

void Matrix::addRow(int row1, int row2, Fraction scalar) {
    printf("R%d + (%d/%d) R%d\n", row1, scalar.getNum(), scalar.getDen(), row2);
    for (int i = 0; i < cols; i++) {
        Fraction f(matrix[row2][i]);
        f.multiply(scalar);
        matrix[row1][i].add(f);
    }
}

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
                printMatrix();
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
                printMatrix();
                for (int k = i - 1; k >= 0; k--) {
                    Fraction f2(matrix[k][j]);
                    f2.multiply(Fraction(-1, 1));
                    addRow(k, i, f2);
                }
                printMatrix();
                break;
            }
        }
    }
}

void Matrix::setValue(char* atom, int col, int quantity) {
    for (int i = 0; i < rows; i++) {
        if (!strcmp(atoms[i], atom)) {
            Fraction f(quantity, 1);
            matrix[i][col] = f;
            return;
        }
    }
}

Fraction Matrix::getValue(int row, int col) {
    return matrix[row][col];
}

Matrix::Matrix(char** atoms, int rows, int cols) {
    this->rows = rows;
    this->cols = cols;
    this->atoms = atoms;
    matrix = (Fraction**) malloc(rows * sizeof(Fraction*));
    for (int i = 0; i < rows; i++) {
        matrix[i] = (Fraction*) malloc(cols * sizeof(Fraction));
    }
}

void Matrix::printMatrix() {
    printf("-----------------------------\n");
    for (int i = 0; i < rows; i++) {
        printf("%s\t", atoms[i]);
        for (int j = 0; j < cols; j++) {
            printf("%d/%d\t", matrix[i][j].getNum(),  matrix[i][j].getDen());
        }
        printf("\n");
    }
}

int main1() {
    char** atoms = (char**) malloc(3 * sizeof(char*));
    atoms[0] = (char*) malloc(5);
    sprintf(atoms[0], "Aa");
    atoms[1] = (char*) malloc(5);
    sprintf(atoms[1], "B");
    atoms[2] = (char*) malloc(5);
    sprintf(atoms[2], "Cc");

    Matrix m(atoms, 3, 4);
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            m.setValue(atoms[i], j, i + j + 3);
        }
        m.setValue(atoms[i], 3, 0);
    }
    m.reduce();
    m.printMatrix();
    return 0;
}
