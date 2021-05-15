#include "literal.h"

Literal::Literal()
{
    isOperator_ = false;
}

Literal::Literal(char name)
    :BaseNode(name)
{

}
