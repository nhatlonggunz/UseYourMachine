#include "basenode.h"

BaseNode::BaseNode()
{

}

BaseNode::BaseNode(char name)
    :name_(name)
{

}

BaseNode::BaseNode(BaseNode *left, BaseNode *right)
    :left_(left), right_(right)
{

}

void BaseNode::ToNFA(Automaton &avtomat)
{

}

void BaseNode::Operate(const std::vector<BaseNode *> &operands)
{
    if(operands.size() != 2)
        throw std::invalid_argument("there must be 2 operands for a node to operate.");
    this->left_ = operands[0];
    this->right_ = operands[1];
}

BaseNode *BaseNode::left() const
{
    return left_;
}

BaseNode *BaseNode::right() const
{
    return right_;
}

bool BaseNode::IsOperator() const
{
    return isOperator_;
}

std::string BaseNode::toString()
{
    std::string content = "";

    if(this->IsOperator()) {
        std::string Left = this->left_->toString();
        std::string Right = this->right_->toString();

        content = "(" +
                  Left +
                  this->nodeContentName_ +
                  Right +
                  ")";
    }
    else {
        content = this->nodeContentName_;
    }

    return content;
}
