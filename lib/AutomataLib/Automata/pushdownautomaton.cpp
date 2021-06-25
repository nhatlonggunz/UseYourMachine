#include "pushdownautomaton.h"
#include "algorithm"

PushDownAutomaton::PushDownAutomaton()
{

}

PushDownAutomaton::PushDownAutomaton(std::string alphabet, std::string stackAlphabet)
    : Automaton(alphabet), stackAlphabet_(stackAlphabet)
{

}

PushDownAutomaton::PushDownAutomaton(std::string alphabet,
                                     std::string stackAlphabet,
                                     std::vector<State> listStates,
                                     std::vector<State> listFinalStates,
                                     PushdownTransitions transitions)
    : Automaton(alphabet, listStates, listFinalStates),
      stackAlphabet_(stackAlphabet),
      transitions_(transitions)
{
    // need to validate transition here

    // Sort the adjcency list of each node following precedence rule
//    std::sort(transitions_.begin(), transitions_.end(), PushdownTransitionComparer);

//    for(auto&& setAdjacents: transitions) {
//        std::sort(setAdjacents.second.begin(), setAdjacents.second.end(),
//                  PushdownTransitionComparer);
//    }
}

bool PushDownAutomaton::IsWordBelongTo(std::string word)
{
    std::string stack;
    std::set<std::string> visited;
    return isWordBelongTo_util(this->startState(), 0, stack, word, visited);
}

void PushDownAutomaton::addTransition(State startState, char symbol, char popSymbol, char pushSymbol, State toState)
{
    PushdownStateLink link(symbol, popSymbol, pushSymbol, toState);
    this->transitions_[startState].insert(link);
}


bool PushDownAutomaton::isWordBelongTo_util(State currentState,
                                            int wordIndex,
                                            std::string &stack,
                                            std::string word,
                                            std::set<std::string>& visited)
{
    // accept the word if satisfy condition
    if(wordIndex == (int)word.size()) {
        auto endstates = this->listEndStates();
        if(std::find(endstates.begin(), endstates.end(), currentState)
                != endstates.end())
            return stack.empty();
    }

    if(!checkCycle(currentState, wordIndex, stack, visited))
        return false;

    // set visited
    visited.insert(currentState.getName() + "," + std::to_string(wordIndex) + "," + stack);

    // Loop through all transitions
    // Note that at this time, the transitions list is sorted
    // so that the first valid transition obeys the precedence rules

    // We don't need to consider all possible transitions
    // the PDA is made deterministc by the precedence rulse
    // hence the first valid transition must be used
    // if that transition leads to a visited state
    // then there is a loop.

    for(auto&& link: this->transitions_[currentState]) {
        if((link.symbol() == word[wordIndex] || link.symbol() == EMPTY_SYMBOL) &&
           (link.popSymbol() == EMPTY_SYMBOL ||
                (!stack.empty() && stack.back() == link.popSymbol())))
        {
            // this is a valid transition
            // now check if visited

            // next state and index
            std::string nextState = link.toState().getName();
            int nextIndex = wordIndex + (link.symbol() == EMPTY_SYMBOL? 0 : 1);

            // new processed stack
            if(link.popSymbol() != EMPTY_SYMBOL)
                stack.pop_back();
            if(link.pushSymbol() != EMPTY_SYMBOL)
                stack.push_back(link.pushSymbol());

            // if next state is visited, then there is a loop, hence the word is not valid
            if(visited.count(nextState + "," + std::to_string(nextIndex)+ "," + stack))
                return false;

            return isWordBelongTo_util(link.toState(), nextIndex, stack, word, visited);
        }
    }

    return false;
}

void PushDownAutomaton::parseVisited(std::string visited,
                                     std::string &state,
                                     int &wordIndex,
                                     std::string &stack)
{
    size_t pos;
    std::vector<std::string> listTokens;

    while((pos = visited.find(",")) != std::string::npos) {
        listTokens.emplace_back(visited.substr(0, pos));
        visited.erase(0, pos + 1);
    }

    // add last token, which does not have ","
    listTokens.emplace_back(visited.substr(0, visited.size()));

    state = listTokens[0];
    wordIndex = std::stoi(listTokens[1]);
    stack = listTokens[2];
}

bool PushDownAutomaton::checkCycle(const State &currentState,
                                   int wordIndex,
                                   const std::string &stack,
                                   const std::set<std::string> &visited)
{
    for(unsigned len = 1; len <= stack.size()/2; ++len) {
        // if we came here, then came back twice,
        // each with the same stack sequence
        // then we have a cycle at this state
        if(stack.substr(stack.size() - len, len) ==
           stack.substr(stack.size() - 2*len, len))
        {
            auto thePastStack = stack.substr(0, stack.size() - 2*len);
            if(visited.count(currentState.getName() + "," +
                             std::to_string(wordIndex) + "," +
                             thePastStack))
                return false;
        }
    }

    return true;
}

std::string PushDownAutomaton::toGraph()
{
    // start string
    std::string content = "digraph PushdownAutomaton {\n"
                          "rankdir=LR;\n"
                          "\"\" [shape=none]\n";

    // end state
    content += "node [shape = doublecircle]; ";
    for(auto&& s : this->listEndStates()) {
        content += s.getName() + " ";
    }
    content += ";\n";


    // Draw remaining nodes. Draw edges
    content += "node [shape = circle];\n";
    content += std::string("\"\" -> ") + "\"" + this->startState().getName() + "\"\n";

    for(auto&& iter_f: transitions_) {
        for(auto&& iter_s: iter_f.second) {
            std::string formattedSymbol = std::string(1, iter_s.symbol());
            if(formattedSymbol == "_")
                formattedSymbol = "\u03B5";
            content += "\"" + iter_f.first.getName() + "\"";
            content += " -> ";
            content += "\"" + iter_s.toState().getName() + "\"";

            std::string label = "" + formattedSymbol;
            if(iter_s.popSymbol() != EMPTY_SYMBOL || iter_s.pushSymbol() != EMPTY_SYMBOL)
            {
                label += "[" + std::string(1,iter_s.popSymbol()) + ", " +
                        std::string(1, iter_s.pushSymbol()) + "]";
            }

            content += std::string("[label = \"") + label + "\"];\n";
        }
    }

    content += "}";

    return content;
}

bool PushdownTransitionComparer(const PushdownStateLink &a, const PushdownStateLink &b)
{
    /* The transition precedence is: prioritize non-empty symbol
     * (transition symbol first then pop stack symbol)
     *
     * Hence need to sort transitions by transition symbol, then pop stack symbol
     * _ must go last in consideration */

    // "_" (empty symbol) must go last in the order.
    // "_" has higher value than uppercase letter
    if(a.symbol() != b.symbol()) {
        return toupper(a.symbol()) < toupper(b.symbol());
    }

    // "_" pop stack must go last in the order
    return toupper(a.popSymbol()) < toupper(b.popSymbol());
}
