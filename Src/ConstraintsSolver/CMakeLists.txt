set(ConstraintsSolver_source_files
	Expression.cpp
	Constraint_abstr.cpp
	EquationsCluster.cpp
	ConstrCluster.cpp
	ConstraintsSystem.cpp
	ConstrSyst.cpp
)

add_library(ConstraintsSolver ${ConstraintsSolver_source_files})