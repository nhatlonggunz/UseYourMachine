#include "pushdownautomaton.h"

PushDownAutomaton::PushDownAutomaton()
{

}

PushDownAutomaton::PushDownAutomaton(std::string alphabet, std::string stackAlphabet)
    : Automaton(alphabet), stackAlphabet_(stackAlphabet)
{

}

void PushDownAutomaton::addTransition(State startState, char symbol, char popSymbol, char pushSymbol, State toState)
{
    PushdownStateLink link(symbol, popSymbol, pushSymbol, toState);
    this->transitions_[startState].insert(link);
}
