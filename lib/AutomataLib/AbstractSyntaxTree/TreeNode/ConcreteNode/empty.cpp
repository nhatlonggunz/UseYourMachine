#include "empty.h"

Empty::Empty()
{
    isOperator_ = false;
    nodeContentName_ = '_';
}

FiniteStateAutomaton Empty::ToNFA(std::string alphabet, int &nodeCounter)
{
    FiniteStateAutomaton avtomat(alphabet);

    State q("q" + std::to_string(nodeCounter++));
    State f("q" + std::to_string(nodeCounter++));

    avtomat.addState(q);
    avtomat.addState(f);

    avtomat.addTransition(q, EMPTY_SYMBOL, f);

    avtomat.setStartState(q);
    avtomat.setOneEndState(f);

    return avtomat;
}
