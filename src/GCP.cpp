// GCP.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include "prep.h"
#include "gurobi_c++.h"
#include "boost/dynamic_bitset/dynamic_bitset.hpp"
#include "heugcol.h"
#include "bgraph.h"
#include <algorithm>

#include "boost/graph/sequential_vertex_coloring.hpp"
#include "boost/graph/smallest_last_ordering.hpp"
#include "dsatur_boost.hpp"

// #include <algorithm> // std::min_element
#include "boost/compute/algorithm/min_element.hpp"

// data structure to store edges

void gurobiGcpModel(GRBModel &model, surya::Graph const &inputGraph);
void printSolution(GRBModel& model, GRBVar* colors, GRBVar* vertex_color, int vertex_count);
int myrandom (int i) { return std::rand()%i;}

int main()
{	
	bool build_model{false};
	// std::cout << "Surya" << std::endl;

	std::string filepath = "/home/surya/orstuff/instances/le450_5d.col"; //DSJC1000.1.col
	int vertex_count = 0; // will be filled by readInput


	std::vector<Edge> edges = readInput(filepath.c_str(), vertex_count);

	int edge_count = edges.size();
	// std::cout << "Vertex count: " << vertex_count << std::endl;
	// std::cout << "Input Edge count: " << edge_count << std::endl;

	typedef MyGraphAdjListType Graph;

	Graph G_al(vertex_count);
	
	// add edges to the boost graph from our edge list (our edge list from DIMACS is assumed to be 1-indexed)
	for (const auto& i : edges)
	{
		// std::cout << i.src << " "  << i.dest << std::endl;
		boost::add_edge(boost::vertex(i.src - 1, G_al), boost::vertex(i.dest - 1, G_al), G_al);
	};

	auto bvcount {boost::num_vertices(G_al)};
	boost::graph_traits< Graph >::vertices_size_type bvcount_t {boost::num_vertices(G_al)};

	std::cout << "Number of vertices in boost graph: " << boost::num_vertices(G_al) << std::endl;
	std::cout << "Number of edges in boost graph: " << boost::num_edges(G_al) << std::endl;

	typename boost::graph_traits< Graph >::vertex_descriptor zero, one, two;
	typename boost::graph_traits< Graph >::out_edge_iterator out, out_end;
    typename boost::graph_traits< Graph >::in_edge_iterator in, in_end;
	typename boost::graph_traits< Graph >::adjacency_iterator adj, adj_end;
/*
	zero = vertex(0, G_al);
    one = vertex(1, G_al);
    two = vertex(2, G_al);
*/
	/*
	std::cout << "out_edges(0):";
    for (boost::tie(out, out_end) = out_edges(zero, G_al); out != out_end;
         ++out)
        std::cout << ' ' << *out;
    std::cout << std::endl << "in_edges(0):";
    for (boost::tie(in, in_end) = in_edges(zero, G_al); in != in_end; ++in)
        std::cout << ' ' << *in;
    std::cout << std::endl;
	*/
	/*
	std::cout << std::endl << "adjacency(0):";
	// adjacent_vertices return an iterator-range for adjacent vertices of given vertex
	// tie creates a tier (a tuple that holds references to objects)
	for (boost::tie(adj, adj_end) = boost::adjacent_vertices(zero, G_al); adj != adj_end;
         ++adj)
        std::cout << ' ' << *adj;
	std::cout << std::endl;

	std::cout << std::endl << "adjacency(1):";
	for (boost::tie(adj, adj_end) = boost::adjacent_vertices(one, G_al); adj != adj_end;
         ++adj)
        std::cout << ' ' << *adj;
	std::cout << std::endl;
    */
	// std::cout << "out-degree of vertex(0) = " << out_degree(zero, G_al) << std::endl;
	// std::cout << "out-degree of vertex(1) = " << out_degree(one, G_al) << std::endl;

	// std::cout << "edge (0, 1) exists?: " << std::boolalpha << boost::edge(zero, one, G_al).second << std::endl;
	// std::cout << "edge (1, 0) exists?: " << std::boolalpha << boost::edge(one, zero, G_al).second << std::endl;
    


	surya::Graph inputGraph(edges, vertex_count, true);
	// std::cout << "graph class edge count = " << inputGraph.edgeCount() << std::endl << std::endl;
	
	typedef typename boost::graph_traits<Graph>::vertex_descriptor vertex_descriptor;
	typedef typename boost::graph_traits<Graph>::vertices_size_type vertices_size_type;
	typedef typename boost::property_map<Graph, boost::vertex_index_t>::const_type vertex_index_map;

	std::vector<vertices_size_type> color_vec(num_vertices(G_al));
	boost::iterator_property_map<vertices_size_type*, vertex_index_map>
		color(&color_vec.front(), get(boost::vertex_index, G_al));
/*
	vertices_size_type num_colors = sequential_vertex_coloring(G_al, color);
	std::cout << "Coloring UB: " << num_colors << std::endl;
*/
    typedef typename boost::graph_traits< Graph >::vertex_iterator
        vertex_iterator;

    std::pair< vertex_iterator, vertex_iterator > v = vertices(G_al);
	
    std::vector< vertex_descriptor > order(v.first, v.second);
/*	
	std::vector< typename boost::graph_traits< Graph >::vertex_descriptor >
        o = boost::smallest_last_vertex_ordering(G_al);

	//auto ss = max_element(o.begin(), o.end() );

	//std::cout << "Coloring UB_smallest_last: " << *ss << std::endl;

	std::vector< typename boost::graph_traits< Graph >::vertex_descriptor >
        my_order{order};


	std::sort(
		my_order.begin(), my_order.end(),
		[&G_al](auto const &a, auto const &b) {return boost::out_degree(a, G_al) > boost::out_degree(b, G_al);}
	);

	std::vector<vertices_size_type> degrees(bvcount);
	for (auto i=0; i < bvcount; i++)
	{
		degrees[i] = out_degree(order[i], G_al);
		// std::cout << out_degree(order[i], G_al) << " " << degrees[i] << std::endl;
	}

/*
	vertices_size_type num_colors2
							= boost::sequential_vertex_coloring(G_al, 
								boost::make_iterator_property_map(my_order.begin(), boost::identity_property_map(),
            					boost::graph_traits< Graph >::null_vertex()),color);
	std::cout << "Coloring UB_my_order: " << num_colors2 << std::endl;

	vertices_size_type num_colors3 
							= boost::sequential_vertex_color_ting(G_al, 
								boost::make_iterator_property_map(o.begin(), boost::identity_property_map(),
            					boost::graph_traits< Graph >::null_vertex()),color);
	std::cout << "Coloring UB_ting: " << num_colors3 << std::endl;

	std::cout << std::endl;
	// printGraph(inputGraph, vertex_count);

	std::vector< typename boost::graph_traits< Graph >::vertex_descriptor >
        random_order{order};

	std::srand(std::time(0));

	vertices_size_type min_UB = bvcount;
	vertices_size_type max_UB = 0;

	vertices_size_type num_colors6 = heuristics::Greedy(G_al, my_order);
	std::cout << "Coloring UB_greedy with degree order: " << num_colors6 << std::endl;

	for (int i = 0; i < 1; i++)
	{   
		
		// std::random_shuffle(random_order.begin(), random_order.end());
		// std::cout << "First element: " << random_order[0] << std::endl;

		// vertices_size_type num_colors4 = heuristics::Greedy(G_al, random_order);
		// std::cout << "Coloring UB_greedy: " << num_colors4 << std::endl;
		

		std::random_shuffle(random_order.begin(), random_order.end());
		vertices_size_type num_colors4 
							= boost::sequential_vertex_coloring(G_al, 
								boost::make_iterator_property_map(random_order.begin(), boost::identity_property_map(),
            					boost::graph_traits< Graph >::null_vertex()),color);
		

		min_UB = std::min(min_UB, num_colors4);
		max_UB = std::max(max_UB, num_colors4);

		std::cout << num_colors4 << " " ;
	}
	std::cout << std::endl;
	std::cout << "min_UB: " << min_UB << std::endl;
	std::cout << "max_UB: " << max_UB << std::endl;
*/
	vertices_size_type num_colors8
							= heuristics::dsatur_sequential_vertex_coloring(G_al, 
								boost::make_iterator_property_map(order.begin(), boost::identity_property_map(),
            					boost::graph_traits< Graph >::null_vertex()),color);
	std::cout << "Coloring dsatur: " << num_colors8 << std::endl;
	// printGraph(inputGraph, vertex_count);
	if (build_model == true)
	{
	try
	{
		GRBEnv* env = 0;
		env = new GRBEnv();
		GRBModel model = GRBModel(*env);

		gurobiGcpModel(model, inputGraph);
		model.write("C:\\Users\\surya\\source\\repos\\GCP Gurobi\\out.lp");
		
	}
	catch (...)
	{
		std::cout << "Exception during Solving the model." << std::endl;
	}
	}
	std::cout << "Program ends" << std::endl << std::endl;

}

void gurobiGcpModel(GRBModel &model, surya::Graph const &inputGraph)
{
	//GRBEnv* env = 0;
	GRBVar* colors = 0;
	GRBVar* vertex_color = 0;

	int vertex_count = inputGraph.vertexCount();
	std::vector<Edge> edge_list = inputGraph.undirectedEdgeList();
	
	// lambda func for 2d indexing
	auto x = [&](int i, int j)
	{
		return vertex_color[i*vertex_count + j];
	};

	try
	{
		// Model
		//env = new GRBEnv();
		// GRBModel model = GRBModel(*env);
		model.set(GRB_StringAttr_ModelName, "CP model");

		// Create decision variables
		// Colors
		colors = model.addVars(vertex_count, GRB_BINARY);
		// Vertex-color assignment
		vertex_color = model.addVars(vertex_count*vertex_count, GRB_BINARY);

		// Set objective
		GRBLinExpr obj_fn = 0;
		for (int i = 0; i < vertex_count; i++)
		{
			obj_fn += colors[i];
		}
		model.setObjective(obj_fn, GRB_MINIMIZE);

		// Constraints
		// Adjacency constraints
		Edge edge;
		int node1 = 0, node2 = 0;

		for (int i = 0; i < edge_list.size(); i++)
		{
			
			edge = edge_list[i];
			node1 = edge.src - 1;
			node2 = edge.dest - 1;
			for (int color_j = 0; color_j < vertex_count; color_j++)
			{
				GRBLinExpr edge_constraint = 0;
				edge_constraint += x(node1, color_j) + x(node2, color_j);
				model.addConstr(edge_constraint, GRB_LESS_EQUAL, colors[color_j]);
			}
		}

		// Vertex-Color assignment constraints
		for (int i = 0; i < vertex_count; i++)
		{
			GRBLinExpr ver_col_constraint = 0;
			for (int j = 0; j < vertex_count; j++)
			{
				ver_col_constraint += x(i, j);
			}
			model.addConstr(ver_col_constraint, GRB_EQUAL, 1);
		}

		// Removing symmetry
		// Activation of color
		for (int j = 0; j < vertex_count; j++)
		{
			GRBLinExpr sym_cons = 0;
			for (int i = 0; i < vertex_count; i++)
			{
				sym_cons += x(i, j);
			}
			model.addConstr(colors[j], GRB_LESS_EQUAL, sym_cons);
		}

		// Use new color, only if previous color is used
		for (int j = 1; j < vertex_count-1; j++)
		{
			GRBLinExpr sym_cons = 0;
			model.addConstr(colors[j], GRB_GREATER_EQUAL, colors[j + 1]);
		}

		model.optimize();
		// printSolution(model, colors, vertex_color, vertex_count);
		std::getchar();
	}
	catch (GRBException e)
	{
		std::cout << "Error code = " << e.getErrorCode() << std::endl;
		std::cout << e.getMessage() << std::endl;
	}
	catch (...)
	{
		std::cout << "Exception during modelling" << std::endl;
	}
}

void printSolution(GRBModel& model, GRBVar* colors, GRBVar* vertex_color, int vertex_count)
{
	auto x = [&](int i, int j)
	{
		return vertex_color[i*vertex_count + j];
	};

	if (model.get(GRB_IntAttr_Status) == GRB_OPTIMAL)
	{
		std::cout << "\nCost: " << model.get(GRB_DoubleAttr_ObjVal) << std::endl;
		std::cout << "\nColors:" << std::endl;
		for (int j = 0; j < vertex_count; ++j)
		{
			//if (colors[j].get(GRB_DoubleAttr_X) > 0.0001)
			{
				std::cout << colors[j].get(GRB_StringAttr_VarName) << " " <<
					colors[j].get(GRB_DoubleAttr_X) << std::endl;
			}
		}
		std::cout << "\nVertex-Color assignment:" << std::endl;
		for (int i = 0; i < vertex_count; ++i)
		{
			for (int j = 0; j < vertex_count; ++j)
			std::cout << x(i,j).get(GRB_StringAttr_VarName) << " " <<
				x(i,j).get(GRB_DoubleAttr_X) << std::endl;
		}
	}
	else
	{
		std::cout << "No solution" << std::endl;
	}
	std::getchar();
}


// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
