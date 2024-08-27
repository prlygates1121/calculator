#pragma clang diagnostic push
#pragma ide diagnostic ignored "misc-no-recursion"

/* Syntax:
 *      Expression: T {+ | - T}
 *      Term: TV {* | / TV}
 *      TermVIP: F {^ F}
 *      Factor: Identifier | Double | (E) | -F | F!
 */

#include <iostream>
#include <cstring>
#include <cmath>

class TreeNode {
public:
    [[nodiscard]] virtual double eval() const = 0;
    virtual void print() const = 0;
};

class Add : public TreeNode {
public:
    TreeNode* left;
    TreeNode* right;
    Add(TreeNode* l, TreeNode* r) : TreeNode(), left(l), right(r) {};
    [[nodiscard]] double eval() const override {
        return left->eval() + right->eval();
    }
    void print() const override {
        std::cout << "(";
        left->print();
        std::cout << "+";
        right->print();
        std::cout << ")";
    }
};

class Sub : public TreeNode {
public:
    TreeNode* left;
    TreeNode* right;
    Sub(TreeNode* l, TreeNode* r) : TreeNode(), left(l), right(r) {};
    [[nodiscard]] double eval() const override {
        return left->eval() - right->eval();
    }
    void print() const override {
        std::cout << "(";
        left->print();
        std::cout << "-";
        right->print();
        std::cout << ")";
    }
};

class Mul : public TreeNode {
public:
    TreeNode* left;
    TreeNode* right;
    Mul(TreeNode* l, TreeNode* r) : TreeNode(), left(l), right(r) {};
    [[nodiscard]] double eval() const override {
        return left->eval() * right->eval();
    }
    void print() const override {
        std::cout << "(";
        left->print();
        std::cout << "*";
        right->print();
        std::cout << ")";
    }
};

class Div : public TreeNode {
public:
    TreeNode* left;
    TreeNode* right;
    Div(TreeNode* l, TreeNode* r) : TreeNode(), left(l), right(r) {};
    [[nodiscard]] double eval() const override {
        return left->eval() / right->eval();
    }
    void print() const override {
        std::cout << "(";
        left->print();
        std::cout << "/";
        right->print();
        std::cout << ")";
    }
};

class Caret : public TreeNode {
public:
    TreeNode* left;
    TreeNode* right;
    Caret(TreeNode* l, TreeNode* r) : TreeNode(), left(l), right(r) {};
    [[nodiscard]] double eval() const override {
        return pow(left->eval(), right->eval());
    }
    void print() const override {
        std::cout << "(";
        left->print();
        std::cout << "^";
        right->print();
        std::cout << ")";
    }
};

class Negate : public TreeNode {
public:
    TreeNode* arg;
    explicit Negate(TreeNode* a) : TreeNode(), arg(a) {};
    [[nodiscard]] double eval() const override {
        return -arg->eval();
    }
    void print() const override {
        std::cout << "(-";
        arg->print();
        std::cout << ")";
    }
};

class Factorial : public TreeNode {
public:
    TreeNode* arg;
    explicit Factorial(TreeNode* a) : TreeNode(), arg(a) {};
    [[nodiscard]] double eval() const override {
        return fact((int)arg->eval());
    }
    void print() const override {
        std::cout << "(";
        arg->print();
        std::cout << "!)";
    }
    [[nodiscard]] double fact(int in, int acc = 1) const {
        if (in == 0) {
            return acc;
        }
        if (in < 0) {
            return 0./0;
        }
        return fact(in - 1, acc * in);
    }
};

class Double : public TreeNode {
public:
    double val;
    explicit Double(double v) : TreeNode(), val(v) {};
    [[nodiscard]] double eval() const override {
        return val;
    }
    void print() const override {
        std::cout << val;
    }
};

class Identifier : public TreeNode {
public:
    const char* str;
    int val;
    explicit Identifier(const char* s, int v) : TreeNode(), str(s), val(v) {};
    [[nodiscard]] double eval() const override {
        return val;
    }
    void print() const override {
        std::cout << str;
    }
};

#define MAX_SIZE 30

char nextToken;
TreeNode* resultTree;
char* pInput, *startInput;
char nextIdentifier[MAX_SIZE];
char nextDouble[MAX_SIZE];

TreeNode* parseExp();
TreeNode* parseTerm();
TreeNode* parseTermVIP();
TreeNode* parseFactor();

bool isDigit(char in) {
    return in >= '0' && in <= '9';
}

bool isLetter(char in) {
    return in >= 'a' && in <= 'z' || in >= 'A' && in <= 'Z';
}

void scanToken() {
    nextToken = *pInput;
    // if next character is a digit
    if (isDigit(nextToken)) {
        int i = 0;
        bool hasDigit = false;
        while (isDigit(*pInput) || *pInput == '.') { // stop on encountering a non-digit
            if (i == MAX_SIZE - 1) {
                std::cout << "The number is too long.\n";
                exit(-1);
            }
            if (*pInput == '.') {
                if (hasDigit) {
                    std::cout << "A number is not formatted.";
                    exit(-1);
                } else {
                    hasDigit = true;
                }
            }
            nextDouble[i++] = *pInput;
            pInput++;
        }
        nextDouble[i] = '\0';
        return;
    }
    // if next character is +, -, *, /, (, ) or !
    if (nextToken == '+' || nextToken == '-' || nextToken == '*' || nextToken == '/' ||
        nextToken == '(' || nextToken == ')' || nextToken == '!' || nextToken == '^') {
        pInput++;
        return;
    }
    // otherwise, the next character is part of an Identifier (a string starting with a letter, consisting of letters and digits)
    int i = 0;
    do {
        if (i == MAX_SIZE - 1) {
            std::cout << "The identifier is too long.\n";
            exit(-1);
        }
        nextIdentifier[i++] = *pInput;
        pInput++;
    } while (isDigit(*pInput) || isLetter(*pInput)); // stop on encountering a non-digit and non-letter
    nextIdentifier[i] = '\0';
}



TreeNode* parseExp() {
    TreeNode* a = parseTerm();
    if (a == nullptr) {
        return nullptr; // report error if parseTerm() fails
    }
    while (true) {
        if (nextToken == '+') {
            scanToken();
            TreeNode* b = parseTerm();
            if (b == nullptr) {
                return nullptr; // report error if parseTerm() fails
            }
            a = new Add(a, b);
        } else if (nextToken == '-') {
            scanToken();
            TreeNode* b = parseTerm();
            if (b == nullptr) {
                return nullptr; // report error if parseTerm() fails
            }
            a = new Sub(a, b);
        } else {
            return a;
        }
    }
}

TreeNode* parseTerm() {
    TreeNode* a = parseTermVIP(); // scan a factor
    if (a == nullptr) {
        return nullptr; // report error if parseFactor() fails
    }
    while (true) {
        if (nextToken == '*') { // if nextToken is a '*' -> term: F * T
            scanToken();
            TreeNode* b = parseTermVIP();
            if (b == nullptr) {
                return nullptr; // report error if parseTerm() fails
            }
            a = new Mul(a, b);
        } else if (nextToken == '/') { // if nextToken is a '/' -> term: F / T
            scanToken();
            TreeNode* b = parseTermVIP();
            if (b == nullptr) {
                return nullptr; // report error if parseTerm() fails
            }
            a = new Div(a, b);
        } else { // otherwise -> term: F
            return a;
        }
    }

}

TreeNode* parseTermVIP() {
    TreeNode* a = parseFactor();
    if (a == nullptr) {
        return nullptr;
    }
    while (true) {
        if (nextToken == '^') {
            scanToken();
            TreeNode* b = parseFactor();
            if (b == nullptr) {
                return nullptr;
            }
            a = new Caret(a, b);
        } else {
            return a;
        }
    }
}

TreeNode* parseFactor() {
    // if nextToken is an Identifier -> factor: Identifier
    if (isLetter(nextToken)) {
        scanToken();
        return new Identifier(nextIdentifier, 0);
    }
    // if nextToken is an Double -> factor: Double
    if (isDigit(nextToken)) {
        scanToken();
        TreeNode* a = new Double(atof(nextDouble));
        while (true) {
            if (nextToken == '!') {
                scanToken();
                a = new Factorial(a);
            } else {
                return a;
            }
        }
    }
    // if nextToken is a left parenthesis -> factor: (E)
    if (nextToken == '(') {
        scanToken();
        TreeNode* a = parseExp();
        if (a == nullptr) {
            return nullptr; // report error if no expression found
        }
        if (nextToken == ')') {
            scanToken();
            while (true) {
                if (nextToken == '!') {
                    scanToken();
                    a = new Factorial(a);
                } else {
                    return a;
                }
            }
        }
        return nullptr; // report error if no right parenthesis found
    }
    // if nextToken is a minus sign -> factor: -F
    if (nextToken == '-') {
        scanToken();
        return new Negate(parseFactor());
    }
    // report error if nextToken is anything else (+ | * | / etc.)
    return nullptr;
}

int main(int argc, char** argv) {
    if (argc == 1) {
        std::cout << "Please pInput an expression.\n";
        return -1;
    }
    if (argc == 2) {
        pInput = argv[1];
    } else {
        std::string a;
        for (int i = 1; i < argc; i++) {
            a.append(argv[i]);
        }
        startInput = new char[a.size() + 1];
        std::strcpy(startInput, a.c_str());
        pInput = startInput;
    }

    scanToken();
    resultTree = parseExp();
    if (resultTree == nullptr || nextToken != '\0') {
        std::cout << "Invalid pInput.\n";
        return -1;
    }

    resultTree->print();
    std::cout << " = ";
    std::cout << resultTree->eval() << "\n";

    delete[] startInput;
}

#pragma clang diagnostic pop