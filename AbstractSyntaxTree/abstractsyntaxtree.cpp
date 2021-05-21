#include "abstractsyntaxtree.h"
#include "TreeNode/basenodefactory.h"
#include <algorithm>
#include "regexparser.h"

AbstractSyntaxTree::AbstractSyntaxTree()
{
    regex_ = "";
}

AbstractSyntaxTree::AbstractSyntaxTree(std::string regex)
    :regex_(regex)
{
    auto listTokens = RegexParser::Tokenize(regex);
    this->root_ = BuildFromTokens(listTokens);
}

std::string AbstractSyntaxTree::toString()
{
    return this->root_->toString();
}

BaseNode* AbstractSyntaxTree::BuildFromTokens(std::vector<std::string> listTokens)
{
    std::reverse(listTokens.begin(), listTokens.end());
    BaseNode* result = BuildFromTokensUtil(listTokens);
    return result;
}

BaseNode *AbstractSyntaxTree::BuildFromTokensUtil(std::vector<std::string> &listTokens)
{
    char currentOperator = listTokens.back()[0];

    BaseNode* currentNode = BaseNodeFactory::GetNode(currentOperator);
    listTokens.pop_back();

    if(!currentNode->IsOperator())
        return currentNode;

    BaseNode* leftOperand = BuildFromTokensUtil(listTokens);
    BaseNode* rightOperand = BuildFromTokensUtil(listTokens);

    currentNode->Operate(std::vector<BaseNode*>{leftOperand, rightOperand});

    return currentNode;
}
