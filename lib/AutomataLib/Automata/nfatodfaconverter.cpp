#include "nfatodfaconverter.h"

NFAToDFAConverter::NFAToDFAConverter()
{

}

NFAToDFAConverter::NFAToDFAConverter(Automaton nfa)
    : nfa_(nfa)
{
    for(auto&& state: nfa.listStates()) {
        closures_[state] = findClosure(state);
    }
}

std::vector<State> NFAToDFAConverter::findClosure(State nfaState)
{
    std::vector<State> statesInClosure = {nfaState};
    std::unordered_set<State, StateHasher> visited;
    visited.insert(nfaState);

    /* graph traverse to all states in the closure */
    for(unsigned i = 0; i < statesInClosure.size(); ++i) {
        auto setEdges = this->nfa_.transitions()[statesInClosure[i]];

        // Traverse through all Epsilon edges
        for(auto&& link: setEdges) {
            if(link.first == EMPTY_SYMBOL) {
                if(!visited.count(link.second)) {
                    visited.insert(link.second);
                    statesInClosure.emplace_back(link.second);
                }
            }
        }
    }

    return statesInClosure;
}
