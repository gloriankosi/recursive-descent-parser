/**
 * @file bnf-recursive-desc.cc
 * @author Glorian Kosi
 * @brief 
 * Builds and evaluates expression tree based on grammar using recursive descent.
 * 
 * To compile:
 * g++ -g -std=c++2a expr-tree.cc -o exp
 * 
 * To run:
 * ./exp filename.txt
 * 
 * NOTE: You need a file when running, this file should contain an expression following the
 * grammar below. There is no error checking so the grammar must be correct. For example, something
 * like: {5*6}/2+5
 * 
 * ----Grammar used----
 * <expression>  ::=  <factor>  * <expression>   |   <factor>  /  <expression>   |   <factor>
 * <factor>  :==  <term> + <factor>  |  <term> - <factor>  |  <term>
 * <term>  ::=  {  <expression>  }  |  <literal>
 * <literal>  ::=  0|1|2|3|4|5|6|7|8|9
 * 
 */

#include <iostream>

using std::cerr;
using std::cin;
using std::cout;
using std::endl;
using std::exception;
using std::string;

typedef int symbol;

struct Node
{
    symbol data;
    Node *left = nullptr;
    Node *right = nullptr;
};

void Expression(Node *&, FILE *);
void Factor(Node *&, FILE *);
void Term(Node *&, FILE *);
void Literal(Node *&, FILE *);
void next(FILE *, symbol &);
void printSubtree(Node *, const std::string &);
void printTree(Node *);
int evaluate(Node *);

int c; //global, keeps track of current character

int main(int argc, char *argv[])
{
    Node *root = new Node();
    FILE *file;
    try
    {
        file = fopen(argv[1], "r");
    }
    catch (const exception &e)
    {
        cerr << e.what() << '\n';
    }

    next(file, c);

    Expression(root, file);
    printTree(root);
    cout << evaluate(root) << "\n";
    return 0;
};

void Literal(Node *&T, FILE *file)
{
    T->data = c;
    T->left = nullptr;
    T->right = nullptr;
    next(file, c);
};

void Term(Node *&T, FILE *file)
{
    if (c == '{')
    {
        next(file, c);
        Expression(T, file);
        next(file, c);
    }
    else
    {
        Literal(T, file);
    }
};

void Factor(Node *&T, FILE *file)
{
    Node *termTree = new Node();
    Term(termTree, file);

    if (c == '+')
    {
        Node *factorTree = new Node();
        next(file, c);
        Factor(factorTree, file);
        T->data = '+';
        T->left = termTree;
        T->right = factorTree;
    }
    else if (c == '-')
    {
        Node *factorTree = new Node();
        next(file, c);
        Factor(factorTree, file);
        T->data = '-';
        T->left = termTree;
        T->right = factorTree;
    }
    else // <term> case
    {
        T = termTree;
    }
};

void Expression(Node *&T, FILE *file)
{
    Node *factorTree = new Node();
    Factor(factorTree, file);

    if (c == '*')
    {
        Node *expressionTree = new Node();
        next(file, c);
        Expression(expressionTree, file);
        T->data = '*';
        T->left = factorTree;
        T->right = expressionTree;
    }
    else if (c == '/')
    {
        Node *expressionTree = new Node();
        next(file, c);
        Expression(expressionTree, file);
        T->data = '/';
        T->left = factorTree;
        T->right = expressionTree;
    }
    else // <factor> case
    {
        T = factorTree;
    }
};

void next(FILE *file, symbol &c)
{
    c = fgetc(file);
}

int evaluate(Node *node)
{
    if (!node)
    {
        return 0;
    }

    if (!node->left && !node->right) // Leaf node
    {

        return node->data - '0';
    }

    char leftValue = evaluate(node->left);
    char rightValue = evaluate(node->right);

    if ((char)node->data == '+')
    {
        return leftValue + rightValue;
    }
    if ((char)node->data == '-')
    {
        return leftValue - rightValue;
    }
    if ((char)node->data == '*')
    {
        return leftValue * rightValue;
    }
    if ((char)node->data == '/')
    {
        return leftValue / rightValue;
    }
    return 0;
}

// https://linux.die.net/man/1/tree
// https://stackoverflow.com/a/50650932
void printSubtree(Node *root, const string &prefix)
{
    if (root == NULL)
    {
        return;
    }

    bool hasLeft = (root->left != NULL);
    bool hasRight = (root->right != NULL);

    if (!hasLeft && !hasRight)
    {
        return;
    }

    cout << prefix;
    cout << ((hasLeft && hasRight) ? "├── " : "");
    cout << ((!hasLeft && hasRight) ? "└── " : "");

    if (hasRight)
    {
        bool printStrand = (hasLeft && hasRight && (root->right->right != NULL || root->right->left != NULL));
        string newPrefix = prefix + (printStrand ? "│   " : "    ");
        cout << (char)root->right->data << endl;
        printSubtree(root->right, newPrefix);
    }

    if (hasLeft)
    {
        cout << (hasRight ? prefix : "") << "└── " << (char)root->left->data << endl;
        printSubtree(root->left, prefix + "    ");
    }
}

// https://linux.die.net/man/1/tree
// https://stackoverflow.com/a/50650932
void printTree(Node *root)
{
    if (root == NULL)
    {
        return;
    }

    cout << (char)root->data << endl;
    printSubtree(root, "");
    cout << endl;
}