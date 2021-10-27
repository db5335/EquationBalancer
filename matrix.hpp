#include "fraction.hpp"

class Matrix {
    private:
        int rows;
        int cols;
        char** atoms;
        Fraction** matrix;

    public:
        Matrix(char** atoms, int rows, int cols);
        Fraction getValue(int row, int col);
        void setValue(char* atom, int col, int quantity);
        void reduce();
        void printMatrix();
};
