//
//  eval.cpp
//  2. stacks&queues
//
//  Created by Chang Liu on 2/1/15.
//  Copyright (c) 2015 Chang Liu. All rights reserved.
//

#include <iostream>
#include <string>
#include <stack>
#include <cassert>
using namespace std;

bool isValid(const string infix);

bool lessEqualTo(const char c1, const char c2);

void toPostfix(const string infix, string &postfix);

int evaluate(string infix, const bool values[], string& postfix, bool& result) {
    
    if (! isValid(infix)) {
        return 1;
    }
    postfix = "";
    toPostfix(infix, postfix);
    
    /*
     Initialize the operand stack to empty
     For each character ch in the postfix string
	    if ch is an operand
     push the value that ch represents onto the operand stack
	    else // ch is a binary operator
     set operand2 to the top of the operand stack
     pop the stack
     set operand1 to the top of the operand stack
     pop the stack
     apply the operation that ch represents to operand1 and
     operand2, and push the result onto the stack
     When the loop is finished, the operand stack will contain one item,
     the result of evaluating the expression
     */
    stack<bool> opStack;
    for (char ch = 0; ch < postfix.length(); ch++) {
        if (isdigit(postfix[ch])) {
            int i = postfix[ch] - '0';
            opStack.push(values[i]);
        }
        else if (postfix[ch] == '!') {
            bool op = opStack.top();
            if (op) {
                op = false;
            }
            else
                op = true;
            opStack.pop();
            opStack.push(op);
        }
        else {
            bool op1 = opStack.top();
            opStack.pop();
            bool op2 = opStack.top();
            opStack.pop();
            switch (postfix[ch]) {
                case '&':
                    if (op1 && op2) {
                        opStack.push(true);
                    }
                    else {
                        opStack.push(false);
                    }
                    break;
                case '|':
                    if (op1 || op2) {
                        opStack.push(true);
                    }
                    else {
                        opStack.push(false);
                    }
                    break;
            }
            
            
            
        }
        
    }
    result = opStack.top();
    opStack.pop();
    
    return 0;
}

void toPostfix(const string infix, string &postfix) {
    stack<char> opStack;
    bool neg = false;
    bool negP = false;
    for (char ch = 0; ch < infix.length(); ch++) {
        switch (infix[ch]) {
            case '(':
                opStack.push(infix[ch]);
                if (neg) {
                    negP = true;
                }
                break;
                
            case ')':
                while (opStack.top() != '(') {
                    postfix += opStack.top();
                    opStack.pop();
                }
                opStack.pop();
                if (negP) {
                    postfix += opStack.top();
                    negP = false;
                    neg = false;
                    opStack.pop();
                }
                break;
            case '&':
            case '|':
                while (!opStack.empty() && opStack.top() != '(' && lessEqualTo(infix[ch], opStack.top())) {
                    postfix += opStack.top();
                    opStack.pop();
                }
                opStack.push(infix[ch]);
                break;
            case '!':
                opStack.push(infix[ch]);
                neg = true;
                break;
                
            default:
                // skips spaces
                if (isdigit(infix[ch])) {
                    postfix += infix[ch];
                    if (neg && !negP) {
                        postfix += opStack.top();
                        opStack.pop();
                        neg = false;
                    }
                }
                break;
        }
    }
    while (!opStack.empty()) {
        postfix += opStack.top();
        opStack.pop();
    }
    
    
}


bool isValid(const string infix) {
    char prev = '~'; // for testing "uninitialized" char
    int openCount = 0;
    int closeCount = 0;
    for (int i = 0; i < infix.length(); i++) {
        switch (infix[i]) {
            case ' ':
                break;
            case '0': case '1': case '2': case '3': case '4': case '5': case '6': case '7': case '8': case '9':
                if (isdigit(prev)) {
                    return false;
                }
                prev = infix[i];
                break;
            case '|': case '&':
                if (prev == '~') {
                    return false;
                }
                if (prev == '|' || prev == '&' || prev == '!' || prev == '(') {
                    return false;
                }
                prev = infix[i];
                break;
            case '!':
                if (isdigit(prev) || prev == ')') {
                    return false;
                }
                prev = infix[i];
                break;
            case ')':
                closeCount++;
                if (prev == '(') {
                    return false;
                }
                prev = infix[i];
                break;
            case '(':
                if (isdigit(prev)) {
                    return false;
                }
                openCount++;
                prev = infix[i];
                break;
            default:
                return false;
                
                
        }
    }
    // count parentheses
    if (openCount != closeCount) {
        return false;
    }
    // empty string and ending with operator
    switch(prev) {
        case '~':
        case '!':
        case '(':
        case '&':
        case '|':
            return false;
    }
    return true;
}

bool lessEqualTo(const char c1, const char c2) {
    if (c1 == '|') {
        return true;
    }
    else if (c1 == '&' && (c2 == '&' || c2 == '!')) {
        return true;
    }
    else if (c1 == '!' && c2 == '!') {
        return true;
    }
    else
        return false;
    
}

int main()
{
    bool ba[10] = {
        //  0      1      2      3      4      5      6      7      8      9
        true,  true,  true,  false, false, false, true,  false, true,  false
    };
    string pf;
    bool answer;
    assert(evaluate("2| 3", ba, pf, answer) == 0  &&  pf == "23|" &&  answer);
    assert(evaluate("8|", ba, pf, answer) == 1);
    assert(evaluate("4 5", ba, pf, answer) == 1);
    assert(evaluate("01", ba, pf, answer) == 1);
    assert(evaluate("()", ba, pf, answer) == 1);
    assert(evaluate("2(9|8)", ba, pf, answer) == 1);
    assert(evaluate("2(&8)", ba, pf, answer) == 1);
    assert(evaluate("(6&(7|7)", ba, pf, answer) == 1);
    assert(evaluate("4  |  !3 & (0&3) ", ba, pf, answer) == 0
           &&  pf == "43!03&&|"  &&  !answer);
    assert(evaluate("", ba, pf, answer) == 1);
    assert(evaluate(" 9  ", ba, pf, answer) == 0  &&  pf == "9"  &&  !answer);
    ba[2] = false;
    ba[9] = true;
    assert(evaluate("((9))", ba, pf, answer) == 0  &&  pf == "9"  &&  answer);
    assert(evaluate("2| 3", ba, pf, answer) == 0  &&  pf == "23|" &&  !answer);
    assert(evaluate("!!!2", ba, pf, answer) == 0 && answer);
    cout << "Passed all tests" << endl;
}