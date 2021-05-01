#ifndef KLEENESTAR_H
#define KLEENESTAR_H

#include "../ioperator.h"

class KleeneStar: public IOperator, public BaseNode
{
public:
    KleeneStar();

    void ToNFA(Automaton& avtomat);
    void Operate(const std::vector<BaseNode*> operands) override;
};

#endif // KLEENESTAR_H
