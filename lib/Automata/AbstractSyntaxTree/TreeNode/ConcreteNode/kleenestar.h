#ifndef KLEENESTAR_H
#define KLEENESTAR_H

#include "../basenode.h"

class KleeneStar: public BaseNode
{
public:
    KleeneStar();

    Automaton ToNFA(std::string alphabet, int& nodeCounter) override;
    void Operate(const std::vector<BaseNode *> &operands) override;
    std::string toString();
};

#endif // KLEENESTAR_H
