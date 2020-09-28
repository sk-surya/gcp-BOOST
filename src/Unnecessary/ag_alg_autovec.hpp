#ifndef __AG_ALG_AUTOVEC_H__
#define __AG_ALG_AUTOVEC_H__

#include <omp.h>
#include "bgraph.h"
#include "boost/functional/hash.hpp"
#include <limits>

typedef typename boost::graph_traits<MyGraphAdjListType>::vertices_size_type vertices_size_type;
typedef boost::graph_traits<MyGraphAdjListD>::edge_iterator edge_iteratorD;
typedef boost::graph_traits<MyGraphAdjListD>::out_edge_iterator out_edge_iteratorD;
typedef boost::graph_traits<MyGraphAdjListD>::in_edge_iterator in_edge_iteratorD;
typedef boost::graph_traits<MyGraphAdjListD>::adjacency_iterator adj_iteratorD;
typedef std::pair<vertices_size_type, vertices_size_type> vertex_pair;

    vertices_size_type min_v;
    vertices_size_type max_v;

    edge_iteratorD e_start, e_end; 
    // Construct Line graph of the acyclic orientation
    // A vertex in the line graph represents an edge in the acyclic orientation

void vertex_pair_hasher (const vertices_size_type& v1, 
                                    const vertices_size_type& v2, 
                                    const vertices_size_type& numV,
                                    vertices_size_type& target)
{
    // v1, v2 has to be in range [0, numV-1]
    // our problem specific - v1 has to be in [0, numV-2]
    // we are going to map the locations of
    // upper triangle elems of a matrix excluding
    // leading diag elems --> range [0, numV*(numV-1)/2]
    // beware numV*(numV-1)/2 may exceed max limit of vertices_size_type
    // TODO handle it (will using 'auto' handle it?? I hope so.)

    static constexpr vertices_size_type MAX_V = std::numeric_limits<vertices_size_type>::max();
    static const auto flat_array_size = numV * (numV-1) / 2;
    // static const bool has_enough_size = (flat_array_size <= MAX_V);

    static const bool has_enough_size = [](){
        // runs only in the first function call
        std::cout << std::boolalpha << "Perfect hasher has enough size: " 
                    << (flat_array_size <= MAX_V) 
                    << std::endl;
        if (flat_array_size > MAX_V)
        {   
            std::cout << "Perfect hashing limits exceeded. Terminating Program." << std::endl;
            std::terminate;
        }
        return (flat_array_size <= MAX_V);
    } ();

    // IDEA - lets calculate the flat position
    // of full matrix, then calculate offset in
    // triangular matrix to subtract

    // const auto regular_pos = v1 * numV + v2;
    // const auto offset = (v1 + 1) * (v1 + 2) / 2;
    // target = regular_pos - offset;
    target = v1 * numV + v2 - (v1 + 1) * (v1 + 2) / 2;
}

template <class T>
int numDigits(T number)
{
    int digits = 0;
    if (number < 0) digits = 1; // remove this line if '-' counts as a digit
    while (number) {
        number /= 10;
        digits++;
    }
    return digits;
}

template <class Graph>
bool is_edge(
    const typename boost::graph_traits<Graph>::vertex_descriptor& v,
    const typename boost::graph_traits<Graph>::vertex_descriptor& candidate, 
    const Graph& G)
    {
      return boost::edge(v, candidate, G).second;
    };

MyGraphAdjListD get_acyclic_graph (const vertices_size_type& vertex_count, const MyGraphAdjListType& G_al)
{
    typedef MyGraphAdjListD Graph_D;
    Graph_D G_acyclic (vertex_count);

    typedef boost::graph_traits<MyGraphAdjListD>::edge_iterator edge_iteratorD;
    typedef boost::graph_traits<MyGraphAdjListD>::out_edge_iterator out_edge_iteratorD;
    typedef boost::graph_traits<MyGraphAdjListD>::in_edge_iterator in_edge_iteratorD;
    typedef boost::graph_traits<MyGraphAdjListD>::adjacency_iterator adj_iteratorD;
    typename boost::graph_traits< MyGraphAdjListType >::adjacency_iterator adj, adj_end;
    typedef typename boost::graph_traits< MyGraphAdjListType >::vertex_iterator
        vertex_iterator;

    std::pair< vertex_iterator, vertex_iterator > v = vertices(G_al);
    vertices_size_type min_v;
    vertices_size_type max_v;

    edge_iteratorD e, e_end; 

    std::unordered_set<vertices_size_type> V_all (vertex_count);
    for (size_t i =0; i < vertex_count; i++)
    {
        V_all.insert(V_all.end(), i);
    }

    std::unordered_set<vertices_size_type> V_all_temp (vertex_count);

    for (v = vertices(G_al); v.first != v.second; v.first++)
    {
        // initialise and begin removing
        V_all_temp = V_all;

        vertices_size_type current_vertex = *v.first;
        V_all_temp.erase(current_vertex);
        for (boost::tie(adj, adj_end) = adjacent_vertices(current_vertex, G_al); adj != adj_end; ++adj)
        {
            // std::cout << ' ' << *adj;
            V_all_temp.erase(*adj);

        }
        // std::cout << std::endl;

        for (auto&x : V_all_temp)
        {
            // std::cout << x << " ";

            min_v = std::min(current_vertex, x);
            max_v = std::max(current_vertex, x);
            
            if (is_edge(min_v, max_v, G_acyclic) == false)
            {
                // std::cout << "Added Edge: (" << min_v << ", " << max_v << ")" << std::endl;
                add_edge(min_v, max_v, G_acyclic);
            }
        }   
    }
    return G_acyclic;
}


MyGraphAdjListType get_aux_graph (
            vertices_size_type vertex_count
            , const MyGraphAdjListD& G_acyclic
            )
{   
    // bool line_graph_debug = false;

    MyGraphAdjListType G_line (vertex_count);

    // vertices_size_type chumma1[i], chumma2[i];

    std::cout << "Building Line Graph..." << std::endl;
    std::size_t edge_counter = 0;

    const auto numV = num_vertices(G_acyclic);
    vertices_size_type line_src, line_trg, e_v1, e_v2;
    
    boost::tie(e_start, e_end) = boost::edges(G_acyclic);
    const auto numE = num_edges(G_acyclic);
    auto e = e_start;
    
    // omp_set_num_threads(2);
       
    // for (auto e = e_start; e != e_end; e++)
    //for (auto e : boost::make_iterator_range(edges(G_acyclic)))
    // for (auto e = edges(G_acyclic).first; e != edges(G_acyclic).second; e++)
    std::vector<vertices_size_type> chumma1, chumma2;
    for (int i = 0; i < numE; i++)
    {
        chumma1.push_back(source(*e, G_acyclic));
        chumma2.push_back(target(*e, G_acyclic));
        e++;
    }
    // int isss = 0;
    int i = 0;
    #pragma omp parallel num_threads(2)
    {
    #pragma omp for private(i)
    for (int i = 0; i < numE; i++)
    {
        std::cout << "Thread number: " << omp_get_thread_num() << std::endl;
        edge_counter++;
        //std::cout << source(*e, G_acyclic) << ", " << target(*e, G_acyclic) << std::endl;
        // chumma1[i] = chumma1[i]; //source(*chumma[i], G_acyclic);
        // chumma2[i] = chumma2[i]; //target(*chumma[i], G_acyclic);
        // *current_edge = std::make_pair(chumma1[i], chumma2[i]);
        vertex_pair_hasher(chumma1[i], chumma2[i], numV, line_src); // edge to vertex mapper
        
        // find adjacent edges of current edge
        // ie, (incident edges of chumma1[i] + incident edges of chumma2[i])

        out_edge_iteratorD oe, oe_end;
        in_edge_iteratorD ie, ie_end;
        const std::vector<vertices_size_type> edge_vec {chumma1[i], chumma2[i]};

        // if (line_graph_debug) {std::cout 
                // << "Source in line graph: " << "v" 
                // << chumma1[i] 
                // << ", v" 
                // << chumma2[i] << std::endl;}
            
            auto current_vertex = edge_vec[0];
            // if (line_graph_debug) std::cout << "Out vertices of vertex " << current_vertex << " : " << std::endl;
            for (boost::tie(oe, oe_end) = boost::out_edges(current_vertex, G_acyclic); oe != oe_end; oe++)
            {
                // if (line_graph_debug) std::cout << target(*oe, G_acyclic) << " ";
                e_v1 = source(*oe, G_acyclic);
                e_v2 = target(*oe, G_acyclic);

                if (not(e_v1 == chumma1[i] and e_v2 == chumma2[i])) // when equal, this entry is not needed (also it'll be duplicated)
                {
                    // if (line_graph_debug) std::cout << e_v1 << ", " << e_v2 << " ";
                    vertex_pair_hasher(e_v1, e_v2, numV, line_trg);
                    if (not(is_edge(line_src, line_trg, G_line)))
                    add_edge(line_src, line_trg, G_line);
                }
            }
            // if (line_graph_debug) std::cout << std::endl;

            // if (line_graph_debug) std::cout << "In vertices of vertex " << current_vertex << " : " << std::endl;
            for (boost::tie(ie, ie_end) = boost::in_edges(current_vertex, G_acyclic); ie != ie_end; ie++)
            {
                // if (line_graph_debug) std::cout << source(*ie, G_acyclic) << " ";
                e_v1 = source(*ie, G_acyclic);
                e_v2 = target(*ie, G_acyclic);

                if (not(e_v1 == chumma1[i] and e_v2 == chumma2[i])) // when equal, this entry is not needed (also it'll be duplicated)
                {
                    // if (line_graph_debug) std::cout << e_v1 << ", " << e_v2 << " ";
                    vertex_pair_hasher(e_v1, e_v2, numV, line_trg);
                    if (not(is_edge(line_src, line_trg, G_line)))
                    add_edge(line_src, line_trg, G_line);
                }
            }

            current_vertex = edge_vec[1];
            // if (line_graph_debug) std::cout << "Out vertices of vertex " << current_vertex << " : " << std::endl;
            for (boost::tie(oe, oe_end) = boost::out_edges(current_vertex, G_acyclic); oe != oe_end; oe++)
            {
                // if (line_graph_debug) std::cout << target(*oe, G_acyclic) << " ";
                e_v1 = source(*oe, G_acyclic);
                e_v2 = target(*oe, G_acyclic);

                if (not(e_v1 == chumma1[i] and e_v2 == chumma2[i])) // when equal, this entry is not needed (also it'll be duplicated)
                {
                    // if (line_graph_debug) std::cout << e_v1 << ", " << e_v2 << " ";
                    vertex_pair_hasher(e_v1, e_v2, numV, line_trg);
                    if (not(is_edge(line_src, line_trg, G_line)))
                    add_edge(line_src, line_trg, G_line);
                }
            }
            // if (line_graph_debug) std::cout << std::endl;

            // if (line_graph_debug) std::cout << "In vertices of vertex " << current_vertex << " : " << std::endl;
            for (boost::tie(ie, ie_end) = boost::in_edges(current_vertex, G_acyclic); ie != ie_end; ie++)
            {
                // if (line_graph_debug) std::cout << source(*ie, G_acyclic) << " ";
                e_v1 = source(*ie, G_acyclic);
                e_v2 = target(*ie, G_acyclic);

                if (not(e_v1 == chumma1[i] and e_v2 == chumma2[i])) // when equal, this entry is not needed (also it'll be duplicated)
                {
                    // if (line_graph_debug) std::cout << e_v1 << ", " << e_v2 << " ";
                    vertex_pair_hasher(e_v1, e_v2, numV, line_trg);
                    if (not(is_edge(line_src, line_trg, G_line)))
                    add_edge(line_src, line_trg, G_line);
                }
            }
            // if (line_graph_debug) std::cout << std::endl;
        
        if (edge_counter % 5000 == 0) std::cout << "Edge count: " << edge_counter << "/" << num_edges(G_acyclic) << std::endl;
        //e++;
    }
    }
    std::cout << "Line Graph ready" << std::endl;
    return G_line;
}

#endif // __AG_ALG_AUTOVEC_H__