#include <iostream>
#include <regex>

class TreeNode {
public:
    [[nodiscard]] virtual int eval() const = 0;
    virtual void print() const = 0;
};

class Add : public TreeNode {
public:
    TreeNode* left;
    TreeNode* right;
    Add(TreeNode* l, TreeNode* r) : TreeNode(), left(l), right(r) {};
    [[nodiscard]] int eval() const override {
        return left->eval() + right->eval();
    }
    void print() const override {
        std::cout << "(" << left->eval() << "+" << right->eval() << ")";
    }
};

class Sub : public TreeNode {
public:
    TreeNode* left;
    TreeNode* right;
    Sub(TreeNode* l, TreeNode* r) : TreeNode(), left(l), right(r) {};
    [[nodiscard]] int eval() const override {
        return left->eval() - right->eval();
    }
    void print() const override {
        std::cout << "(" << left->eval() << "-" << right->eval() << ")";
    }
};

class Mul : public TreeNode {
public:
    TreeNode* left;
    TreeNode* right;
    Mul(TreeNode* l, TreeNode* r) : TreeNode(), left(l), right(r) {};
    [[nodiscard]] int eval() const override {
        return left->eval() * right->eval();
    }
    void print() const override {
        std::cout << "(" << left->eval() << "*" << right->eval() << ")";
    }
};

class Div : public TreeNode {
public:
    TreeNode* left;
    TreeNode* right;
    Div(TreeNode* l, TreeNode* r) : TreeNode(), left(l), right(r) {};
    [[nodiscard]] int eval() const override {
        return left->eval() / right->eval();
    }
    void print() const override {
        std::cout << "(" << left->eval() << "/" << right->eval() << ")";
    }
};

class Negate : public TreeNode {
public:
    TreeNode* arg;
    explicit Negate(TreeNode* a) : TreeNode(), arg(a) {};
    [[nodiscard]] int eval() const override {
        return -arg->eval();
    }
    void print() const override {
        std::cout << "(-" << arg->eval() << ")";
    }
};

class Integer : public TreeNode {
public:
    int val;
    explicit Integer(int v) : TreeNode(), val(v) {};
    [[nodiscard]] int eval() const override {
        return val;
    }
    void print() const override {
        std::cout << val;
    }
};

class LeftParenthesis : public TreeNode {
public:

};

class Identifier : public TreeNode {
public:
    const char* str;
    int val;
    explicit Identifier(const char* s, int v) : TreeNode(), str(s), val(v) {};
    [[nodiscard]] int eval() const override {
        return val;
    }
    void print() const override {
        std::cout << str;
    }
};

char nextToken;
TreeNode* resultTree;
char* input;

void scanToken() {
    nextToken = *input;
    // if next character is a digit
    if (nextToken <= '9' && nextToken >= '0') {
        while (*input <= '9' && *input >= '0') { // stop on encountering a non-digit
            input++;
        }
        return;
    }
    // if next character is +, -, *, /, ( or )
    if (nextToken == '+' || nextToken == '-' || nextToken == '*' ||
        nextToken == '/' || nextToken == '(' || nextToken == ')') {
        input++;
        return;
    }
    // otherwise, the next character is part of an identifier (a string starting with a letter, consisting of letters and digits)
    do {
        input++;
    } while ((*input >= '0' && *input <= '9') || // stop on encountering a non-digit and non-letter
             (*input >= 'a' && *input <= 'z' || *input >= 'A' && *input <= 'Z'));
    return;
}

int main(int argc, char** argv) {
    if (argc != 2) {
        std::cout << "Please input exactly one expression.\n";
        return -1;
    }
    input = argv[1];
    do {
        scanToken();
        std::cout << nextToken << " ";
    } while (nextToken != '\0');
}
