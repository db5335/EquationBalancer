#include "fraction.hpp"

class Matrix {
    private:
        int rows;
        int cols;
        char** atoms;
        Fraction** matrix;
        void swapRows(int row1, int row2);
        void multiplyRow(int row, Fraction scalar);
        void addRow(int row1, int row2, Fraction scalar);

    public:
        Matrix(char** atoms, int rows, int cols);
        Fraction getValue(int row, int col);
        void setValue(char* atom, int col, int quantity);
        void reduce();
        int* solve();
};
