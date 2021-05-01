#ifndef IOPERATOR_H
#define IOPERATOR_H

#include "basenode.h"
#include <vector>

class IOperator
{
public:
    virtual ~IOperator() {}
    virtual void Operate(const std::vector<BaseNode*> operands) = 0;
};

#endif // IOPERATOR_H
