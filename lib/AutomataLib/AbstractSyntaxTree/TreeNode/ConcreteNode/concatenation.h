#ifndef CONCATENATION_H
#define CONCATENATION_H

#include "../basenode.h"

class Concatenation: public BaseNode
{
public:
    Concatenation();
    FiniteStateAutomaton ToNFA(std::string alphabet, int& nodeCounter) override;
};

#endif // CONCATENATION_H
