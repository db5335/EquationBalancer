
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
    int g = gcd(numerator, denominator);
    if (g > 1) {
        numerator /= g;
        denominator /= g;
    }
}

void Fraction::multiply(Fraction other) {
    numerator = numerator * other.numerator;
    denominator = denominator * other.denominator;
    simplify();
}

void Fraction::add(Fraction other) {
    int lcd = lcm(denominator, other.denominator);
    numerator = numerator * (lcd / denominator) + other.numerator * (lcd / other.denominator);
    denominator = lcd;
    simplify();
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

