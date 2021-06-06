#include "abstractsyntaxtree.h"
#include "TreeNode/basenodefactory.h"
#include <algorithm>
#include "regexparser.h"
#include <iostream>

AbstractSyntaxTree::AbstractSyntaxTree()
{
    regex_ = "";
}

AbstractSyntaxTree::AbstractSyntaxTree(std::string regex)
    :regex_(regex)
{
    auto listTokens = RegexParser::Tokenize(regex);
    this->root_ = BuildFromTokens(listTokens);

    // Make alphabet characters unique
    std::sort(alphabet_.begin(), alphabet_.end());
    auto itUnique = std::unique(alphabet_.begin(), alphabet_.end());
    alphabet_.resize(std::distance(alphabet_.begin(), itUnique));
}

std::string AbstractSyntaxTree::toString()
{
    return this->root_->toString();
}

Automaton AbstractSyntaxTree::toNFA()
{
    int nodeCounter = 0;
    return this->root_->ToNFA(this->alphabet_, nodeCounter);
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
    if(currentOperator >= 'a' && currentOperator <= 'z')
        this->alphabet_.append(std::string(1,currentOperator));

    BaseNode* currentNode = BaseNodeFactory::GetNode(currentOperator);
    listTokens.pop_back();

    if(!currentNode->IsOperator())
        return currentNode;

    BaseNode* leftOperand = BuildFromTokensUtil(listTokens);
    BaseNode* rightOperand = nullptr;

    if(currentOperator != '*')
        rightOperand = BuildFromTokensUtil(listTokens);

    currentNode->Operate(std::vector<BaseNode*>{leftOperand, rightOperand});

    return currentNode;
}
