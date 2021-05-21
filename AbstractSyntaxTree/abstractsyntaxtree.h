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

    std::string toString();

private:
    std::string regex_;
    BaseNode* root_;

    // a wrapper to the recursive tree building
    BaseNode* BuildFromTokens(std::vector<std::string> listTokens);

    // Recursively build the tree
    // listTokens must be in reverse order
    BaseNode* BuildFromTokensUtil(std::vector<std::string>& listTokens);
};

#endif // ABSTRACTSYNTAXTREE_H
