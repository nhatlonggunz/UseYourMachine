#ifndef EMPTY_H
#define EMPTY_H

#include "../basenode.h"

class Empty : public BaseNode
{
public:
    Empty();
    void ToNFA(Automaton& avtomat) override;
};

#endif // EMPTY_H
