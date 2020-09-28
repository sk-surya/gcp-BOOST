// #include <mcheck.h>
#include "pch.h"

#include "prep.h"
#include "heugcol.h"
#include "bgraph.h"
#include <algorithm>
#include "dsatur_boost.hpp"
#include "boost/compute/algorithm/min_element.hpp"
#include "boost/graph/sequential_vertex_coloring.hpp"
#include <boost/filesystem.hpp>
// #include "aux_graph_karma_cstr.h"
// #include "aux_graph_int_map.hpp"
// #include "boost/functional/hash.hpp"
#include "aux_graph_algebra.hpp"
// #include "ag_alg_autovec.hpp"
// #include "aux_graph_fmt.h"
// #include "aux_graph_ltoa.h"
// #include "aux_graph_old.h"
// #include "aux_graph_std_res.h"
// #include "aux_graph_cppx.h"
#include <boost/graph/graphml.hpp>
#include <boost/graph/max_cardinality_matching.hpp>

using namespace std;

int main(int argc, char** argv)
{	
    if(argc==1) 
    {
        printf("\nNo Extra Command Line Argument Passed Other Than Program Name"); 
        printf("\nExiting.");
        printf("\n");
        return 1;
    }
    // mtrace();
    
    std::string filename {argv[1]};
    std::cout << "File: " << filename << std::endl;
	// bool build_model{false};
	// std::cout << "Surya" << std::endl;
	std::string filepath = "/home/surya/orstuff/instances/" + filename;
    boost::filesystem::path p (filepath);
    // cout << std::boolalpha << boost::filesystem::exists(p) << "\n";
    
    if (boost::filesystem::exists(p))    // does p actually exist?
  {
    if (is_regular_file(p))        // is p a regular file?   
      cout << p << " size is " << file_size(p) << '\n';

    else if (is_directory(p))      // is p a directory?
      cout << p << "is a directory\n";

    else
      cout << p << "exists, but is neither a regular file nor a directory\n";
  }
  else
  {
    cout << p << "does not exist\n";
    cout << "Exiting.\n";
    return 1;
  }

	int vertex_count = 0; // will be filled by readInput
	std::vector<Surya::Edge> edges = Surya::readInput(filepath, vertex_count);
	// std::size_t edge_count = edges.size();
  // Surya::Graph Gin(edges, vertex_count, true);
	// std::cout << "Vertex count: " << vertex_count << std::endl;
	// std::cout << "Input Edge count: " << edge_count << std::endl;

	typedef MyGraphAdjListType Graph;
    typedef graph_traits< Graph > GraphTraits;

	Graph G_al(vertex_count);
	
	// add edges to the boost graph from our edge list (our edge list from DIMACS is assumed to be 1-indexed)
	for (const auto& i : edges)
	{
		// std::cout << i.src << " "  << i.dest << std::endl;
		boost::add_edge(boost::vertex(i.src - 1, G_al), boost::vertex(i.dest - 1, G_al), G_al);
	};

	std::cout << "Number of vertices in boost graph: " << boost::num_vertices(G_al) << std::endl;
	std::cout << "Number of edges in boost graph: " << boost::num_edges(G_al) << std::endl;

	typename boost::graph_traits< Graph >::adjacency_iterator adj, adj_end;
	
	typedef typename boost::graph_traits<Graph>::vertex_descriptor vertex_descriptor;
	typedef typename boost::graph_traits<Graph>::vertices_size_type vertices_size_type;
	typedef typename boost::property_map<Graph, boost::vertex_index_t>::const_type vertex_index_map;

	std::vector<vertices_size_type> color_vec(num_vertices(G_al));
	boost::iterator_property_map<vertices_size_type*, vertex_index_map>
		color(&color_vec.front(), get(boost::vertex_index, G_al));

    typedef typename boost::graph_traits< Graph >::vertex_iterator
        vertex_iterator;

    std::pair< vertex_iterator, vertex_iterator > v = vertices(G_al);

    std::vector< vertex_descriptor > order(v.first, v.second);

    // vertex_iterator vb, vend;

    // for (int i = 0; i < 1; i++)
    // {   
        vertices_size_type num_colors8
                                = heuristics::dsatur_sequential_vertex_coloring(G_al, 
                                    boost::make_iterator_property_map(order.begin(), boost::identity_property_map(),
                                    boost::graph_traits< Graph >::null_vertex()),color);
        std::cout << "Coloring dsatur: " << num_colors8 << std::endl;
        std::cout << std::boolalpha << verifier::verify_k_color(G_al, color) << std::endl;

    //     /*
    //     // print coloring
    //     for (boost::tie(vb, vend) = vertices(G_al); vb != vend;
    //          ++vb)
    //         std::cout << get(color, *vb) << " ";
    //     std::cout << std::endl;
    //     */

    //    /*
    //    // boost greedy coloring
    //     vertices_size_type num_colors9
    //                             = boost::sequential_vertex_coloring(G_al, 
    //                                 boost::make_iterator_property_map(order.begin(), boost::identity_property_map(),
    //                                 boost::graph_traits< Graph >::null_vertex()),color);
    //     std::cout << "Coloring boost: " << num_colors9 << std::endl;
    //     */

    //     // std::cout << std::boolalpha << verifier::verify_k_color(G_al, color) << std::endl;
        
    // }
 
 
    // auxiliary graph
    // acyclic orientation

    typedef MyGraphAdjListD Graph_D;
    Graph_D G_acyclic = get_acyclic_graph(vertex_count, G_al);
    
    std::cout << std::endl;
    std::cout << "Number of vertices in acyclic orientation of G: " << num_vertices(G_acyclic) << std::endl;
    std::cout << "Number of edges in acyclic orientation of G: " << num_edges(G_acyclic) << std::endl;
    // print G_acyclic
    /*
    for (boost::tie(e, e_end) = boost::edges(G_acyclic); e != e_end; e++)
    {
        std::cout << source(*e, G_acyclic) << ", " << target(*e, G_acyclic) << std::endl;
    }
    */
    typedef std::pair<vertices_size_type, vertices_size_type> vertex_pair;
    // std::unordered_map<vertex_pair, vertices_size_type, boost::hash<vertex_pair>> line_graph_index_map = get_LG_idx_map(G_acyclic);
    // std::unordered_map<std::string, vertices_size_type> line_graph_index_map = get_LG_idx_map(G_acyclic);
    // MyGraphAdjListType G_line = get_aux_graph<MyGraphAdjListType>(vertex_count, G_acyclic, line_graph_index_map);
    typedef MyGraphAdjListType myLineGraph;
    typedef boost::labeled_graph<myLineGraph, vertices_size_type, hash_mapS>  Labeled_Graph;
    auto G_line = get_aux_graph<myLineGraph>(G_acyclic);
    std::cout << "Number of vertices in G_line: " << num_vertices(G_line) << std::endl;
    std::cout << "Number of edges in G_line: " << num_edges(G_line) << std::endl;

    // creating adjacency matrix graph (tried initialising G_line as matrix, unsuccesful)
    // MyGraphAdjMatType mg( num_vertices(G_line));
    // boost::copy_graph(G_line.graph(), mg);
    // MatrixAccessor<MyGraphAdjMatType> matr(&mg);

    // for (auto v = vertices(mg); v.first != v.second; ++v.first)
    // {
    //     std::cout << *v.first << std::endl;
    // }

    
    // std::cout << sizeof(vertices_size_type) << std::endl;
    // std::cout << std::numeric_limits<vertices_size_type>::max() << std::endl;

    // Step 3 : Find and remove all edges between pair of arcs
    // which are simplicial in the line graph
    // ie, In the acyclic graph, 
    // given a = (v_i, v_j) and b = (v_i, v_k)
    // and edge (v_j, v_k) or (v_k, v_j) exists 
    // then (a, b) is a simplicial pair of arcs
    // edge (a,b) can be removed from the line graph

    // O(|V||E|) algorithm to find triangles
    bool bDetectTriangles = true;
    if (bDetectTriangles == true)
    {
        typedef boost::graph_traits<Graph_D>::vertex_iterator vertex_iteratorD;
        vertex_iteratorD w, w_end;
        size_t simplicial_counter = 0;
        
        std::cout << "Begin Triangle detection..." << std::endl;
        vertices_size_type lg_v1, lg_v2;
        auto numV = num_vertices(G_al); // mapping from G_acyclic to G_line, hence
        for (boost::tie(e, e_end) = boost::edges(G_acyclic); e != e_end; e++)
        {
            //std::cout << source(*e, G_acyclic) << ", " << target(*e, G_acyclic) << std::endl;
            auto u = source(*e, G_acyclic);
            auto v = target(*e, G_acyclic);
            
            for (boost::tie(w, w_end) = vertices(G_acyclic); w != w_end; w++)
            {
                // auto w1 = *w;
                
                if (is_edge(*w, u, G_acyclic) and is_edge(*w, v, G_acyclic))
                {
                    // (w, u) and (w, v) is the simplicial pair
                    // auto lg_v1_string = "v" + std::to_string(*w) + "v" + std::to_string(u);
                    // auto lg_v2_string = "v" + std::to_string(*w) + "v" + std::to_string(v);
                    vertex_pair_hasher(*w, u, numV, lg_v1);
                    vertex_pair_hasher(*w, v, numV, lg_v2);
                    // auto lg_v1 = line_graph_index_map[lg_v1_string];
                    // auto lg_v2 = line_graph_index_map[lg_v2_string];
                    // std::cout << w1 << ": " << u << v << std::endl;
                    remove_edge_by_label(lg_v1, lg_v2, G_line);
                    simplicial_counter++;
                }
            }
        }

        vertices_size_type max_degree = 0, min_degree = num_vertices(G_line);
        std::vector<vertices_size_type> degrees_LG(num_vertices(G_line));
        for (auto v = vertices(G_line); v.first != v.second; v.first++)
        {
            degrees_LG.push_back(out_degree(*v.first, G_line));
            max_degree = std::max(max_degree, out_degree(*v.first, G_line));  
            min_degree = std::min(max_degree, out_degree(*v.first, G_line));  
        }
        std::sort(degrees_LG.begin(), degrees_LG.end(), std::greater<typename decltype(degrees_LG)::value_type>());
/*         vertices_size_type dg = num_vertices(G_line);
        for(auto& d : degrees_LG)
        {
          assert(dg >= d);
          dg = d;
        }
        std::cout << "Sorted into Descending order." << std::endl; */
        
        // Analytical Upper Bounds on MIS
        // Hansen Upper Bound
        auto hansen_UB = floor(0.5 + sqrt(0.25 + pow(num_vertices(G_line), 2) - num_vertices(G_line) - 2*num_edges(G_line)));
        // Minimum Degree Bound
        auto min_deg_UB = num_vertices(G_line) - min_degree;
        // Kwok Bound
        auto kwok_UB = num_vertices(G_line) - num_edges(G_line)/max_degree;
        // Borg Bound
        auto borg_UB = floor(((max_degree - 1)*num_vertices(G_line) + 1) / max_degree);
        // Annihilation number Bound
        long long int sum_so_far = 0, sum_remaining = 0;
        const long long int sum_total = std::accumulate(degrees_LG.begin(), degrees_LG.end(), 0); // calculates sum
        vertices_size_type k = 0;
        for (auto& d : degrees_LG)
        {
          k++;
          sum_so_far += d;
          sum_remaining = sum_total - sum_so_far;
          if (sum_so_far > sum_remaining)
          {
            k--;
            break;
          }
        }
        auto annihilation_UB = k;
        std::cout << std::accumulate(degrees_LG.begin(), degrees_LG.begin()+annihilation_UB-1, 0) << std::endl;
        std::cout << sum_total  << std::endl;
        std::cout << sum_so_far << " " << sum_remaining << std::endl;

        // https://math.stackexchange.com/questions/3314227/best-known-upper-bound-for-the-size-of-the-maximum-independent-set-in-a-given-gr
        auto so_UB = (0.5) * (1 + sqrt(pow(2*num_vertices(G_line) - 1, 2) - 8*num_edges(G_line)));

        // Matching number bound
        std::vector<graph_traits<Labeled_Graph>::vertex_descriptor> mate(num_vertices(G_line));

        bool success = checked_edmonds_maximum_cardinality_matching(G_line, &mate[0]);
        assert(success);
        auto matching_num = matching_size(G_line, &mate[0]);
        auto matching_UB = num_vertices(G_line) - matching_num;
        std::cout << std::endl << "Found a matching of size " << matching_num << std::endl << std::endl;

        std::cout << "Auxiliary graph ready." << std::endl << std::endl;
        std::cout << "Number of simplicial pairs removed: " << simplicial_counter << std::endl;
        std::cout << "Number of vertices in the auxiliary graph: " << num_vertices(G_line) << std::endl;
        std::cout << "Number of edges in the auxiliary graph: " << num_edges(G_line) << std::endl;
        std::cout << "Max degree of auxiliary graph: " << max_degree << std::endl;
        std::cout << "Min degree of auxiliary graph: " << min_degree << std::endl << std::endl;
        std::cout << "Analytical Upper Bounds on MIS for auxiliary graph: " << std::endl;
        std::cout << "*************************************************** " << std::endl;
        std::cout << "Trivial UB       \t\t: " << num_vertices(G_line) << std::endl;
        std::cout << "Hansen UB        \t\t: " << hansen_UB << std::endl;
        std::cout << "Minimum Degree UB\t\t: " << min_deg_UB << std::endl;
        std::cout << "Kwok UB          \t\t: " << kwok_UB << std::endl;
        std::cout << "Borg UB          \t\t: " << borg_UB << std::endl;
        std::cout << "Annihilation UB  \t\t: " << annihilation_UB << std::endl;
        std::cout << "Unknown UB       \t\t: " << so_UB << std::endl;
        std::cout << "Matching Num UB  \t\t: " << matching_UB << std::endl;
         

        // ofstream aux_g_file;
        // aux_g_file.open ("/home/surya/orstuff/instances/gml/" + filename + ".auxg.graphml");
        // dynamic_properties dp;
        // write_graphml(aux_g_file, G_line, dp, true);
        // aux_g_file.close();
    }

    std::cout << std::endl;
    return 0;
}