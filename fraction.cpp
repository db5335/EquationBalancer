
#include <stdio.h>

#include "fraction.hpp"

int gcd(int m, int n) {
    int i = 1;
    int g = 1;
    while (i <= m && i <= n) {
        if (m % i == 0 && n % i == 0) g = i;
        i++;
    }
    return g;
}

int lcm(int m, int n) {
    int l = m > n ? m : n;
    while (true) {
        if (l % m == 0 && l % n == 0) {
            return l;
        }
        l++;
    }
}

int Fraction::getNum() {
    return numerator;
}

int Fraction::getDen() {
    return denominator;
}

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

void Fraction::multiply(Fraction other) {
    numerator = numerator * other.numerator;
    denominator = denominator * other.denominator;
    simplify();
}

void Fraction::add(Fraction other) {
    int den1 = denominator > 0 ? denominator : -1 * denominator;
    int den2 = other.denominator > 0 ? other.denominator : -1 * other.denominator;
    int lcd = lcm(den1, den2);
    
    numerator = numerator * (lcd / denominator) + other.numerator * (lcd / other.denominator);
    denominator = lcd;
    simplify();
}

bool Fraction::equals(double d) {
    return d == (double) numerator / (double) denominator;
}

Fraction::Fraction(const Fraction &copy) {
    numerator = copy.numerator;
    denominator = copy.denominator;
}

Fraction::Fraction(int num, int den) {
    numerator = num;
    denominator = den;
    simplify();
}

