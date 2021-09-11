set(ConstraintsSolver_source_files
  Expression.cpp
  ExpressionVec.cpp
  Constraint_abstr.cpp
  EquationsCluster.cpp
  ConstraintsSystem.cpp)

add_library(ConstraintsSolver ${ConstraintsSolver_source_files})