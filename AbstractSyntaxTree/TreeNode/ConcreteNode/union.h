#ifndef UNION_H
#define UNION_H

#include "../basenode.h"

class Union: public BaseNode
{
public:
    Union();
    void ToNFA(Automaton& avtomat);
};

#endif // UNION_H
