///
/// file: fraction.cpp
/// Implementation for the Fraction class
///
/// @author Dominick Banasik

#include <stdio.h>

#include "fraction.hpp"

#ifndef _FRACTION_IMPL_
#define _FRACTION_IMPL_

/// Computes the great common divisor of two numbers.
///
/// @param m the first number
/// @param n the second number
/// @return the greatest common divisor

int gcd(int m, int n) {
    int i = 1;
    int g = 1;
    
    while (i <= m && i <= n) {
        if (m % i == 0 && n % i == 0) g = i;
        i++;
    }

    return g;
}

/// Computes the least common multiple of two numbers.
///
/// @param m the first number
/// @param n the second number
/// @return the least common multiple

int lcm(int m, int n) {
    int l = m > n ? m : n;
    while (true) {
        if (l % m == 0 && l % n == 0) {
            return l;
        }
        l++;
    }
}

/// Returns the numerator of the fraction.

int Fraction::getNum() {
    return numerator;
}

/// Returns the denominator of the fraction.

int Fraction::getDen() {
    return denominator;
}

/// Simplifies the fraction.

void Fraction::simplify() {
    if (numerator == 0) {
        denominator = 1;
        return;
    }
    
    int num = numerator > 0 ? numerator : -1 * numerator;
    int den = denominator > 0 ? denominator : -1 * denominator;

    int g = gcd(num, den);
    if (g > 1) {
        numerator /= g;
        denominator /= g;
    }

    if (denominator < 0) {
        numerator *= -1;
        denominator *= -1;
    }
}

/// Multiplies the fraction by another.

void Fraction::multiply(Fraction other) {
    numerator = numerator * other.numerator;
    denominator = denominator * other.denominator;
    simplify();
}

/// Adds another fraction.

void Fraction::add(Fraction other) {
    int den1 = denominator > 0 ? denominator : -1 * denominator;
    int den2 = other.denominator > 0 ? other.denominator : -1 * other.denominator;
    int lcd = lcm(den1, den2);
    
    numerator = numerator * (lcd / denominator) + other.numerator * (lcd / other.denominator);
    denominator = lcd;
    simplify();
}

/// Checks whether the fraction equals a decimal value.

bool Fraction::equals(double d) {
    return d == (double) numerator / (double) denominator;
}

/// Copy constructor for the Fraction class.

Fraction::Fraction(const Fraction &copy) {
    numerator = copy.numerator;
    denominator = copy.denominator;
}

/// Constructor for the Fraction class.

Fraction::Fraction(int num, int den) {
    numerator = num;
    denominator = den;
    simplify();
}

#endif
