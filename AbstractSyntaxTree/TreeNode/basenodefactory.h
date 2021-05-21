#ifndef BASENODEFACTORY_H
#define BASENODEFACTORY_H

#include "ConcreteNode/empty.h"
#include "ConcreteNode/literal.h"
#include "ConcreteNode/kleenestar.h"
#include "ConcreteNode/concatenation.h"
#include "ConcreteNode/union.h"

class BaseNodeFactory
{
public:
    static BaseNode* GetNode(char c);
};

#endif // BASENODEFACTORY_H
