///
/// file: balancer.cpp
/// The main program for the equation balancer.
///
/// @author Dominick Banasik

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "molecule.hpp"

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

/// Adds a molecule to a group of reactants or products.
///
/// @param molecules the array of molecules to add to
/// @param count the number of molecules in the array
/// @param string the string from input
/// @param start the start of the molecule's substring
/// @param index the index after the substring's end
/// @return the updated array of molecules

Molecule* add_molecule(Molecule* molecules, int count, char* string, int start, int index) {
    char* molecule_string = (char*) malloc((index - start + 1) * sizeof(char));
    for (int i = 0; i < index - 1; i++) {
        molecule_string[i - start] = string[i];
    }
    molecules = (Molecule*) realloc(molecules, count * sizeof(Molecule));
    Molecule m(molecule_string);
    molecules[count - 1] = m;
    return molecules;
}

/// Parses a string of input into reactants and products.
///
/// @param string the string from input
/// @param reactantCount pointer to the number of reactants
/// @param productCount pointer to the number of products
/// @return the array of reactants and the array of products

Molecule** parse(char* string, int* reactantCount, int* productCount) {
    Molecule** molecules = (Molecule**) malloc(2 * sizeof(Molecule*));
    Molecule* reactants = (Molecule*) malloc(0);
    Molecule* products = (Molecule*) malloc(0);

    int start = 0;
    int index = 0;
    char next;
    while (next = *(string + index++)) {
        if (next == '+') {
            reactants = add_molecule(reactants, ++(*reactantCount), string, start, index);
            start = index;
        } else if (next == '=') {
            reactants = add_molecule(reactants, ++(*reactantCount), string, start, index);
            start = index;
            break;
        }
    }
    while (next = *(string + index++)) {
        if (next == '+') {
            products = add_molecule(products, ++(*productCount), string, start, index);
            start = index;
        }
    }
    products = add_molecule(products, ++(*productCount), string, start, index);
    
    molecules[0] = reactants;
    molecules[1] = products;

   return molecules;
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
 
    int reactantCount = 0;
    int productCount = 0;

    Molecule** molecules = parse(string, &reactantCount, &productCount);

    for (int i = 0; i < reactantCount; i++) {
        printf("Reactant %d:\n\n", i);
        molecules[0][i].printAtoms();
    }
    for (int i = 0; i < productCount; i++) {
        printf("Product %d:\n\n", i);
        molecules[1][i].printAtoms();
    }


    return 0;
}
