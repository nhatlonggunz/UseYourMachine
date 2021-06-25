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

bool PushdownStateLink::operator<(const PushdownStateLink &other) const
{
    /* The transition precedence is: prioritize non-empty symbol
     * (transition symbol first then pop stack symbol)
     *
     * Hence need to sort transitions by transition symbol, then pop stack symbol
     * _ must go last in consideration */

    // "_" (empty symbol) must go last in the order.
    // "_" has higher value than uppercase letter
    if(this->symbol() != other.symbol()) {
        return toupper(this->symbol()) < toupper(other.symbol());
    }

    // "_" pop stack must go last in the order
    return toupper(this->popSymbol()) < toupper(other.popSymbol());
}

std::size_t PushdownStateLinkHasher::operator()(const PushdownStateLink &s) const
{
    using std::hash;
    using std::string;
    string tmp = std::string(1, s.symbol()) + s.popSymbol() + s.pushSymbol() + s.toState().getName();

    return hash<string>()(tmp);
}
