

// #include <constraintsSolver/DR_planner.hpp>
#include <utils/errorLogger.hpp>

#include <iostream>
#include <cmath>

int main()
{
	errorLogger::get_instance().init();
	
	// variable_ptr x1(new variable("x1", 0.0, false));
	// variable_ptr y1(new variable("y1", 0.0, false));
	// variable_ptr x2(new variable("x2", 10.0, false)); 
	// variable_ptr y2(new variable("y2", 10.0, false)); 

	// expression_ptr eq = pow(x1->expr()-x2->expr(), 2.0) - pow(y1->expr()-y2->expr(), 2.0);
	// equationsSystem sys({ eq }, { x1, y1, x2, y2 });
	
	// std::cout<<"Sys size: "<<sys.vars_size()<<",  "<<sys.equs_size()<<"\n";
	// std::cout<<"Attempting solve...\n";
	// int out = sys.solve(true);
	// std::cout<<"output: "<<out<<"\n";
	// std::cout<<"p1: ("<<x1<<",  "<<y1<<") ; p2: ("<<x2<<",  "<<y2<<")\n";
}


// int main() 
// {	
// 	errorLogger::get_instance().init();

// 	// cluster_ptr a(new cluster(2));
// 	// cluster_ptr b(new cluster(2));
// 	// cluster_ptr c(new cluster(2));
// 	// cluster_ptr d(new cluster(2));
// 	// cluster_ptr e(new cluster(2));
// 	// cluster_ptr f(new cluster(2));	

// 	// edge_ptr e1(new edge(b, a, 1));
// 	// edge_ptr e2(new edge(a, d, 1));
// 	// edge_ptr e3(new edge(d, e, 1));
// 	// edge_ptr e4(new edge(e, f, 1));
// 	// edge_ptr e5(new edge(f, c, 1));
// 	// edge_ptr e6(new edge(e, c, 1));
// 	// edge_ptr e7(new edge(d, c, 1));
// 	// edge_ptr e8(new edge(a, c, 1));
// 	// edge_ptr e9(new edge(b, c, 1));

// 	// cluster_ptr G(new cluster({ a, b, c, d, e, f }, { e1, e2, e3, e4, e5, e6, e7, e8, e9 }));

// 	// -----------------------------------------------------------------------

// 	// cluster_ptr a(new cluster(2));
// 	// cluster_ptr b(new cluster(2));
// 	// cluster_ptr c(new cluster(2));
// 	// cluster_ptr d(new cluster(2));
// 	// cluster_ptr e(new cluster(2));
// 	// cluster_ptr f(new cluster(2));	
// 	// cluster_ptr g(new cluster(2));	
// 	// cluster_ptr h(new cluster(2));	
// 	// cluster_ptr i(new cluster(2));	

// 	// edge_ptr e1(new edge(a, b, 1));
// 	// edge_ptr e2(new edge(b, c, 1));
// 	// edge_ptr e3(new edge(c, a, 1));
// 	// edge_ptr e4(new edge(b, d, 1));
// 	// edge_ptr e5(new edge(c, d, 1));

// 	// edge_ptr e6(new edge(d, e, 1));
// 	// edge_ptr e7(new edge(d, f, 1));
// 	// edge_ptr e8(new edge(f, g, 1));
// 	// edge_ptr e9(new edge(e, g, 1));
// 	// edge_ptr e10(new edge(e, f, 1));

// 	// edge_ptr e11(new edge(g, h, 1));
// 	// edge_ptr e12(new edge(g, i, 1));
// 	// edge_ptr e13(new edge(i, a, 1));
// 	// edge_ptr e14(new edge(h, a, 1));
// 	// edge_ptr e15(new edge(h, i, 1));

// 	// cluster_ptr G(new cluster({ a, b, c, d, e, f, g, h, i}, { e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15 }));

// 	// --------------------------------------------------------------------------------------

	
// 	cluster_ptr a(new cluster(2));
// 	cluster_ptr b(new cluster(2));
// 	cluster_ptr c(new cluster(2));
// 	cluster_ptr d(new cluster(2));
// 	cluster_ptr e(new cluster(2));
// 	cluster_ptr f(new cluster(2));	
// 	cluster_ptr g(new cluster(2));	
// 	cluster_ptr h(new cluster(2));	
// 	cluster_ptr i(new cluster(2));	
// 	cluster_ptr j(new cluster(2));	
// 	cluster_ptr k(new cluster(2));	

// 	edge_ptr e1(new edge(a, b, 1));
// 	edge_ptr e2(new edge(b, c, 1));
// 	edge_ptr e3(new edge(c, a, 1));
// 	edge_ptr e4(new edge(a, d, 1));
// 	edge_ptr e5(new edge(d, c, 1));
// 	edge_ptr e6(new edge(c, e, 1));
// 	edge_ptr e7(new edge(b, e, 1));

// 	edge_ptr e8(new edge(d, f, 1));
// 	edge_ptr e9(new edge(d, g, 1));
// 	edge_ptr e10(new edge(g, f, 1));
// 	edge_ptr e11(new edge(f, i, 1));
// 	edge_ptr e12(new edge(i, g, 1));
// 	edge_ptr e13(new edge(g, h, 1));
// 	edge_ptr e14(new edge(i, h, 1));

// 	edge_ptr e15(new edge(h, j, 1));
// 	edge_ptr e16(new edge(h, k, 1));
// 	edge_ptr e17(new edge(j, k, 1));
// 	edge_ptr e18(new edge(j, e, 1));
// 	edge_ptr e19(new edge(k, e, 1));

// 	cluster_ptr G(new cluster({ a, b, c, d, e, f, g, h, i, j, k}, { e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19 }));

// 	cluster_ptr skeleton = G->skeletonize(G->denseClusters(-4));
// 	std::vector<cluster_ptr> G_ = G->denseClusters(-4);
// 	// vertex_ptr v;
// 	// U.dense(-4, G_, v);
// 	// if(G_) {
// 		std::cout<<"G_:\n";
// 		for(int i = 0; i < G_.size(); ++i) {
// 			std::cout<<"ID: "<<G_[i]->name()<<"\n";
// 			for(int j = 0; j < G_[i]->subClusters().size(); ++j) {
// 				std::cout<<"\t"<<(char)(G_[i]->subClusters()[j]->name() + 'a' - 1	)<<"\n";
// 			}
// 		}
// 	// } else {
// 		// LOG_ERROR("G_ could not be created.");
// 	// }

//     return 0; 
// } 

/*
	point_abstract_ptr a = std::make_shared<point_abstract>(glm::vec3(-0.520555, 0.219129, 0)), b = std::make_shared<point_abstract>(glm::vec3(3, 4, 0));
	std::vector<expression_ptr> eqs {
		a->pos()->x->expr() - b->pos()->x->expr(),
		a->pos()->y->expr() - b->pos()->y->expr()
	};
	std::vector<variable_ptr> vars {
		a->pos()->x, a->pos()->y,
		b->pos()->x, b->pos()->y
	};
	equationsSystem sys(eqs, vars);

	std::cout<<"Start: "<<glm::to_string(a->pos_val())<<" - "<<glm::to_string(b->pos_val())<<"\n";
	std::cout<<"Sys size: "<<sys.vars_size()<<",  "<<sys.equs_size()<<"\n";
	std::cout<<"Sys solved: "<<sys.satisfied()<<"\n";
	std::cout<<"Sys solve attempt: "<<sys.solve()<<"\n";
	std::cout<<"Sys solved: "<<sys.satisfied()<<"\n";
	std::cout<<"End: "<<glm::to_string(a->pos_val())<<" - "<<glm::to_string(b->pos_val())<<"\n";

*/

/*
	// Three variables, three equations


	variable_ptr x1(new variable("x1", 0.1, false));
	variable_ptr x2(new variable("x2", 0.1, false));
	variable_ptr x3(new variable("x3", -0.1, false));
	expression_ptr eq1 = 3 * x1->expr() - cos(x2->expr() * x3->expr()) - 0.5f;
	expression_ptr eq2 = pow(x1->expr(), 2.0f) - 81 * pow(x2->expr() + 0.1, 2.0f) + sin(x3->expr()) + 1.06;
	expression_ptr eq3 = exp(-x1->expr()*x2->expr()) + 20*x3->expr() + (10.0f * expConst::pi - 3.0f) / 3.0f;
	equationsSystem sys({ eq1, eq2, eq3 }, { x1, x2, x3 });
	
	std::cout<<"Sys size: "<<sys.vars_size()<<",  "<<sys.equs_size()<<"\n";
	std::cout<<"Sys solved: "<<sys.satisfied()<<"\n";
	std::cout<<"Sys solve attempt: "<<sys.solve()<<"\n";
	std::cout<<"Sys solved: "<<sys.satisfied()<<"\n";
*/

/*
	// Two variables, one equation


	variable_ptr x1 = std::make_shared<variable>("x1", 0.0f), x2 = std::make_shared<variable>("x2", 0.0f);
	x1->set_fixed(true);
	x2->set_fixed(true);
	expression_ptr expr = pow(pow(x1->expr(), 2.0f) + x2->expr() - 11.0f, 2.0f) + pow(x1->expr() + pow(x2->expr(), 2.0f) - 7.0f, 2.0f);

	std::cout<<"Expr: "<<expr<<"\n";
	x1->set_fixed(false);
	std::cout<<"DX1: "<<expr->d()->eval()<<"\n";
	x1->set_fixed(true);
	x2->set_fixed(false);
	std::cout<<"DX1: "<<expr->d()->eval()<<"\n";
	std::cout<<"Eq_i: "<<expr->eval()<<"\n";
	
	equationsSystem sys({ expr }, { x1, x2 });
	std::cout<<"Attempt solve: "<<sys.solve()<<"\n";
	std::cout<<"X1: "<<x1->val()<<"\n";
	std::cout<<"X2: "<<x2->val()<<"\n";
	std::cout<<"Eq_s: "<<expr->eval()<<"\n";
*/