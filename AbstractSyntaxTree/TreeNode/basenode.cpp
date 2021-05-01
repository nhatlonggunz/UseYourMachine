#include "basenode.h"

BaseNode::BaseNode()
{

}

BaseNode::BaseNode(BaseNode *left, BaseNode *right)
    :left_(left), right_(right)
{

}

BaseNode *BaseNode::left() const
{
    return left_;
}

BaseNode *BaseNode::right() const
{
    return right_;
}
