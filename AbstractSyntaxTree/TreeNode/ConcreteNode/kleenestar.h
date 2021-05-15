#ifndef KLEENESTAR_H
#define KLEENESTAR_H

#include "../basenode.h"

class KleeneStar: public BaseNode
{
public:
    KleeneStar();

    void ToNFA(Automaton& avtomat) override;
};

#endif // KLEENESTAR_H
