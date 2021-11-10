///
/// file: equation.hpp
/// Header file for the Equation class
///
/// @author Dominick Banasik

#include <stdlib.h>

#ifndef _EQUATION_H_
#define _EQUATION_H_

#define CAPACITY 5
#define ATOM_SIZE 3

#include "molecule.hpp"
#include "matrix.hpp"

/// The Equation class represents a chemical equation
/// with a list of reactants and a list of products.

class Equation {
    private:
        Molecule* reactants;
        Molecule* products;
        char** atoms;
        int reactantCount;
        int productCount;
        int freeReactantCount;
        int freeProductCount;
        int atomCount;
        int reactantCapacity;
        int productCapacity;
        int atomCapacity;

        /// Parses a string that represents the molecules that
        /// make up the equation.
        ///
        /// @param string the string to parse

        void parse(char* string);

        /// Adds a molecule to the list of reactants or products.
        ///
        /// @param string the string representation of the equation
        /// @param start the starting index of the molecule
        /// @param index the ending index of the molecule
        /// @param isReactant whether the molecule is a reactant or products

        void addMolecule(char* string, int start, int index, bool isReactant);
        
        /// Updates the list of atoms to include all atoms from a group
        /// of molecules.
        ///
        /// @param isReactant whether to update from reactants or products

        void generateAtoms(bool isReactant);

        /// Fills a column of a matrix with the coefficients corresponding
        /// to the number of a certain atom in that molecule.
        ///
        /// @param matrix the matrix to fill
        /// @param isReactant whether a reactant or product column is being filled

        void fillMatrix(Matrix matrix, bool isReactant);

        /// Fills the last column of a matrix with the fixed number of a certain
        /// atom in the entire equation.
        ///
        /// @param matrix the matrix to fill

        void fillLastColumn(Matrix matrix);

    public:
        /// Constructor for the Equation class.
        ///
        /// @param string the string to create the equation from

        Equation(char* string);
        
        /// Returns a list of all atoms in the equation.
        ///
        /// @return list of atoms

        char** getAtoms();

        /// Generates a matrix from the chemical equation.
        ///
        /// @return augmented matrix representing the equation

        Matrix createMatrixFromEquation();

        /// Prints the solution to the equation, or states otherwise
        /// if no solution exists, the equation is balanced, or
        /// the equation is unbalanced.

        void printSolution(Solution solution);
};

#endif
