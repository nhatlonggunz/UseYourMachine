#include "basenodefactory.h"


BaseNode BaseNodeFactory::GetNode(char c)
{
    switch (c) {
    case '_':
        return Empty();
    case '.':
        return Concatenation();
    case '|':
        return Union();
    case '*':
        return KleeneStar();
    default:
        return Literal(c);
    }
}
