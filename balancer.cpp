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

/// Adds a molecule to a group of reactants or products.
///
/// @param molecules the array of molecules to add to
/// @param count the number of molecules in the array
/// @param string the string from input
/// @param start the start of the molecule's substring
/// @param index the index after the substring's end
/// @param freeCount pointer to the number of molecules in the arrya without a fixed quantity
/// @return the updated array of molecules

Molecule* addMolecule(Molecule* molecules, int count, char* string, int start, int index, int* freeCount) {
    char* moleculeStr = (char*) malloc((index - start + 1) * sizeof(char));
    for (int i = 0; i < index - 1; i++) {
        moleculeStr[i - start] = string[i];
    }
    molecules = (Molecule*) realloc(molecules, count * sizeof(Molecule));
    Molecule m(moleculeStr);
    if (!m.getFixed()) {
        (*freeCount)++;
    }
    molecules[count - 1] = m;
    return molecules;
}

/// Parses a string of input into reactants and products.
///
/// @param string the string from input
/// @param reactantCount pointer to the number of reactants
/// @param productCount pointer to the number of products
/// @param freeReactantCount pointer to the number of reactants without a fixed quantity
/// @param freeProductCount pointer to the number of products without a fixed quantity
/// @return the array of reactants and the array of products

Molecule** parse(char* string, int* reactantCount, int* productCount, int* freeReactantCount, int* freeProductCount) {
    Molecule** molecules = (Molecule**) malloc(2 * sizeof(Molecule*));
    Molecule* reactants = (Molecule*) malloc(0);
    Molecule* products = (Molecule*) malloc(0);

    int start = 0;
    int index = 0;
    bool flip = false;
    char next;
    while (next = *(string + index++)) {
        if (next == '+') {
            if (!flip) {
                reactants = addMolecule(reactants, ++(*reactantCount), string, start, index, freeReactantCount);
            } else {
                products = addMolecule(products, ++(*productCount), string, start, index, freeProductCount);
                flip = false;
            }
            start = index;
        } else if (next == '-') {
            if (!flip) {
                reactants = addMolecule(reactants, ++(*reactantCount), string, start, index, freeReactantCount);
                flip = true;
            } else {
                products = addMolecule(products, ++(*productCount), string, start, index, freeProductCount);
            }
            start = index;
        } else if (next == '=') {
            if (!flip) {
                reactants = addMolecule(reactants, ++(*reactantCount), string, start, index, freeReactantCount);
            } else {
                products = addMolecule(products, ++(*productCount), string, start, index, freeProductCount);
                flip = false;
            }
            start = index;
            break;
        }
    }
    while (next = *(string + index++)) {
        if (next == '+') {
            if (!flip) {
                products = addMolecule(products, ++(*productCount), string, start, index, freeProductCount);
            } else {
                reactants = addMolecule(reactants, ++(*reactantCount), string, start, index, freeReactantCount);
                flip = false;
            }
            start = index;
        } else if (next == '-') {
            if (!flip) {
                products = addMolecule(products, ++(*productCount), string, start, index, freeProductCount);
                flip = true;
            } else {
                reactants = addMolecule(reactants, ++(*reactantCount), string, start, index, freeReactantCount);
            }
            start = index;
        }
    }
    
    if (!flip) {
            products = addMolecule(products, ++(*productCount), string, start, index, freeProductCount);
    } else {
        reactants = addMolecule(reactants, ++(*reactantCount), string, start, index, freeReactantCount);
    }
    
    molecules[0] = reactants;
    molecules[1] = products;

   return molecules;
}

/// Adds atoms from a list of molecules to a collective list.
///
/// @param atoms the list of atoms
/// @param numAtoms pointer to the number of total atoms
/// @param molecules the list of molecules
/// @param moleculeCount number of molecules
/// @return the updated list of atoms

char** addAtoms(char** atoms, int* numAtoms, Molecule* molecules, int moleculeCount) { 
    for (int i = 0; i < moleculeCount; i++) {
        Molecule molecule = molecules[i];
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

/// Fills a matrix with the entries that match the number of a
/// particular atom in one molecule.
///
/// @param m the matrix to fill
/// @param molecules the molecules to fill the matrix with
/// @param atoms the list of atoms
/// @param moleculeCount number of molecules
/// @param numAtoms number of atoms
/// @param offset the column to start filling the matrix from
/// @param pos whether or not the entries will be positive

void fillMatrix(Matrix m, Molecule* molecules, char** atoms, int moleculeCount, int numAtoms, int offset, bool pos) {
    int mult = pos ? 1 : -1;
    int index = offset;
    for (int i = 0; i < moleculeCount; i++) {
        Molecule molecule = molecules[i];
        if (!molecule.getFixed()) {
            for (int j = 0; j < numAtoms; j++) {
                char* atom = atoms[j];
                m.setValue(atom, index, mult * molecule.getCountOfAtom(atom));
            }
            index++;
        }
    }
}

/// Fills the last column of a matrix with 0's.
/// TODO
/// @param m the matrix to fill
/// @param atoms the list of atoms
/// @param numAtoms number of atoms
/// @oaram col the column of the matrix to fill

void fillLastColumn(Matrix m, Molecule** molecules, char** atoms, int reactantCount, int productCount, int numAtoms, int col) {
    for (int i = 0; i < numAtoms; i++) {
        m.setValue(atoms[i], col, 0);
    }

    for (int i = 0; i < reactantCount; i++) {
        Molecule molecule = molecules[0][i];
        if (molecule.getFixed()) {
            for (int j = 0; j < numAtoms; j++) {
                char* atom = atoms[j];
                Fraction f = m.getValue(j, col);
                m.setValue(atom, col, molecule.getCountOfAtom(atom) + f.getNum());
            }
        }
    }

    for (int i = 0; i < productCount; i++) {
        Molecule molecule = molecules[1][i];
        if (molecule.getFixed()) {
            for (int j = 0; j < numAtoms; j++) {
                char* atom = atoms[j];
                Fraction f = m.getValue(j, col);
                m.setValue(atom, col, -1 * molecule.getCountOfAtom(atom) + f.getNum());
            }
        }
    }
}

/// Prints the solution to the balanced equation.
///
/// @param solution the list of integer coefficients
/// @param size the number of coefficients

void printSolution(Fraction* solution, int size, Molecule** molecules, int reactantCount, int productCount) {
    if (solution == NULL) {
        printf("No solution\n");
        return;
    }

    int index = 0;
    
    for (int i = 0; i < reactantCount; i++) {
        Molecule reactant = molecules[0][i];
        if (reactant.getFixed()) {
            reactant.printMolecule();
        } else {
            printf("_");
            int num = solution[index].getNum();
            int den = solution[index++].getDen();
            if (num % den == 0) {
                printf("%d", num / den);
            } else {
                printf("%d/%d", num, den);
            }
            reactant.printMolecule();
        }
        if (i < reactantCount - 1) printf(" + ");
    }

    printf(" = ");

    for (int i = 0; i < productCount; i++) {
        Molecule product = molecules[1][i];
        if (product.getFixed()) {
            product.printMolecule();
        } else {
            printf("_");
            int num = solution[index].getNum();
            int den = solution[index++].getDen();
            if (num % den == 0) {
                printf("%d", num / den);
            } else {
                printf("%d/%d", num, den);
            }
            product.printMolecule();
        }
        if (i < productCount - 1) printf(" + ");
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
 
    Equation equation(string);
    char** atoms = equation.getAtoms();

    Matrix matrix = equation.createMatrixFromEquation();

    /// initialize a matrix
    ///Matrix matrix(atoms, numAtoms, freeReactantCount + freeProductCount + 1);
    ///fillMatrix(matrix, molecules[0], atoms, reactantCount, numAtoms, 0, true);
    ///fillMatrix(matrix, molecules[1], atoms, productCount, numAtoms, freeReactantCount, false);
    ///fillLastColumn(matrix, molecules, atoms, reactantCount, productCount, numAtoms, freeReactantCount + freeProductCount);

    /// balance the equation
    matrix.reduce();
    Solution solution = matrix.solve();
    equation.printSolution(solution);

    return 0;
}
