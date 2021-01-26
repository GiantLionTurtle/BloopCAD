

#include <constraintsSolver/DR_planner.hpp>
#include <utils/errorLogger.hpp>

#include <iostream>
  
int main() 
{	
	errorLogger::get_instance().init();

	/* Super simple graph, dense with k = 0 */
	// vertex_ptr v1(new vertex(2));
	// vertex_ptr v2(new vertex(3));
	// vertex_ptr v3(new vertex(4));

	// edge_ptr e1(new edge(v1, v2, 4));
	// edge_ptr e2(new edge(v2, v3, 3));
	// edge_ptr e3(new edge(v3, v1, 3));

	// bipartite_graph G({v1, v2, v3}, {e1, e2, e3});

	// vertex_ptr v1(new vertex(3));
	// vertex_ptr v2(new vertex(3));
	// vertex_ptr v3(new vertex(3));
	// vertex_ptr v4(new vertex(3));
	// vertex_ptr v5(new vertex(3));
	
	// edge_ptr e1(new edge(v1, v2, 2));
	// edge_ptr e2(new edge(v2, v3, 2));
	// edge_ptr e3(new edge(v3, v4, 2));
	// edge_ptr e4(new edge(v4, v5, 2));
	// edge_ptr e5(new edge(v5, v1, 2));
	// edge_ptr e6(new edge(v3, v5, 2));

	// bipartite_graph G({v1, v2, v3, v4, v5}, {e1, e2, e3, e4, e5, e6});

	cluster_ptr a(new cluster(2));
	cluster_ptr b(new cluster(2));
	cluster_ptr c(new cluster(2));
	cluster_ptr d(new cluster(2));
	cluster_ptr e(new cluster(2));
	cluster_ptr f(new cluster(2));	

	edge_ptr e1(new edge(b, a, 1));
	edge_ptr e2(new edge(a, d, 1));
	edge_ptr e3(new edge(d, e, 1));
	edge_ptr e4(new edge(e, f, 1));
	edge_ptr e5(new edge(f, c, 1));
	edge_ptr e6(new edge(e, c, 1));
	edge_ptr e7(new edge(d, c, 1));
	edge_ptr e8(new edge(a, c, 1));
	edge_ptr e9(new edge(b, c, 1));

	cluster_ptr G(new cluster({ a, b, c, d, e, f }, { e1, e2, e3, e4, e5, e6, e7, e8, e9 }));

	// bipartite_graph U({a, b}, {e1});
	// std::cout<<"U density: "<<U.density()<<"\n";

	// cluster_ptr a(new cluster(2));
	// cluster_ptr b(new cluster(2));
	// cluster_ptr c(new cluster(2));
	// cluster_ptr d(new cluster(2));
	// cluster_ptr e(new cluster(2));
	// cluster_ptr f(new cluster(2));	
	// cluster_ptr g(new cluster(2));	
	// cluster_ptr h(new cluster(2));	
	// cluster_ptr i(new cluster(2));	

	// edge_ptr e1(new edge(a, b, 1));
	// edge_ptr e2(new edge(b, c, 1));
	// edge_ptr e3(new edge(c, a, 1));
	// edge_ptr e4(new edge(b, d, 1));
	// edge_ptr e5(new edge(c, d, 1));

	// edge_ptr e6(new edge(d, e, 1));
	// edge_ptr e7(new edge(d, f, 1));
	// edge_ptr e8(new edge(f, g, 1));
	// edge_ptr e9(new edge(e, g, 1));
	// edge_ptr e10(new edge(e, f, 1));

	// edge_ptr e11(new edge(g, h, 1));
	// edge_ptr e12(new edge(g, i, 1));
	// edge_ptr e13(new edge(i, a, 1));
	// edge_ptr e14(new edge(h, a, 1));
	// edge_ptr e15(new edge(h, i, 1));

	// cluster_ptr G(new cluster({ a, b, c, d, e, f, g, h, i}, { e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15 }));

	// bipartite_graph G({ a, b, c, d, e, f, g, h, i}, { e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15 });

	// std::cout<<"E1: "<<e1->capacity<<" => "<<e1->flow_a<<",  "<<e1->flow_b<<"\n";
	// std::cout<<"E2: "<<e2->capacity<<" => "<<e2->flow_a<<",  "<<e2->flow_b<<"\n";
	// std::cout<<"E3: "<<e3->capacity<<" => "<<e3->flow_a<<",  "<<e3->flow_b<<"\n";
	// // std::cout<<"E4: "<<e4->capacity<<" => "<<e4->flow_a<<",  "<<e4->flow_b<<"\n";
	// // std::cout<<"E5: "<<e5->capacity<<" => "<<e5->flow_a<<",  "<<e5->flow_b<<"\n";
	// // std::cout<<"E6: "<<e6->capacity<<" => "<<e6->flow_a<<",  "<<e6->flow_b<<"\n";


	// std::cout<<"--------------------------------------\n";
	
	// std::cout<<"Dis1: "<<G.distribute(e1)<<"\n";
	// std::cout<<"Dis2: "<<G.distribute(e2)<<"\n";
	// std::cout<<"Dis3: "<<G.distribute(e3)<<"\n";

	// std::cout<<"E1: "<<e1->capacity<<" => "<<e1->flow_a<<",  "<<e1->flow_b<<"\n";
	// std::cout<<"E2: "<<e2->capacity<<" => "<<e2->flow_a<<",  "<<e2->flow_b<<"\n";
	// std::cout<<"E3: "<<e3->capacity<<" => "<<e3->flow_a<<",  "<<e3->flow_b<<"\n";
	// std::cout<<"E4: "<<e4->capacity<<" => "<<e4->flow_a<<",  "<<e4->flow_b<<"\n";
	// std::cout<<"E5: "<<e5->capacity<<" => "<<e5->flow_a<<",  "<<e5->flow_b<<"\n";
	// std::cout<<"E6: "<<e6->capacity<<" => "<<e6->flow_a<<",  "<<e6->flow_b<<"\n";

	cluster_ptr G_ = G->extend(G->minimal(-4));
	// vertex_ptr v;
	// U.dense(-4, G_, v);
	if(G_) {
		std::cout<<"G_:\n";
		for(int i = 0; i < G_->subClusters().size(); ++i) {
			std::cout<<"ID: "<<G_->subClusters()[i]->name()<<"\n";
		}
	} else {
		LOG_ERROR("G_ could not be created.");
	}

    return 0; 
} 


// #include <iostream>

// #include <constraintsSolver/expression.hpp>
// #include <constraintsSolver/equationsSystem.hpp>
// #include <constraintsSolver/DR_planner.hpp>

// #include <entities/geometry/point_abstract.hpp>
// #include <utils/errorLogger.hpp>


// #include <glm/gtx/string_cast.hpp>

// int main()
// {
// 	errorLogger::get_instance().init();


// 	node_ptr 	n1(new node({nullptr, 3, 0, 0, 0, 0, 1})), n2(new node({nullptr, 3, 0, 0, 0, 0, 2})), n3(new node({nullptr, 3, 0, 0, 0, 0, 3})),
// 				n4(new node({nullptr, 3, 0, 0, 0, 0, 4})), n5(new node({nullptr, 3, 0, 0, 0, 0, 5}));
// 	edge_ptr 	e1(new edge{n1, n2, nullptr, 2, 0, 0, 0, 0, 0, 1}), e2(new edge{n2, n3, nullptr, 2, 0, 0, 0, 0, 0, 2}), 
// 				e3(new edge{n3, n4, nullptr, 2, 0, 0, 0, 0, 0, 3}), e4(new edge{n4, n5, nullptr, 2, 0, 0, 0, 0, 0, 4}), 
// 				e5(new edge{n5, n1, nullptr, 2, 0, 0, 0, 0, 0, 5}), e6(new edge{n3, n5, nullptr, 2, 0, 0, 0, 0, 0, 6});

// 	network_ptr G(new network({{n1, n2, n3, n4, n5}, {e1, e2, e3, e4, e5, e6}}));

	
// 	// node_ptr n1(new node({nullptr, 2, 0, 0, 0, 0, 1})), n2(new node({nullptr, 3, 0, 0, 0, 0, 2})), n3(new node({nullptr, 4, 0, 0, 0, 0, 3}));
// 	// edge_ptr e1(new edge{n1, n2, nullptr, 3, 0, 0, 0, 0, 0}), 
// 	// e2(new edge{n2, n3, nullptr, 4, 0, 0, 0, 0, 0}), e3(new edge{n3, n1, nullptr, 3, 0, 0, 0, 0, 0});

// 	// network_ptr G(new network({{n1, n2, n3}, {e1, e2, e3}}));

// 	// std::cout<<"TryDisE1: "<<distribute(G, e1)<<"\n";
// 	// std::cout<<"TryDisE2: "<<distribute(G, e2)<<"\n";
// 	// std::cout<<"TryDisE3: "<<distribute(G, e3)<<"\n";

// 	// std::cout<<"E1: "<<e1->pathCap<<" => "<<e1->flowa<<",  "<<e1->flowb<<"\n";
// 	// std::cout<<"E2: "<<e2->pathCap<<" => "<<e2->flowa<<",  "<<e2->flowb<<"\n";
// 	// std::cout<<"E3: "<<e3->pathCap<<" => "<<e3->flowa<<",  "<<e3->flowb<<"\n";

// 	// restore_flow(e1);
// 	// restore_flow(e2);
// 	// restore_flow(e3);
// 	// n1->pathCap = 0;
// 	// n2->pathCap = 0;
// 	// n3->pathCap = 0;

// 	std::vector<node_ptr> subgraph = dense(graph_ptr(new graph{G->N, G->M}));
// 	std::cout<<"Dense subgraph: "<<subgraph.size()<<"\n";

// 	for(int i = 0; i < subgraph.size(); ++i) {
// 		std::cout<<"ID: "<<subgraph[i]->name<<"\n";
// 	}
	
// 	// print_node(source);

// 	return 0;
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