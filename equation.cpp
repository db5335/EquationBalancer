#include <string.h>
#include <stdio.h>

#include "equation.hpp"

#ifndef _EQUATION_IMPL_
#define _EQUATION_IMPL_

void Equation::fillLastColumn(Matrix matrix) {
    int col = freeReactantCount + freeProductCount;

    for (int i = 0; i < atomCount; i++) {
        matrix.setValue(atoms[i], col, 0);
    }

    for (int i = 0; i < reactantCount; i++) {
        Molecule molecule = reactants[i];
        if (molecule.getFixed()) {
            for (int j = 0; j < atomCount; j++) {
                char* atom = atoms[j];
                Fraction f = matrix.getValue(j, col);
                matrix.setValue(atom, col, molecule.getCountOfAtom(atom) + f.getNum());
            }
        }
    }

    for (int i = 0; i < productCount; i++) {
        Molecule molecule = products[i];
        if (molecule.getFixed()) {
            for (int j = 0; j < atomCount; j++) {
                char* atom = atoms[j];
                Fraction f = matrix.getValue(j, col);
                matrix.setValue(atom, col, -1 * molecule.getCountOfAtom(atom) + f.getNum());
            }
        }
    }
}

void Equation::fillMatrix(Matrix matrix, bool isReactant) {
    int mult = isReactant ? 1 : -1;
    int index = isReactant ? 0 : freeReactantCount;
    int moleculeCount = isReactant ? reactantCount : productCount;
    Molecule* molecules = isReactant ? reactants : products;
    
    for (int i = 0; i < moleculeCount; i++) {
        Molecule molecule = molecules[i];
        if (!molecule.getFixed()) {
            for (int j = 0; j < atomCount; j++) {
                char* atom = atoms[j];
                matrix.setValue(atom, index, mult * molecule.getCountOfAtom(atom));
            }
            index++;
        }
    }
}

Matrix Equation::createMatrixFromEquation() {
    Matrix matrix(atoms, atomCount, freeReactantCount + freeProductCount + 1);
    fillMatrix(matrix, true);
    fillMatrix(matrix, false);
    fillLastColumn(matrix);
    return matrix;
}

void Equation::generateAtoms(bool isReactant) {
    int moleculeCount = reactantCount;
    Molecule* molecules = reactants;

    if (!isReactant) {
        moleculeCount = productCount;
        molecules = products;
    }

    for (int i = 0; i < moleculeCount; i++) {
        Molecule molecule = molecules[i];
        int moleculeAtomCount = molecule.getSize();
        char** moleculeAtoms = molecule.getAtoms();

        for (int j = 0; j < moleculeAtomCount; j++) {
            bool duplicate = false;

            for (int k = 0; k < atomCount; k++) {
                if (!strcmp(atoms[k], moleculeAtoms[j])) {
                    duplicate = true;
                }
            }

            if (!duplicate) {
                if (atomCount == atomCapacity) {
                    atomCapacity += CAPACITY;
                    atoms = (char**) realloc(atoms, atomCapacity * sizeof(char*));
                }
                atoms[atomCount] = (char*) malloc(ATOM_SIZE * sizeof(char));
                strcpy(atoms[atomCount++], moleculeAtoms[j]);
            }
        }
    }
}

char** Equation::getAtoms() {
    return atoms;
}

void Equation::addMolecule(char* string, int start, int index, bool isReactant) {
    char* moleculeStr = (char*) malloc((index - start + 1) * sizeof(char));

    for (int i = 0; i < index - 1; i++) {
        moleculeStr[i - start] = string[i]; 
    }

    int* moleculeCount = &reactantCount;
    int* freeMoleculeCount = &freeReactantCount;
    int* moleculeCapacity = &reactantCapacity;
    Molecule* molecules = reactants;
    if (!isReactant) {
        moleculeCount = &productCount;
        freeMoleculeCount = &freeProductCount;
        moleculeCapacity = &productCapacity;
        molecules = products;
    }

    if (*moleculeCount == *moleculeCapacity) {
        *moleculeCapacity += CAPACITY;
        molecules = (Molecule*) realloc(molecules, *moleculeCapacity * sizeof(Molecule));
    }

    Molecule molecule(moleculeStr);
        
    if (!molecule.getFixed()) (*freeMoleculeCount)++;
    molecules[(*moleculeCount)++] = molecule;
}

void Equation::parse(char* string) {
    int start = 0;
    int index = 0;
    bool flip = false;
    char next;

    while (next = *(string + index++)) {
        if (next == '+' || next == '=') {
            if (!flip) {
                addMolecule(string, start, index, true);
            } else {
                addMolecule(string, start, index, false);
                flip = false;
            }
            start = index;
            if (next == '=') break;
        } else if (next == '-') {
            if (!flip) {
                addMolecule(string, start, index, true);
                flip = true;
            } else {
                addMolecule(string, start, index, false);
            }
            start = index;
        }
    }

    while (next = *(string + index++)) {
        if (next == '+') {
            if (!flip) {
                addMolecule(string, start, index, false);
            } else {
                addMolecule(string, start, index, true);
                flip = false;
            }
            start = index;
        } else if (next == '-') {
            if (!flip) {
                addMolecule(string, start, index, false);
                flip = true;
            } else {
                addMolecule(string, start, index, true);
            }
            start = index;
        }
    }

    if (!flip) {
        addMolecule(string, start, index, false);
    } else {
        addMolecule(string, start, index, true);
    }
}

void Equation::printSolution(Solution solution) {
    if (solution.getStatus() == SOLVED) {
        int index = 0;
        
        for (int i = 0; i < reactantCount; i++) {
            Molecule reactant = reactants[i];
            if (reactant.getFixed()) {
                reactant.printMolecule();
            } else {
                printf("_");
                int num = solution.getValue(index).getNum();
                int den = solution.getValue(index++).getDen();
                if (num % den) {
                    printf("%d/%d", num, den);
                } else {
                    printf("%d", num / den);
                }
                reactant.printMolecule();
            }
            if (i < reactantCount - 1) printf(" + ");
        }
        
        printf(" = ");

        for (int i = 0; i < productCount; i++) {
            Molecule product = products[i];
            if (product.getFixed()) {
                product.printMolecule();
            } else {
                printf("_");
                int num = solution.getValue(index).getNum();
                int den = solution.getValue(index++).getDen();
                if (num % den) {
                    printf("%d/%d", num, den);
                } else {
                    printf("%d", num / den);
                }
                product.printMolecule();
            }
            if (i < productCount - 1) printf(" + ");
        }
    } else if (solution.getStatus() == UNSOLVED) {
        printf("The equation has no solution\n");
    } else if (solution.getStatus() == BALANCED) {
        printf("The equation is already balanced\n");
    } else if (solution.getStatus() == UNBALANCED) {
        printf("The equation is unbalanced\n");
    }
}

Equation::Equation(char* string) {
    reactantCapacity = CAPACITY;
    productCapacity = CAPACITY;
    atomCapacity = CAPACITY;
    reactantCount = 0;
    productCount = 0;
    freeReactantCount = 0;
    freeProductCount = 0;
    atomCount = 0;

    reactants = (Molecule*) malloc(reactantCapacity * sizeof(Molecule));
    products = (Molecule*) malloc(productCapacity * sizeof(Molecule));
    atoms = (char**) malloc(atomCapacity * sizeof(char*));

    parse(string);
    generateAtoms(true);
    generateAtoms(false);

}

#endif
