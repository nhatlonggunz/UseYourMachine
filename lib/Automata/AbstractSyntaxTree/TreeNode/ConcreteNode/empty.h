#ifndef EMPTY_H
#define EMPTY_H

#include "../basenode.h"

class Empty : public BaseNode
{
public:
    Empty();
    Automaton ToNFA(std::string alphabet, int& nodeCounter) override;
};

#endif // EMPTY_H
