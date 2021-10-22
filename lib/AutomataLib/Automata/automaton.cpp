#include "automaton.h"
#include <algorithm>
#include <iostream>
#include <string>
#include <set>
#include "finitestateautomaton.h"

// StateLinkHasher for HashMap
std::size_t StateLinkHasher::operator()(const StateLink &s) const {
    using std::hash;
    using std::string;

    return hash<char>()(s.first) ^ (hash<string>()(s.second.getName()) << 1);
}

std::string Automaton::alphabet() const
{
    return alphabet_;
}

std::vector<State> Automaton::listStates() const
{
    return listStates_;
}

Transitions Automaton::transitions() const
{
    return transitions_;
}

State Automaton::startState() const
{
    return startState_;
}

void Automaton::setStartState(const State &startState)
{
    startState_ = startState;
}

std::vector<State> Automaton::listEndStates() const
{
    return listEndStates_;
}

void Automaton::setListEndStates(const std::vector<State> &listEndStates)
{
    listEndStates_ = listEndStates;
}

State Automaton::oneEndState() const
{
    return oneEndState_;
}

void Automaton::setOneEndState(const State &oneEndState)
{
    oneEndState_ = oneEndState;
    this->listEndStates_.emplace_back(oneEndState);
}

void Automaton::addEndState(State &s)
{
    this->listEndStates_.emplace_back(s);
    std::sort(listEndStates_.begin(), listEndStates_.end());

    auto itUnique = std::unique(listEndStates_.begin(), listEndStates_.end());
    listEndStates_.resize(std::distance(listEndStates_.begin(), itUnique));
}

void Automaton::ValidateTransitionsInput()
{
    // check if starting state is a valid state
    for(auto& it: transitions_)
    {
        auto stateInList = std::find(listStates_.begin(), listStates_.end(), State((it.first).getName()));

        if(stateInList == listStates_.end()) {
            throw std::invalid_argument("Transitions contain non-exist initial state");
        }

        auto setEnpoints = it.second;

        for(auto &itEndpoints: setEnpoints)
        {
            // check if transition symbol is a valid symbol
            if(this->alphabet_.find(std::string(1, itEndpoints.first)) == std::string::npos &&
               itEndpoints.first != EMPTY_SYMBOL)
                throw std::invalid_argument("Transitions contain invalid symbol" + std::string(1, itEndpoints.first));

            // check if next state is a valid state
            stateInList = std::find(listStates_.begin(),
                                    listStates_.end(),
                                    State((itEndpoints.second).getName()));

            if(stateInList == listStates_.end()) {
                throw std::invalid_argument("Transitions contain non-exist next state");
            }
        }
    }
}

void Automaton::DfsPopulateLanguage(
        State currentState,
        std::unordered_set<State, StateHasher> &visited,
        std::unordered_set<State, StateHasher> &belongsToCycle,
        std::vector<std::string> &language,
        std::string currentWord)
{
    visited.insert(currentState);

    // end state
    if(std::find(listEndStates_.begin(), listEndStates_.end(), currentState) != listEndStates_.end())
    {
        language.push_back(currentWord);
    }

    for(auto&& link: transitions_[currentState]) {
        State nextState = link.second;
        char weight = link.first;

        if(!visited.count(nextState) && !belongsToCycle.count(nextState)) {

            DfsPopulateLanguage(nextState,
                                visited,
                                belongsToCycle,
                                language,
                                currentWord + (weight == EMPTY_SYMBOL? "" : std::string(1, weight)));
        }
    }

    visited.erase(currentState);
}

Automaton::Automaton() {}

Automaton::Automaton(std::string alphabet)
    : alphabet_(alphabet)
{

}

Automaton::Automaton(std::string alphabet,
                     std::vector<State> listStates,
                     std::vector<State> listFinalStates,
                     Transitions transitions)
    : alphabet_(alphabet), listStates_(listStates), transitions_(transitions),
      listEndStates_(listFinalStates)
{
    ValidateTransitionsInput();

    startState_ = listStates_[0];

    // Set states in listState as final states.
    for(auto&& finalState: listFinalStates) {
        auto it = std::find(listStates_.begin(), listStates_.end(), State(finalState));

        if(it == listFinalStates.end())
            throw std::invalid_argument("An input final state does not exist in list of states");
        it->setFinal(true);
    }

    // update final states in transition function
    for(auto&& iter_f: this->transitions_){
        auto trueState = *std::find(listStates_.begin(), listStates_.end(), State(iter_f.first.getName()));
        iter_f.first.setFinal(trueState.IsFinal());

        for(auto&& iter_s: iter_f.second) {
            auto tmpTrueState = *std::find(listStates_.begin(), listStates_.end(), State(iter_s.second.getName()));
            iter_s.second.setFinal(tmpTrueState.IsFinal());
        }
    }
}

Automaton::Automaton(std::string alphabet, std::vector<State> listStates, std::vector<State> listFinalStates_)
    :alphabet_(alphabet), listStates_(listStates), listEndStates_(listFinalStates_)
{
    startState_ = listStates_[0];
}

void Automaton::addState(State state)
{
    auto itState = std::find(this->listStates_.begin(), this->listStates_.end(), state);

    if(itState != this->listStates_.end())
        return;

    this->listStates_.emplace_back(state);
}

void Automaton::addTransition(State startState, char symbol, State endState)
{
    StateLink link(symbol, endState);

    if(std::find(alphabet_.begin(), alphabet_.end(), symbol) == alphabet_.end() &&
       symbol != EMPTY_SYMBOL)
        throw std::invalid_argument("addTransition: symbol does not exist");
    if(std::find(listStates_.begin(), listStates_.end(), startState) == listStates_.end())
        throw std::invalid_argument("addTransition: start state does not exist");
    if(std::find(listStates_.begin(), listStates_.end(), link.second) == listStates_.end())
        throw std::invalid_argument("addTransition: end state does not exist");

    this->transitions_[startState].insert(link);
}

void Automaton::combineAutomaton(Automaton &other)
{
    if(this->alphabet_ != other.alphabet()) {
        throw std::invalid_argument("Combine Automaton: alphabet does not match");
    }

    // add other's list states to this list states
    for(auto state: other.listStates()) {
        if(std::find(listStates_.begin(), listStates_.end(), state) == listStates_.end())
            this->listStates_.emplace_back(state);
    }

    // move transition from other to this
    for(auto&& itStart: other.transitions()) {
        auto start = itStart.first;

        for(auto&& itLink: itStart.second) {
            this->addTransition(start.getName(), itLink.first, itLink.second.getName());
        }
    }
}

bool Automaton::isStateExisted(State &state)
{
    auto it = std::find(listStates_.begin(), listStates_.end(), state);
    return it != listStates_.end();
}

std::ostream& operator<<(std::ostream& os, const Automaton& avtomat)
{
    std::string output = "";
    output += avtomat.alphabet_ + "\n";
    output += "\n";

    for(auto&& state: avtomat.listStates_) {
        output += state.getName() + ",";
    }
    output.erase(output.size() - 1, 1); // remove last ","
    output += "\n";
    output += "\n";


    for(auto&& state: avtomat.listStates_) {
        if(state.IsFinal())
            output += state.getName() + ",";
    }
    output.erase(output.size() - 1, 1); // remove last ","
    output += "\n";
    output += "\n";

    for(auto&& s_iter: avtomat.transitions_)
    {
        State start = s_iter.first;
        auto setLinks = s_iter.second;

        for(auto& link: setLinks) {
            output += start.getName() +
                      "," +
                      std::string(1, link.first) +
                      "-->" +
                      (link.second).getName() +
                      "\n";
        }
    }

    output += "\n";
    output += "\n";

    output += "Is DFA: ";
    output += (avtomat.IsDFA()? "y" : "n");

    os << output;

    return os;
}
