#include "union.h"

Union::Union()
{
    isOperator_ = true;
    nodeContentName_ = '|';
}

FiniteStateAutomaton Union::ToNFA(std::string alphabet, int &nodeCounter)
{
    FiniteStateAutomaton leftNFA = this->left_->ToNFA(alphabet, nodeCounter);
    FiniteStateAutomaton rightNFA = this->right_->ToNFA(alphabet, nodeCounter);

    FiniteStateAutomaton avtomat(alphabet);
    avtomat.combineAutomaton(leftNFA);
    avtomat.combineAutomaton(rightNFA);

    State q("q" + std::to_string(nodeCounter++));
    State f("f" + std::to_string(nodeCounter++));

    avtomat.addState(q);
    avtomat.addTransition(q, EMPTY_SYMBOL, leftNFA.startState());
    avtomat.addTransition(q, EMPTY_SYMBOL, rightNFA.startState());

    avtomat.addState(f);
    avtomat.addTransition(leftNFA.oneEndState(), EMPTY_SYMBOL, f);
    avtomat.addTransition(rightNFA.oneEndState(), EMPTY_SYMBOL, f);

    avtomat.setStartState(q);
    avtomat.setOneEndState(f);

    return avtomat;
}
