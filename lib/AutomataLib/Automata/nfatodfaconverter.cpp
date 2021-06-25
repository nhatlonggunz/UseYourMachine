#include "nfatodfaconverter.h"
#include <algorithm>
#include <queue>

NFAToDFAConverter::NFAToDFAConverter()
{

}

NFAToDFAConverter::NFAToDFAConverter(FiniteStateAutomaton* nfa)
    : nfa_(nfa)
{

}

std::vector<State> NFAToDFAConverter::findClosure(State nfaState)
{
    std::vector<State> statesInClosure = {nfaState};
    std::unordered_set<State, StateHasher> visited;
    visited.insert(nfaState);

    /* graph traverse to all states in the closure */
    for(unsigned i = 0; i < statesInClosure.size(); ++i) {
        auto setEdges = this->nfa_->transitions()[statesInClosure[i]];

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

FiniteStateAutomaton NFAToDFAConverter::getDFA()
{
    /* Generate closures from all states */
    for(auto&& state: this->nfa_->listStates()) {
        closures_[state] = findClosure(state);
    }

    FiniteStateAutomaton dfa = ConvertToDFA();
    return dfa;
}

State NFAToDFAConverter::createDfaState(std::vector<State> nfaStates)
{
    if(nfaStates.empty())
        return State("_");

    std::sort(nfaStates.begin(), nfaStates.end(), NFAToDFAStateComparer);

    /* Combine nfaStates to create dfa state name */
    std::string stateName = "";
    for(auto&& state: nfaStates) {
        stateName += state.getName() + ",";
    }
    stateName.pop_back();

    return State(stateName);
}

std::vector<State> NFAToDFAConverter::dfaStateToNFAStates(State dfaState)
{
    std::vector<State> listStates;

    std::string s = dfaState.getName();
    std::string delimiter = ",";

    size_t pos = 0;
    std::string token;
    while ((pos = s.find(delimiter)) != std::string::npos) {
        token = s.substr(0, pos);
        s.erase(0, pos + delimiter.length());
        listStates.push_back(State(token));
    }
    token = s;
    listStates.emplace_back(State(token));

    return listStates;
}

void NFAToDFAConverter::exploreState(State curState, FiniteStateAutomaton& dfa, std::queue<State>& qu)
{
    std::vector<State> reachableStates[26];
    std::vector<State> nfaStates = dfaStateToNFAStates(curState);

    for(auto&& state: nfaStates) {
        auto edges = this->nfa_->transitions()[state];

        for(auto&& link: edges) {
            for(auto&& closureState: this->closures_[link.second]) {
                if(link.first != EMPTY_SYMBOL)
                    reachableStates[link.first - 'a'].emplace_back(closureState);
            }
        }
    }

    State sink("_");

    for(char c: this->nfa_->alphabet()) {
        // There exist an unused char in alphabet
        // must connect to sink
        if(reachableStates[c - 'a'].empty()) {
            // create sink if not exist
            if(!dfa.isStateExisted(sink))
            {
                dfa.addState(sink);
                for(char ch: this->nfa_->alphabet()) {
                    dfa.addTransition(sink, ch, sink);
                }
            }

            dfa.addTransition(curState, c, sink);
        }
        else {
            State newState = createDfaState(reachableStates[c - 'a']);
            if(!dfa.isStateExisted(newState)) {
                dfa.addState(newState);
                qu.push(newState);

                for(auto&& nfaEndState: this->nfa_->listEndStates()) {
                    for(auto&& state: reachableStates[c - 'a']) {
                        if(state.getName() == nfaEndState.getName()) {
                            dfa.addEndState(newState);
                            break;
                        }
                    }
                }
            }

            dfa.addTransition(curState, c, newState);
        }
    }
}

FiniteStateAutomaton NFAToDFAConverter::ConvertToDFA()
{
    FiniteStateAutomaton dfa(this->nfa_->alphabet());

    /* Add init state */
    State initDfaState = createDfaState(findClosure(this->nfa_->startState()));
    this->dfaStates_.insert(initDfaState);

    dfa.addState(initDfaState);
    dfa.setStartState(initDfaState);

    std::queue<State> qu;
    qu.push(initDfaState);

    while(!qu.empty()) {
        State curState = qu.front();
        qu.pop();

        auto listNFAStates = this->dfaStateToNFAStates(curState);

        /* step: connect this states to all states */
        exploreState(curState, dfa, qu);
    }

    return dfa;
}

bool NFAToDFAStateComparer(State a, State b) {
    std::string nameA = a.getName();
    std::string nameB = b.getName();

    nameA.erase(0, 1);
    nameB.erase(0, 1);

    return std::stoi(nameA) < std::stoi(nameB);
}
