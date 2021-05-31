#include "kleenestar.h"

KleeneStar::KleeneStar()
{
    isOperator_ = true;
    nodeContentName_ = '*';
}

Automaton KleeneStar::ToNFA(std::string alphabet, int &nodeCounter)
{
    Automaton leftNFA = this->left_->ToNFA(alphabet, nodeCounter);

    Automaton avtomat(alphabet);
    avtomat.combineAutomaton(leftNFA);

    State q("q" + std::to_string(nodeCounter));
    State f("f" + std::to_string(nodeCounter));

    avtomat.addState(q);
    avtomat.addTransition(q, EMPTY_SYMBOL, leftNFA.startState());

    avtomat.addState(f);
    avtomat.addTransition(leftNFA.oneEndState(), EMPTY_SYMBOL, f);

    avtomat.addTransition(leftNFA.oneEndState(), EMPTY_SYMBOL, leftNFA.startState());

    avtomat.addTransition(q, EMPTY_SYMBOL, f);

    avtomat.setStartState(q);
    avtomat.setOneEndState(f);

    return avtomat;
}

void KleeneStar::Operate(const std::vector<BaseNode *> &operands)
{
    this->left_ = operands[0];
}

std::string KleeneStar::toString()
{
    return "*(" + this->left_->toString() + ")";
}
