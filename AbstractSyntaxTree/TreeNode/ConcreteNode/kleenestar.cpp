#include "kleenestar.h"

KleeneStar::KleeneStar()
{
    isOperator_ = true;
    nodeContentName_ = '*';
}

void KleeneStar::ToNFA(Automaton &avtomat)
{

}

void KleeneStar::Operate(const std::vector<BaseNode *> &operands)
{
    this->left_ = operands[0];
}

std::string KleeneStar::toString()
{
    return "(" + this->left_->toString() + ")";
}
