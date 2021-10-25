class Molecule {
    private:
        char** atoms;
        int* counts;
        int* multipliers;
        int coefficient;
        int size;
        void parseAtoms(char* string);
        void addAtoms(char* string, int multiplier);

    public:
        Molecule(char* string);
        char** getAtoms();
        int getCountOfAtom(char* atom);
        void printAtoms();
};
