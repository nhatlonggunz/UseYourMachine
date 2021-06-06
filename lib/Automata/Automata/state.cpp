#include "state.h"

State::State()
{
    this->name = "";
}

State::State(std::string name): name(name), isFinal_(false)
{
}

std::string State::getName() const
{
    return this->name;
}

bool State::IsFinal() const
{
    return this->isFinal_;
}

void State::setFinal(bool isFinal) const
{
    this->isFinal_ = isFinal;
}

bool State::operator==(const State &other) const
{
    return this->name == other.name;
}

bool State::operator<(const State &other) const
{
    return this->name < other.name;
}

std::size_t StateHasher::operator()(const State &s) const
{
    using std::size_t;
    using std::hash;
    using std::string;

    return std::hash<std::string>()(s.getName());
}
