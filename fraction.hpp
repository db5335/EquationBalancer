class Fraction {
    private:
        int numerator;
        int denominator;
    
    public:
        Fraction(int num, int den);
        Fraction(const Fraction &copy);
        int getNum();
        int getDen();
        void multiply(Fraction other);
        void add(Fraction other);
        void simplify();
        bool equals(double d);
};
