///
/// file: balancer.cpp
/// The main program for the equation balancer.
///
/// @author Dominick Banasik

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "molecule.hpp"
#include "matrix.hpp"
#include "solution.hpp"
#include "equation.hpp"

/// Prints a usage message.

void usage() {
    fprintf(stderr, "usage: ./balancer [-h]\n");
}

/// Prints a message explaining how to enter input.

void help() {
    printf("Enter an equation of the form:\n");
    printf("\t_H20 = _H2 + _O2\n");
}

/// Processes all command line flags.
///
/// @param argc the number of command line arguments
/// @param argv the array of command line arguments

void processFlags(int argc, char** argv) {
    int opt;

    while ((opt = getopt(argc, argv, "h")) != -1) {
        switch (opt) {
            case 'h':
                help();
                exit(0);
            default:
                usage();
                exit(1);
        }
    }
}

/// The main function...
///
/// @param argc the number of command line arguments
/// @param argv the array of command line arguments
/// @return EXIT_SUCCESS

int main(int argc, char** argv) {
    /// process command line flags
    processFlags(argc, argv);

    /// get input
    printf("Enter an equation to balance:\n");
    char string[256];
    fgets(string, sizeof(string), stdin);
 
    /// create a matrix from the equation
    Equation equation(string);
    Matrix matrix = equation.createMatrixFromEquation();

    /// balance the equation
    matrix.reduce();
    Solution solution = matrix.solve();
    equation.printSolution(solution);

    return 0;
}
