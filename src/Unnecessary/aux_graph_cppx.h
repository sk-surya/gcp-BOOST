#ifndef _AUX_GRAPH_CPPX_H
#define _AUX_GRAPH_CPPX_H

#include "bgraph.h"
#include "cppx_decimal_from.h"

using cppx::decimal_from_unsigned;
using cppx::ulong_bufsize;

char spec[ulong_bufsize];

typedef typename boost::graph_traits<MyGraphAdjListType>::vertices_size_type vertices_size_type;
typedef boost::graph_traits<MyGraphAdjListD>::edge_iterator edge_iteratorD;
typedef boost::graph_traits<MyGraphAdjListD>::out_edge_iterator out_edge_iteratorD;
typedef boost::graph_traits<MyGraphAdjListD>::in_edge_iterator in_edge_iteratorD;
typedef boost::graph_traits<MyGraphAdjListD>::adjacency_iterator adj_iteratorD;
    
    vertices_size_type min_v;
    vertices_size_type max_v;

    edge_iteratorD e, e_end; 
    // Construct Line graph of the acyclic orientation
    // A vertex in the line graph represents an edge in the acyclic orientation

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

std::unordered_map<std::string, vertices_size_type> get_LG_idx_map (const MyGraphAdjListD& G_acyclic)
{
    std::unordered_map<std::string, vertices_size_type> line_graph_index_map (boost::num_edges(G_acyclic));
    vertices_size_type src, trg;
    vertices_size_type val = 0;

    // std::string src_str, trg_str;

    const auto numD = numDigits(num_vertices(G_acyclic));
    
    std::string ks;
    
    for (boost::tie(e, e_end) = boost::edges(G_acyclic); e != e_end; e++)
    {
        //std::cout << source(*e, G_acyclic) << ", " << target(*e, G_acyclic) << std::endl;
        src = source(*e, G_acyclic);
        trg = target(*e, G_acyclic);

        char key_string[2*numD + 2];

        strcpy(key_string, "v");
        strcat(key_string, decimal_from_unsigned(src, spec));
        strcat(key_string, "v");
        strcat(key_string, decimal_from_unsigned(trg, spec));
       
        std::string ks (key_string);
        line_graph_index_map[ks] = val;
        val++;
    }
    std::cout << "Line Graph index map ready" << std::endl;
    if (val > 10)
    {
        val++;
        // nothing
    }
    std::cout << val << std::endl;
    return line_graph_index_map;
}


MyGraphAdjListType get_aux_graph (
            vertices_size_type vertex_count
            , const MyGraphAdjListD& G_acyclic
            , const std::unordered_map<std::string, vertices_size_type>& line_graph_index_map)
{   
    bool line_graph_debug = false;

    MyGraphAdjListType G_line (vertex_count);

    // lets create a mapping for vertex names and vertex indices
    // for example "v0v2" --> 0

    vertices_size_type src, trg;

    std::cout << "Building Line Graph..." << std::endl;
    std::size_t edge_counter = 0;

    std::vector<std::string> vertex_strings;
    vertices_size_type max_degree = 0;
    for (auto v = vertices(G_acyclic); v.first != v.second; v.first++)
        max_degree = std::max(max_degree, degree(*v.first, G_acyclic));
    size_t vertex_strings_size_estimate = 2*max_degree;
    std::cout << "Max degree of G_acyclic: " << max_degree << std::endl;
    vertex_strings.reserve(vertex_strings_size_estimate);

    const auto numD = numDigits(num_vertices(G_acyclic));
    char keystring[2*numD + 2];
    char source_key_string[2*numD + 2];
    

    std::string ks, sks;

    for (boost::tie(e, e_end) = boost::edges(G_acyclic); e != e_end; e++)
    {
        edge_counter++;
        //std::cout << source(*e, G_acyclic) << ", " << target(*e, G_acyclic) << std::endl;
        src = source(*e, G_acyclic);
        trg = target(*e, G_acyclic);

        strcpy(source_key_string, "v");
        strcat(source_key_string, decimal_from_unsigned(src, spec));
        strcat(source_key_string, "v");
        strcat(source_key_string, decimal_from_unsigned(trg, spec));
        sks = source_key_string;
        auto line_src = line_graph_index_map.at(sks);

        // find adjacent edges of current edge
        // ie, (incident edges of src + incident edges of trg)

        out_edge_iteratorD oe, oe_end;
        in_edge_iteratorD ie, ie_end;
        std::vector<vertices_size_type> edge_vec {src, trg};

        if (line_graph_debug) std::cout << "Source in line graph: " << sks << std::endl;

        vertex_strings.clear();
        for (auto& current_vertex : edge_vec)
        {
            if (line_graph_debug) std::cout << "Out vertices of vertex " << current_vertex << " : " << std::endl;
            for (boost::tie(oe, oe_end) = boost::out_edges(current_vertex, G_acyclic); oe != oe_end; oe++)
            {
                if (line_graph_debug) std::cout << target(*oe, G_acyclic) << " ";

                strcpy(keystring, "v");
                strcat(keystring, decimal_from_unsigned(source(*oe, G_acyclic), spec));
                strcat(keystring, "v");
                strcat(keystring, decimal_from_unsigned(target(*oe, G_acyclic), spec));
                ks = keystring;
                if (strcmp(keystring, source_key_string) != 0) // when equal, this entry is not needed (also it'll be duplicated)
                    vertex_strings.push_back(ks);
            }
            if (line_graph_debug) std::cout << std::endl;

            if (line_graph_debug) std::cout << "In vertices of vertex " << current_vertex << " : " << std::endl;
            for (boost::tie(ie, ie_end) = boost::in_edges(current_vertex, G_acyclic); ie != ie_end; ie++)
            {
                if (line_graph_debug) std::cout << source(*ie, G_acyclic) << " ";

                strcpy(keystring, "v");
                strcat(keystring, decimal_from_unsigned(source(*ie, G_acyclic), spec));
                strcat(keystring, "v");
                strcat(keystring, decimal_from_unsigned(target(*ie, G_acyclic), spec));
                ks = keystring;
                if (strcmp(keystring, source_key_string) != 0) // when equal, this entry is not needed (also it'll be duplicated)
                    vertex_strings.push_back(ks);
            }
            if (line_graph_debug) std::cout << std::endl;
        }
        if (edge_counter % 5000 == 0) std::cout << "Edge count: " << edge_counter << "/" << num_edges(G_acyclic) << std::endl;
        for (auto& key_string : vertex_strings)
        {   
            // std::unordered_set<std::string> set_strings(vertex_strings.begin(), vertex_strings.end());
                
            // assert(set_strings.size() == vertex_strings.size());

            if (line_graph_debug) std::cout << key_string << " ";
            auto line_trg = line_graph_index_map.at(key_string);
            if (not(is_edge(line_src, line_trg, G_line)))
            add_edge(line_src, line_trg, G_line);
        }
        if (line_graph_debug) std::cout << std::endl;
        // cout << "actual size: " << vertex_strings.size() << endl;
        assert (vertex_strings_size_estimate >= vertex_strings.size());
    }
    std::cout << "Line Graph ready" << std::endl;
    return G_line;
}


#endif    