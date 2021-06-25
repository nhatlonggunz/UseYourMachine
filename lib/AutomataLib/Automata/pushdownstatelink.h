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

    bool operator<(const PushdownStateLink& other) const;

private:
    char symbol_;
    char popSymbol_;
    char pushSymbol_;
    State toState_;
};

struct PushdownStateLinkHasher {
    std::size_t operator()(const PushdownStateLink& s) const;
};

inline bool operator == (PushdownStateLink const& lhs, PushdownStateLink const& rhs)
{
    return lhs.symbol() == rhs.symbol() &&
            lhs.popSymbol() == rhs.popSymbol() &&
            lhs.pushSymbol() == rhs.pushSymbol() &&
            lhs.toState() == rhs.toState();
}

#endif // PUSHDOWNSTATELINK_H
