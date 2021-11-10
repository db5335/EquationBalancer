///
/// file: fraction.hpp
/// Header file for the Fraction class
///
/// @author Dominick Banasik

#ifndef _FRACTION_H_
#define _FRACTION_H_

class Fraction {
    private:
        int numerator;
        int denominator;
         
         /// Simplifies the fraction.

        void simplify();
    
    public:
        /// Constructor for the Fraction class.
        ///
        /// @param num numerator of the fraction
        /// @param den denominator of the fraction

        Fraction(int num, int den);
        
        /// Constructor for the Fraction class.
        ///
        /// @param val value of the fraction

        Fraction(int val);

        /// Copy constructor for the Fraction class.
        ///
        /// @param copy the fraction to copy

        Fraction(const Fraction &copy);
        
        /// Returns the reciprocal of the fraction.
        ///
        /// @return the reciprocal

        Fraction getReciprocal();

        /// Returns the numerator of the fraction.
        ///
        /// @return the numerator

        int getNum();

        /// Returns the denominator of the fraction.
        ///
        /// @return the denominator

        int getDen();

        /// Multiplies the fraction by another.
        ///
        /// @param other the fraction to multiply by

        void multiply(Fraction other);
        
        /// Multiplies the fraction by a scalar.
        ///
        /// @param scalar the scalar to multiply by

        void multiply(int scalar);

        /// Adds another fraction.
        ///
        /// @param other the fraction to add

        void add(Fraction other);
       
        /// Checks whether the fraction equals a decimal value.
        ///
        /// @param d the value to compare to
        /// @return whether or not the fraction is equal

        bool equals(double d);
};

#endif
