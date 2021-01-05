
#include "equationsSystem.hpp"

equationsSystem::equationsSystem()
{

}
equationsSystem::equationsSystem(std::vector<expression_ptr> eqs):
    mEquations(eqs)
{

}

void equationsSystem::add_equation(expression_ptr eq)
{
    mEquations.push_back(eq);
}
void equationsSystem::add_equations(std::vector<expression_ptr> eqs)
{
    mEquations.insert(mEquations.end(), eqs.begin(), eqs.end());
}

bool equationsSystem::satisfied()
{
    for(size_t i = 0; i < mEquations.size(); ++i) {
        if(std::abs(mEquations[i]->eval()) >= kEpsilon)
            return false;
    }
    return true;
}