set(constraintsSolver_source_files
  expression.cpp
  expressionVector2.cpp
  expressionVector3.cpp
  equationsSystem.cpp
  constraint.cpp
  DR_planner.cpp)

add_library(constraintsSolver ${constraintsSolver_source_files})