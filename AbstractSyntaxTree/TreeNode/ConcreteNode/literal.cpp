#include "literal.h"

Literal::Literal()
{
    isOperator_ = false;
    nodeContentName_ = '#';
}

Literal::Literal(char name)
    :BaseNode(name)
{
    this->nodeContentName_ = name;
}

Automaton Literal::ToNFA(std::string alphabet, int &nodeCounter)
{
    Automaton avtomat(alphabet);

    State q("q" + std::to_string(nodeCounter++));
    State f("q" + std::to_string(nodeCounter++));

    avtomat.addState(q);
    avtomat.addState(f);

    avtomat.addTransition(q, this->nodeContentName_, f);

    avtomat.setStartState(q);
    avtomat.setOneEndState(f);

    return avtomat;
}
