#include "concatenation.h"

Concatenation::Concatenation()
{
    isOperator_ = true;
    nodeContentName_ = '.';
}

Automaton Concatenation::ToNFA(std::string alphabet, int& nodeCounter)
{
    Automaton leftNFA = this->left_->ToNFA(alphabet, nodeCounter);
    Automaton rightNFA = this->right_->ToNFA(alphabet, nodeCounter);

    Automaton avtomat(alphabet);
    avtomat.combineAutomaton(leftNFA);
    avtomat.combineAutomaton(rightNFA);

    avtomat.addTransition(leftNFA.oneEndState(), EMPTY_SYMBOL, rightNFA.startState());

    avtomat.setStartState(leftNFA.startState());
    avtomat.setOneEndState(rightNFA.oneEndState());

    return avtomat;
}
