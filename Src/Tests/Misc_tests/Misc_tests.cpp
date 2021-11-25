
// BloopCAD
// Copyright (C) 2020  BloopCorp

// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

#include <iostream>

#include <ConstrsSolver/ConstrGraph.hpp>

void build_G_allvars(ConstrGraph& G);
void build_G_reduced(ConstrGraph& G);
void build_G_superreduced(ConstrGraph& G);

int main()
{
	ConstrGraph G;
	build_G_superreduced(G);

	int nG1 = 0, nG2 = 0, nG3 = 0;
	std::vector<int> match = G.maxMatching();
	G.make_bidirectionnal(match);
	
	int nActiveClusters = G.mark_Gs(match, nG1, nG2, nG3);

	std::cout	<<"n clusters = \t"<<nActiveClusters<<"\n"
				<<"nG1 clusters = \t"<<nG1<<"\n"
				<<"nG2 clusters = \t"<<nG2<<"\n"
				<<"nG3 clusters = \t"<<nG3<<"\n";
	
	std::cout<<"Constraints clusters: \n";
	for(int i = 0; i < G.C.size(); ++i) {
		std::cout<<G.C[i].data<<" => "<<G.C[i].cluster<<"\n";
	}

	std::cout<<"Parameters clusters: \n";
	for(int i = 0; i < G.V.size(); ++i) {
		std::cout<<G.V[i].data<<" => "<<G.V[i].cluster<<"\n";
	}
}

void build_G_allvars(ConstrGraph& G)
{
		// Origin point
	G.add_var(0);
	G.add_var(1);

	// Line 1
	G.add_var(2);
	G.add_var(3);
	G.add_var(4);
	G.add_var(5);

	// Line 2
	G.add_var(6);
	G.add_var(7);
	G.add_var(8);
	G.add_var(9);

	// // Fixed point??
	G.add_var(10);
	G.add_var(11);



	// Coincidence 1
	G.add_constr(0);
	G.add_c_to_v_ind(0, 0);
	G.add_c_to_v_ind(0, 1);
	G.add_c_to_v_ind(0, 2);
	G.add_c_to_v_ind(0, 3);

	// Horizonatlity
	G.add_constr(1);
	G.add_c_to_v_ind(1, 3);
	G.add_c_to_v_ind(1, 5);

	// Perpendicularity
	G.add_constr(2);
	G.add_c_to_v_ind(2, 2);
	G.add_c_to_v_ind(2, 3);
	G.add_c_to_v_ind(2, 4);
	G.add_c_to_v_ind(2, 5);
	G.add_c_to_v_ind(2, 6);
	G.add_c_to_v_ind(2, 7);
	G.add_c_to_v_ind(2, 8);
	G.add_c_to_v_ind(2, 9);

	// Coincidence 2
	G.add_constr(3);
	G.add_c_to_v_ind(3, 4);
	G.add_c_to_v_ind(3, 5);
	G.add_c_to_v_ind(3, 6);
	G.add_c_to_v_ind(3, 7);

	// Length 1
	G.add_constr(4);
	G.add_c_to_v_ind(4, 2);
	G.add_c_to_v_ind(4, 3);
	G.add_c_to_v_ind(4, 4);
	G.add_c_to_v_ind(4, 5);

	// Length 2
	G.add_constr(5);
	G.add_c_to_v_ind(5, 6);
	G.add_c_to_v_ind(5, 7);
	G.add_c_to_v_ind(5, 8);
	G.add_c_to_v_ind(5, 9);

	// Coincidence 3
	G.add_constr(6);
	G.add_c_to_v_ind(6, 8);
	G.add_c_to_v_ind(6, 9);
	G.add_c_to_v_ind(6, 10);
	G.add_c_to_v_ind(6, 11);
}

void build_G_reduced(ConstrGraph& G)
{
	G.add_var(0);
	G.add_var(1);
	G.add_var(2);
	G.add_var(3);
	G.add_var(4);
	G.add_var(5);

	// Length 1
	G.add_constr(0);
	G.add_c_to_v_ind(0, 0);
	G.add_c_to_v_ind(0, 1);
	G.add_c_to_v_ind(0, 2);
	G.add_c_to_v_ind(0, 3);

	// Length 2
	G.add_constr(1);
	G.add_c_to_v_ind(1, 2);
	G.add_c_to_v_ind(1, 3);
	G.add_c_to_v_ind(1, 4);
	G.add_c_to_v_ind(1, 5);

	// Perpendicularity
	G.add_constr(2);
	G.add_c_to_v_ind(2, 0);
	G.add_c_to_v_ind(2, 1);
	G.add_c_to_v_ind(2, 2);
	G.add_c_to_v_ind(2, 3);
	G.add_c_to_v_ind(2, 4);
	G.add_c_to_v_ind(2, 5);
}

void build_G_superreduced(ConstrGraph& G)
{
	G.add_var(0);
	G.add_var(1);
	G.add_var(2);

	// Length 1
	G.add_constr(0);
	G.add_c_to_v_ind(0, 0);

	// Length 2
	G.add_constr(1);
	G.add_c_to_v_ind(1, 0);
	G.add_c_to_v_ind(1, 1);
	G.add_c_to_v_ind(1, 2);

	// Perpendicularity
	G.add_constr(2);
	G.add_c_to_v_ind(2, 0);
	G.add_c_to_v_ind(2, 1);
	G.add_c_to_v_ind(2, 2);
}