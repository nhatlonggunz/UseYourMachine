#ifndef EMPTY_H
#define EMPTY_H

#include "../basenode.h"

class Empty : public BaseNode
{
public:
    Empty();
    FiniteStateAutomaton ToNFA(std::string alphabet, int& nodeCounter) override;
};

#endif // EMPTY_H
