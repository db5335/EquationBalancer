///
/// file: balancer.cpp
/// The main program for the equation balancer.
///
/// @author Dominick Banasik

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "molecule.hpp"

/// Prints a usage message

void usage() {
    fprintf(stderr, "usage: ./balancer [-h]\n");
}

/// Prints a message explaining how to enter input

void help() {
    printf("Enter an equation of the form:\n");
    printf("\t_H20 = _H2 + _O2\n");
}

/// Processes all command line flags
///
/// @param argc the number of command line arguments
/// @param argv the array of command line arguments

void process_flags(int argc, char** argv) {
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

Molecule** parse(char* string) {
    int reactantCount = 0;
    int productCount = 0;
    int start = 0;
    int index = 0;
    char next;
    while (next = *(string + index++)) {
        if (next == '+') {
            char* reactant = (char*) malloc((index - start + 1) * sizeof(char));
            for (int i = start; i < index - 1; i++) {
                reactant[i - start] = string[i];
            }
            start = index;
        } else if (next == '=') {
            char* reactant = (char*) malloc((index - start + 1) * sizeof(char));
            for (int i = start; i < index - 1; i++) {
                reactant[i - start] = string[i];
            }
            start = index;
            break;
        }
    }
    while (next = *(string + index++)) {
        if (next == '+') {
            char* product = (char*) malloc((index - start + 1) * sizeof(char));
            for (int i = start; i < index - 1; i++) {
                product[i - start] = string[i];
            }
            start = index;
        }
    }
    char* product = (char*) malloc((index - start + 1) * sizeof(char));
    for (int i = start; i < index - 1; i++) {
        product[i - start] = string[i];
    }
    return NULL;
}

/// The main function...
///
/// @param argc the number of command line arguments
/// @param argv the array of command line arguments
/// @return EXIT_SUCCESS

int main(int argc, char** argv) {
    /// process command line flags
    process_flags(argc, argv);

    /// get input
    printf("Enter an equation to balance:\n");
    char string[256];
    fgets(string, sizeof(string), stdin);
 
    Molecule** molecules = parse(string);

    return 0;
}
