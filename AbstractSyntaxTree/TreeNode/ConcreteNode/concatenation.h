#ifndef CONCATENATION_H
#define CONCATENATION_H

#include "../basenode.h"

class Concatenation: public BaseNode
{
public:
    Concatenation();
    void ToNFA(Automaton& avtomat) override;
};

#endif // CONCATENATION_H
