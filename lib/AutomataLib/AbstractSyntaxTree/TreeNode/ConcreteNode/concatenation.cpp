#include "concatenation.h"

Concatenation::Concatenation()
{
    isOperator_ = true;
    nodeContentName_ = '.';
}

FiniteStateAutomaton Concatenation::ToNFA(std::string alphabet, int& nodeCounter)
{
    FiniteStateAutomaton leftNFA = this->left_->ToNFA(alphabet, nodeCounter);
    FiniteStateAutomaton rightNFA = this->right_->ToNFA(alphabet, nodeCounter);

    FiniteStateAutomaton avtomat(alphabet);
    avtomat.combineAutomaton(leftNFA);
    avtomat.combineAutomaton(rightNFA);

    avtomat.addTransition(leftNFA.oneEndState(), EMPTY_SYMBOL, rightNFA.startState());

    avtomat.setStartState(leftNFA.startState());
    avtomat.setOneEndState(rightNFA.oneEndState());

    return avtomat;
}
