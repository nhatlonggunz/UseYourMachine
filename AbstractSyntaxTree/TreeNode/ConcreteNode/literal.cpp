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

void Literal::ToNFA(Automaton &avtomat)
{

}
