#ifndef LITERAL_H
#define LITERAL_H

#include "../basenode.h"

class Literal: public BaseNode
{
public:
    Literal();
    Literal(char name);
    Automaton ToNFA(std::string alphabet, int& nodeCounter) override;
};

#endif // LITERAL_H
