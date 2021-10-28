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

/// Adds a molecule to a group of reactants or products.
///
/// @param molecules the array of molecules to add to
/// @param count the number of molecules in the array
/// @param string the string from input
/// @param start the start of the molecule's substring
/// @param index the index after the substring's end
/// @return the updated array of molecules

Molecule* addMolecule(Molecule* molecules, int count, char* string, int start, int index) {
    char* moleculeStr = (char*) malloc((index - start + 1) * sizeof(char));
    for (int i = 0; i < index - 1; i++) {
        moleculeStr[i - start] = string[i];
    }
    molecules = (Molecule*) realloc(molecules, count * sizeof(Molecule));
    Molecule m(moleculeStr);
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
            reactants = addMolecule(reactants, ++(*reactantCount), string, start, index);
            start = index;
        } else if (next == '=') {
            reactants = addMolecule(reactants, ++(*reactantCount), string, start, index);
            start = index;
            break;
        }
    }
    while (next = *(string + index++)) {
        if (next == '+') {
            products = addMolecule(products, ++(*productCount), string, start, index);
            start = index;
        }
    }
    products = addMolecule(products, ++(*productCount), string, start, index);
    
    molecules[0] = reactants;
    molecules[1] = products;

   return molecules;
}

char** addAtoms(char** atoms, int* numAtoms, Molecule* molecules, int moleculeCount) { 
    for (int i = 0; i < moleculeCount; i++) {
        printf("Molecule %d:\n", i);
        Molecule molecule = molecules[i];
        molecule.printAtoms();
        int moleculeAtomCount = molecule.getSize();
        char** moleculeAtoms = molecule.getAtoms();
        for (int j = 0; j < moleculeAtomCount; j++) {
            bool duplicate = false;
            for (int k = 0; k < *numAtoms; k++) {
                if (!strcmp(atoms[k], moleculeAtoms[j])) {
                    duplicate = true;
                    break;
                }
            }
            if (!duplicate) {
                atoms = (char**) realloc(atoms, (*numAtoms + 1) * sizeof(char*));
                atoms[*numAtoms] = (char*) malloc(3 * sizeof(char));
                strcpy(atoms[*numAtoms], moleculeAtoms[j]);
                *numAtoms += 1;
            }
        }
    }
    return atoms;
}

void fillMatrix(Matrix m, Molecule* molecules, char** atoms, int moleculeCount, int numAtoms, int offset, bool pos) {
    int mult = pos ? 1 : -1;
    for (int i = 0; i < moleculeCount; i++) {
        Molecule molecule = molecules[i];
        for (int j = 0; j < numAtoms; j++) {
            char* atom = atoms[j];
            m.setValue(atom, i + offset, mult * molecule.getCountOfAtom(atom));
        }
    }
}

void fillLastColumn(Matrix m, char** atoms, int numAtoms, int col) {
    for (int i = 0; i < numAtoms; i++) {
        m.setValue(atoms[i], col, 0);
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
 
    int reactantCount = 0;
    int productCount = 0;

    Molecule** molecules = parse(string, &reactantCount, &productCount);
    int numAtoms = 0;
    char** atoms = (char**) malloc(0);

    atoms = addAtoms(atoms, &numAtoms, molecules[0], reactantCount);
    atoms = addAtoms(atoms, &numAtoms, molecules[1], productCount);

    Matrix matrix(atoms, numAtoms, reactantCount + productCount + 1);
    fillMatrix(matrix, molecules[0], atoms, reactantCount, numAtoms, 0, true);
    fillMatrix(matrix, molecules[1], atoms, productCount, numAtoms, reactantCount, false);
    fillLastColumn(matrix, atoms, numAtoms, reactantCount + productCount);
    matrix.printMatrix();
    matrix.reduce();
    matrix.printMatrix();
    matrix.solve();

    return 0;
}
