#ifndef ABSTRACTSYNTAXTREE_H
#define ABSTRACTSYNTAXTREE_H

#include "TreeNode/basenode.h"
#include "Automata/automaton.h"

class AbstractSyntaxTree
{
public:
    AbstractSyntaxTree(std::string regex);
    AbstractSyntaxTree(BaseNode* root);

private:
    std::string regex_;
    BaseNode* root_;
};

#endif // ABSTRACTSYNTAXTREE_H
