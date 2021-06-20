#include "pushdownstatelink.h"

PushdownStateLink::PushdownStateLink()
{

}

PushdownStateLink::PushdownStateLink(char symbol, char popSymbol, char pushSymbol, State toState)
    : symbol_(symbol), popSymbol_(popSymbol), pushSymbol_(pushSymbol), toState_(toState)
{

}

char PushdownStateLink::symbol() const
{
    return symbol_;
}

char PushdownStateLink::popSymbol() const
{
    return popSymbol_;
}

char PushdownStateLink::pushSymbol() const
{
    return pushSymbol_;
}

State PushdownStateLink::toState() const
{
    return toState_;
}

std::size_t PushdownStateLinkHasher::operator()(const PushdownStateLink &s) const
{
    using std::hash;
    using std::string;
    string tmp = std::string(1, s.symbol()) + s.popSymbol() + s.pushSymbol() + s.toState().getName();

    return hash<string>()(tmp);
}
