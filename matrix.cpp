
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "matrix.hpp"

void Matrix::reduce() {
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
    for (int i = 0; i < rows; i++) {
        printf("%s\t", atoms[i]);
        for (int j = 0; j < cols; j++) {
            printf("%d ", matrix[i][j].getNum() / matrix[i][j].getDen());
        }
        printf("\n");
    }
}

int main() {
    char** atoms = (char**) malloc(3 * sizeof(char*));
    atoms[0] = (char*) malloc(5);
    sprintf(atoms[0], "Aa");
    atoms[1] = (char*) malloc(5);
    sprintf(atoms[1], "B");
    atoms[2] = (char*) malloc(5);
    sprintf(atoms[2], "Cc");

    Matrix m(atoms, 3, 3);
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            m.setValue(atoms[i], j, i * j);
        }
    }

    m.printMatrix();
}
