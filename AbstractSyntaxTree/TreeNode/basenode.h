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
    virtual void Operate(const std::vector<BaseNode*>& operands);

    BaseNode *left() const;
    BaseNode *right() const;

    bool IsOperator() const;

    virtual std::string toString();

private:
    char name_;

protected:
    bool isOperator_;
    char nodeContentName_;
    BaseNode* left_;
    BaseNode* right_;
};

#endif // BASENODE_H
