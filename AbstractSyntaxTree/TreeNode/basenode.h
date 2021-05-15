#ifndef BASENODE_H
#define BASENODE_H

#include "Automata/automaton.h"
#include "vector"

class BaseNode
{
public:
    BaseNode();
    BaseNode(char name);
    BaseNode(BaseNode* left, BaseNode *right);

    virtual void ToNFA(Automaton& avtomat);
    void Operate(const std::vector<BaseNode*>& operands);

    BaseNode *left() const;
    BaseNode *right() const;

    bool IsOperator() const;

private:
    char name_;
    BaseNode* left_;
    BaseNode* right_;

protected:
    bool isOperator_;
};

#endif // BASENODE_H
