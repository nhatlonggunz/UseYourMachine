#ifndef ABSTRACTSYNTAXTREE_H
#define ABSTRACTSYNTAXTREE_H

#include "TreeNode/basenode.h"
#include "Automata/automaton.h"
#include <vector>

class AbstractSyntaxTree
{
public:
    AbstractSyntaxTree();
    AbstractSyntaxTree(std::string regex);
    AbstractSyntaxTree(BaseNode* root);

private:
    std::string regex_;
    BaseNode* root_;

    BaseNode* BuildFromTokens(std::vector<std::string> listTokens);
};

#endif // ABSTRACTSYNTAXTREE_H
