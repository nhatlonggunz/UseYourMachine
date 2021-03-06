#ifndef STATE_H
#define STATE_H

#include <string>
#include <functional>

class State
{
private:
    std::string name;
    mutable bool isFinal_;

public:
    State();
    State(std::string name);

    std::string getName() const;

    bool IsFinal() const;
    void setFinal(bool isFinal) const;

    bool operator== (const State& other) const;
    bool operator< (const State& other) const;
};

struct StateHasher {
  std::size_t operator()(const State& s) const;
};

#endif // STATE_H
