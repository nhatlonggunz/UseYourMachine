#include "basenodefactory.h"


BaseNode* BaseNodeFactory::GetNode(char c)
{
    switch (c) {
    case '_':
        return new Empty();
    case '.':
        return new Concatenation();
    case '|':
        return new Union();
    case '*':
        return new KleeneStar();
    default:
        return new Literal(c);
    }
}
