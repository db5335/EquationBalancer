///
/// file: molecule.hpp
/// Header file for the Molecule class
///
/// @author Dominick Banasik

#ifndef _MOLECULE_H_
#define _MOLECULE_H_

/// The Molecule class represents a molecule with
/// individual atoms.

class Molecule {
    private:
        char** atoms;
        int* counts;
        int* multipliers;
        int coefficient;
        int size;
        bool fixed;
        
        ///

        void setCoefficient(char* string);

        /// Parses a string representation of the molecule
        /// and determines that atoms that make it up.
        ///
        /// #param string the string representing the molecule

        void parseAtoms(char* string);

        /// Adds atoms to the molecule's atom count.
        ///
        /// @param string the string representing the atom
        /// @param multiplier the number of atoms to add

        void addAtoms(char* string, int multiplier);

    public:
        ///

        bool getFixed();

        /// Constructor for the Molecule class.
        ///
        /// @param string the string to parse

        Molecule(char* string);
        
        /// Returns the array of atoms that make up the molecule.
        ///
        /// @return the array of atoms

        char** getAtoms();

        /// Returns the quantity of a particular atom present in
        /// the molecule.
        ///
        /// @param atom the atom to get the quantity of
        /// @return the quantity of that atom

        int getCountOfAtom(char* atom);

        int getSize();

        void printAtoms();
};

#endif
