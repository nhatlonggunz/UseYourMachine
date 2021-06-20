#ifndef PUSHDOWNSTATELINK_H
#define PUSHDOWNSTATELINK_H

#include "state.h"

class PushdownStateLink
{
public:
    PushdownStateLink();
    PushdownStateLink(char symbol, char popSymbol, char pushSymbol, State toState);

    char symbol() const;

    char popSymbol() const;

    char pushSymbol() const;

    State toState() const;

private:
    char symbol_;
    char popSymbol_;
    char pushSymbol_;
    State toState_;
};

struct PushdownStateLinkHasher {
    std::size_t operator()(const PushdownStateLink& s) const;
};

#endif // PUSHDOWNSTATELINK_H
