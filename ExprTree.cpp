#include "ExprTree.h"
#include <sstream>

/*
 * Helper function that tests whether a string is a non-negative integer.
 */

bool isOperator(string tokens);

bool isdigit(const char & c){

  switch (c) {
  case '0' :
  case '1' :
  case '2' :
  case '3' :
  case '4' :
  case '5' :
  case '6' :
  case '7' :
  case '8' :
  case '9' : return true;
  }

  return false;
   
}

bool is_number(const std::string & s)
{
    std::string::const_iterator it = s.begin();
    while (it != s.end() && isdigit(*it)) ++it;
    return !s.empty() && it == s.end();
}

/*
 * Helper function that converts a string to an int.
 */
int to_number(const std::string & s){
  return atoi(s.c_str());
}

/*
 * Helper function that converts a number to a string.
 */
string to_string(const int & n){
  std::stringstream stream;
  stream << n;
  return stream.str();
}

/*
 * Helper function that creates a TreeNode with the appropriate operator
 * when given a string that's "+", "-", "*" or "/". If the string is wrong
 * it gives a NoOp value.
 */
TreeNode * createOperatorNode(const string & op){

  if (op == "+") return new TreeNode(Plus);
  if (op == "-") return new TreeNode(Minus);
  if (op == "*") return new TreeNode(Times);
  if (op == "/") return new TreeNode(Divide);
  return new TreeNode(NoOp);

}

/*
 * Basic constructor that sets up an empty Expr Tree.
 */
ExprTree::ExprTree(){
 
    this->root = NULL;
    this-> _size = 0;
    
}

/*
 * Constructor that takes a TreeNode and sets up an ExprTree with that node at the root.
 */
ExprTree::ExprTree(TreeNode * r){
    this->root = r;
}

/*
 * Destructor to clean up the tree.
 */
ExprTree::~ExprTree(){
   
    
}

/*
 * This function takes a string representing an arithmetic expression and breaks
 * it up into components (number, operators, parentheses).
 * It returns the broken up expression as a vector of strings.
 */
vector<string> ExprTree::tokenise(string expression){
    // splits strings by whitespaces
 vector<string> store;
     string s;
     std::stringstream in(expression);
while(in >> s) {
             store.push_back(s);
    }

  return vector<string>(store);
 }

/*
 * This function takes a vector of strings representing an expression (as produced
 * by tokenise(string), and builds an ExprTree representing the same expression.
 */
ExprTree ExprTree::buildTree(vector<string> tokens){ 

    // Shunting yard algorithm implementation with a binary tree 
    
    stack<TreeNode*> expressionStack; // expression stack (output queue)
    stack<TreeNode*> operatorStack;  // this stack will store the operator tokens
    
for (int i = 0; i < tokens.size(); i++) {  

// Since the TreeNode constructor only takes integer arguments, conversion from string to int is required.
// If Token is a number push it onto the expression stack (output queue)
if (is_number(tokens[i])) {
    TreeNode *ExprTokens = new TreeNode(to_number(tokens[i])); 
     expressionStack.push(ExprTokens); 
}
// create operator node
// store operator in o1
if (isOperator(tokens[i])) {
    TreeNode *storeOp1 = createOperatorNode(tokens[i]); 
    Operator o1 = storeOp1->getOperator();
                  
if (!operatorStack.empty()) {
    // while there is an operator token o2, at the top of the operator stack.
    Operator o2 = operatorStack.top()->getOperator();
    // and either o1 is left-associative and its precedence is less than or equal to that of o2.
if (o1 <= o2) { 
    // pop o2 off the operator stack
     TreeNode* o2 = operatorStack.top(); operatorStack.pop();  
    // pop the expressions off the expression stack and set them as children of o2 (operator)
     TreeNode* topExpr = expressionStack.top(); expressionStack.pop();
     TreeNode* bottomExpr = expressionStack.top(); expressionStack.pop();
     o2->setLeftChild(bottomExpr);
     o2->setRightChild(topExpr);
     // push o2 on to the output queue (expression stack)
     expressionStack.push(o2);
                                                            }
                                            }
                  operatorStack.push(storeOp1); // at the end of iteration push o1 onto the operator stack.
                    }
  }
    // While there are still operator tokens in the stack
    // Pop the operator onto the output queue (expression stack) 
    while (!operatorStack.empty()){
    TreeNode* Operator = operatorStack.top(); operatorStack.pop();		
    TreeNode* topExpr = expressionStack.top(); expressionStack.pop();
    TreeNode* lowerExpr = expressionStack.top(); expressionStack.pop();
    Operator->setLeftChild(lowerExpr);
    Operator->setRightChild(topExpr);
    expressionStack.push(Operator);
    }
   
// This is required  in order to build the Expr tree (expression tree) from the treenodes.
return ExprTree(expressionStack.top()); 
               
}

bool isOperator(string tokens) {
// checks whether tokens are an operator
    for (int i = 0; i < tokens.size(); i++) {
        if (tokens[i] == '+' || tokens[i] == '-' || tokens[i] == '*' || tokens[i] == '/')
            return true;
    }
return false; 
}

/*
 * This function takes a TreeNode and does the maths to calculate
 * the value of the expression it represents.
 */
int ExprTree::evaluate(TreeNode * n){
    // each case represents an operator i.e 1 = '+' , 2 = '-', 3 = '*' , 4 = '/'
    // Treenodes are recursively evaluated from left to right and returns the values stored and the result is calculated based on the operator
    switch(n->getOperator()) { 
        case 1: return (evaluate(n->getLeftChild()) + evaluate(n->getRightChild()));
        case 2: return (evaluate(n->getLeftChild()) - evaluate(n->getRightChild()));
        case 3: return (evaluate(n->getLeftChild()) * evaluate(n->getRightChild()));
        case 4: return (evaluate(n->getLeftChild()) / evaluate(n->getRightChild()));
    }
    return n->getValue();
}

/*
 * When called on an ExprTree, this function calculates the value of the
 * expression represented by the whole tree.
 */
int ExprTree::evaluateWholeTree(){
    return evaluate(this->getRoot());
}

/*
 * Given an ExprTree t, this function returns a string
 * that represents that same expression as the tree in
 * prefix notation.
 */
string ExprTree::prefixOrder(const ExprTree & t){
    string s;
    if (t.root != NULL){      
     s.append(t.root->toString());  
     s += prefixOrder(t.root->getLeftChild());
     s += prefixOrder(t.root->getRightChild());
    }
      return s;
}

/*
 * Given an ExprTree t, this function returns a string
 * that represents that same expression as the tree in
 * infix notation.
 */
string ExprTree::infixOrder(const ExprTree & t){
 string s;
    if (t.root != NULL){      
     s += infixOrder(t.root->getLeftChild()); 
     s.append(t.root->toString());  
     s += infixOrder(t.root->getRightChild());
    }
 
      return s;
 
}

/*
 * Given an ExprTree t, this function returns a string
 * that represents that same expression as the tree in
 * postfix notation.
 */
string ExprTree::postfixOrder(const ExprTree & t){
    string s;
    if (t.root != NULL){      
     s += postfixOrder(t.root->getLeftChild());   
     s += postfixOrder(t.root->getRightChild());
     s.append(t.root->toString());  
    }
 
      return s;
}

/*
 * Returns the size of the tree. (i.e. the number of nodes in it)
 */
int ExprTree::size(){ return _size; }

/*
 * Returns true if the tree contains no nodes. False otherwise.
 */
bool ExprTree::isEmpty(){ return _size == 0; }

TreeNode * ExprTree::getRoot(){ return root; }
