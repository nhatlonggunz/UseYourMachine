#ifndef LITERAL_H
#define LITERAL_H

#include "../basenode.h"

class Literal: public BaseNode
{
public:
    Literal();
    Literal(char name);
    void ToNFA(Automaton& avtomat);
};

#endif // LITERAL_H
