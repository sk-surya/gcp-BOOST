#pragma once
#ifndef _AUX_GRAPH_ALGEBRA_HPP
#define _AUX_GRAPH_ALGEBRA_HPP

#include "bgraph.h"
#include "boost/functional/hash.hpp"
#include <limits>
#include <boost/graph/labeled_graph.hpp>

typedef typename boost::graph_traits<MyGraphAdjListType>::vertices_size_type vertices_size_type;
typedef boost::graph_traits<MyGraphAdjListD>::edge_iterator edge_iteratorD;
typedef boost::graph_traits<MyGraphAdjListD>::out_edge_iterator out_edge_iteratorD;
typedef boost::graph_traits<MyGraphAdjListD>::in_edge_iterator in_edge_iteratorD;
typedef boost::graph_traits<MyGraphAdjListD>::adjacency_iterator adj_iteratorD;
typedef std::pair<vertices_size_type, vertices_size_type> vertex_pair;

    // Construct Line graph of the acyclic orientation
    // A vertex in the line graph represents an edge in the acyclic orientation

template <class Graph>
void vertex_pair_hasher (const vertices_size_type& v1, 
                                    const vertices_size_type& v2, 
                                    const vertices_size_type& numV,
                                    vertices_size_type& target);
template <class T>
int numDigits(T number);

template <class Graph>
bool is_edge(
    const typename boost::graph_traits<Graph>::vertex_descriptor& v,
    const typename boost::graph_traits<Graph>::vertex_descriptor& candidate, 
    Graph& G);

// typedef boost::labeled_graph<Graph, vertices_size_type, boost::hash_mapS> LabeledGraph;
typedef vertices_size_type Label;

template <>
bool is_edge<boost::labeled_graph<MyGraphAdjListType, vertices_size_type, boost::hash_mapS>>(
    const Label& v,
    const Label& candidate, 
    //LabeledGraph& G)
    typename boost::labeled_graph<MyGraphAdjListType, vertices_size_type, boost::hash_mapS>& G);    

template <>
bool is_edge<boost::labeled_graph<MyGraphAdjMatType, vertices_size_type, boost::hash_mapS>>(
    const Label& v,
    const Label& candidate, 
    //LabeledGraph& G)
    typename boost::labeled_graph<MyGraphAdjMatType, vertices_size_type, boost::hash_mapS>& G);        

auto get_acyclic_graph (const vertices_size_type& vertex_count, const MyGraphAdjListType& G_al);

template<class Graph>
boost::labeled_graph<Graph, vertices_size_type, boost::hash_mapS> get_aux_graph (
            const MyGraphAdjListD& G_acyclic
            );

#endif    