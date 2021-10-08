
#include "Constraint_abstr.hpp"

#include "Expression.hpp"

Constraint_abstr::Constraint_abstr()
{

}
Constraint_abstr::~Constraint_abstr()
{

}
double Constraint_abstr::error(size_t ind)
{
    return equ(ind)->eval();
}
bool Constraint_abstr::satisfied()
{
    for(size_t i = 0; i < n_equs(); ++i) {
        if(std::abs(equ(i)->eval()) > 1e-12)
            return false;
    }
    return true;
}
double Constraint_abstr::derivative(var_ptr withRespectTo, size_t from_eq)
{
    // TODO: would going through the constraint's variables to check if the target
    // is in the constraint save time? ; output zero if the variable is unknown to this constraint
    withRespectTo->set_as_var();
    double der = equ(from_eq)->d()->eval();
    withRespectTo->set_as_coeff();
    return der;
}
