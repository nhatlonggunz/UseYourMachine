#ifndef UNION_H
#define UNION_H

#include "../basenode.h"

class Union: public BaseNode
{
public:
    Union();
    Automaton ToNFA(std::string alphabet, int& nodeCounter) override;
};

#endif // UNION_H
