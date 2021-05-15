#include "abstractsyntaxtree.h"

AbstractSyntaxTree::AbstractSyntaxTree()
{
    regex_ = "";
}

AbstractSyntaxTree::AbstractSyntaxTree(std::string regex)
{

}

BaseNode* AbstractSyntaxTree::BuildFromTokens(std::vector<std::string> listTokens)
{

}
