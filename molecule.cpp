///
/// file: molecule.cpp
/// Implementation for the Molecule class
///
/// @author Dominick Banasik

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "molecule.hpp"

#ifndef _MOLECULE_IMPL_
#define _MOLECULE_IMPL_

/// Adds atoms to the molecule's atom count.

void Molecule::addAtoms(char* current, int multiplier) {
    for (int i = 0; i < size; i++) {
        if (!strcmp(current, atoms[i])) {
            counts[i] += multiplier;
            return;
        }
    }
    
    char* atom = (char*) malloc(3 * sizeof(char));
    atoms = (char**) realloc(atoms, (size + 1) * sizeof(char*));
    counts = (int*) realloc(counts, (size + 1) * sizeof(int));
    strcpy(atom, current);
    atoms[size] = atom;
    counts[size++] = multiplier;
}

/// Parses a string representation of the molecule
/// and determines the atoms that make it up.

void Molecule::parseAtoms(char* string) {
    int level = 0;
    char first;
    char* current = (char*) malloc(3 * sizeof(char));
    current[2] = 0;
    while (first = *string++) {
        if ('A' <= first && 'Z' >= first) {
            current[0] = first;
            char second = *string;
            if ('a' <= second && 'z' >= second) {
                current[1] = second;
                string++;
            } else {
                current[1] = 0;
            }
            int count = strtol(string, &string, 10);
            if (!count) count = 1;
            addAtoms(current, multipliers[level] * count);
        } else if (first == '(') {
            char* copy = string;
            char next;
            int count = 1;
            while (next = *copy++) {
                if (next == '(') count++; else if (next == ')') count--;
                if (!count) break;
            }
            int multiplier = strtol(copy, NULL, 10);
            if (!multiplier) multiplier = 1;
            multipliers = (int*) realloc(multipliers, (level + 2) * sizeof(int));
            multipliers[++level] = multipliers[level] * multiplier;
        }
    }
    free(current);
}

void Molecule::printAtoms() {
    for (int i = 0; i < size; i++) {
        printf("%s: %d\n", atoms[i], counts[i]);
    }
}

/// Constructor for the Molecule class.

Molecule::Molecule(char* string) {
    size = 0;
    atoms = (char**) malloc(0);
    counts = (int*) malloc(0);

    coefficient = strtol(string, &string, 10);
    if (!coefficient) coefficient = 1;

    multipliers = (int*) malloc(sizeof(int));
    multipliers[0] = coefficient;
    
    parseAtoms(string);
}

#endif
