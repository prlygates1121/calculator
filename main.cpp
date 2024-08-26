#pragma clang diagnostic push
#pragma ide diagnostic ignored "misc-no-recursion"

/* Syntax:
 *      Expression: T {+ / - T}
 *      Term: F {* / / F}
 *      Factor: Identifier | Double | (E) | -F
 */

#include <iostream>

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

class Double : public TreeNode {
public:
    double val;
    explicit Double(int v) : TreeNode(), val(v) {};
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
char* input;
char nextIdentifier[MAX_SIZE];
char nextDouble[MAX_SIZE];

TreeNode* parseExp();
TreeNode* parseTerm();
TreeNode* parseFactor();

bool isDigit(char in) {
    return in >= '0' && in <= '9';
}

bool isLetter(char in) {
    return in >= 'a' && in <= 'z' || in >= 'A' && in <= 'Z';
}

void scanToken() {
    nextToken = *input;
    // if next character is a digit
    if (isDigit(nextToken)) {
        int i = 0;
        while (isDigit(*input) || *input == '.') { // stop on encountering a non-digit
            if (i == MAX_SIZE - 1) {
                std::cout << "The number is too long.\n";
                exit(-1);
            }
            nextDouble[i++] = *input;
            input++;
        }
        nextDouble[i] = '\0';
        return;
    }
    // if next character is +, -, *, /, ( or )
    if (nextToken == '+' || nextToken == '-' || nextToken == '*' ||
        nextToken == '/' || nextToken == '(' || nextToken == ')') {
        input++;
        return;
    }
    // otherwise, the next character is part of an Identifier (a string starting with a letter, consisting of letters and digits)
    int i = 0;
    do {
        if (i == MAX_SIZE - 1) {
            std::cout << "The identifier is too long.\n";
            exit(-1);
        }
        nextIdentifier[i++] = *input;
        input++;
    } while (isDigit(*input) || isLetter(*input)); // stop on encountering a non-digit and non-letter
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
    TreeNode* a = parseFactor(); // scan a factor
    if (a == nullptr) {
        return nullptr; // report error if parseFactor() fails
    }
    while (true) {
        if (nextToken == '*') { // if nextToken is a '*' -> term: F * T
            scanToken();
            TreeNode* b = parseFactor();
            if (b == nullptr) {
                return nullptr; // report error if parseTerm() fails
            }
            a = new Mul(a, b);
        } else if (nextToken == '/') { // if nextToken is a '/' -> term: F / T
            scanToken();
            TreeNode* b = parseFactor();
            if (b == nullptr) {
                return nullptr; // report error if parseTerm() fails
            }
            a = new Div(a, b);
        } else { // otherwise -> term: F
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
        return new Double(atof(nextDouble));
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
            return a;
        } else {
            return nullptr; // report error if no right parenthesis found
        }
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
    if (argc != 2) {
        std::cout << "Please input exactly one expression.\n";
        return -1;
    }
    input = argv[1];

    scanToken();
    resultTree = parseExp();
    if (resultTree == nullptr || nextToken != '\0') {
        std::cout << "Invalid input.\n";
        return -1;
    }

    resultTree->print();
    std::cout << " = ";
    std::cout << resultTree->eval();
}

#pragma clang diagnostic pop