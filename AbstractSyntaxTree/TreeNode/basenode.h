#ifndef BASENODE_H
#define BASENODE_H

#include "Automata/automaton.h"

class BaseNode
{
public:
    BaseNode();
    BaseNode(BaseNode* left, BaseNode *right);

    virtual void ToNFA(Automaton& avtomat);

    BaseNode *left() const;
    BaseNode *right() const;

private:
    BaseNode* left_;
    BaseNode* right_;
};

#endif // BASENODE_H
