//=======================================================================
// Copyright 1997, 1998, 1999, 2000 University of Notre Dame.
// Copyright 2004 The Trustees of Indiana University
// Authors: Andrew Lumsdaine, Lie-Quan Lee, Jeremy G. Siek
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//=======================================================================
#ifndef DSATUR_BOOST_HPP
#define DSATUR_BOOST_HPP

#include <boost/graph/graph_traits.hpp>
#include <boost/tuple/tuple.hpp>
#include <boost/property_map/property_map.hpp>
#include <boost/limits.hpp>
#include <unordered_set>
#include <algorithm>
#include <numeric>

#ifdef BOOST_NO_TEMPLATED_ITERATOR_CONSTRUCTORS
#include <iterator>
#endif

/* This algorithm is to find coloring of a graph

   Algorithm:
   Let G = (V,E) be a graph with vertices (somehow) ordered v_1, v_2, ...,
   v_n. For k = 1, 2, ..., n the sequential algorithm assigns v_k to the
   smallest possible color.

   Reference:

   Thomas F. Coleman and Jorge J. More, Estimation of sparse Jacobian
   matrices and graph coloring problems. J. Numer. Anal. V20, P187-209, 1983

   v_k is stored as o[k] here.

   The color of the vertex v will be stored in color[v].
   i.e., vertex v belongs to coloring color[v] */
using namespace boost;
namespace heuristics
{
template < class VertexListGraph, class OrderPA, class ColorMap >
typename boost::property_traits< ColorMap >::value_type dsatur_sequential_vertex_coloring(
    const VertexListGraph& G, OrderPA order, ColorMap color)
{
    typedef graph_traits< VertexListGraph > GraphTraits;
    typedef typename GraphTraits::vertex_descriptor Vertex;
    typedef typename property_traits< ColorMap >::value_type size_type;
    typename boost::graph_traits< VertexListGraph >::adjacency_iterator adj, adj_end;

    size_type max_color = 0;
    const size_type V = num_vertices(G);

    // We need to keep track of which colors are used by
    // adjacent vertices. We do this by marking the colors
    // that are used. The mark array contains the mark
    // for each color. The length of mark is the
    // number of vertices since the maximum possible number of colors
    // is the number of vertices.
    std::vector< size_type > mark(V,
        std::numeric_limits< size_type >::max
            BOOST_PREVENT_MACRO_SUBSTITUTION());

    // Initialize colors
    typename GraphTraits::vertex_iterator v, vend;
    for (boost::tie(v, vend) = vertices(G); v != vend; ++v)
        put(color, *v, V - 1);

    // DSATUR mod begins
    
    std::vector<size_type> degrees(V);
	for (size_type i=0; i < V; i++)
	{
		degrees[i] = out_degree(i, G);
		// std::cout << out_degree(order[i], G_al) << " " << degrees[i] << std::endl;
	}

    std::vector< typename boost::graph_traits< VertexListGraph >::vertex_descriptor >
        largest_degree_order(V);
    std::iota(largest_degree_order.begin(), largest_degree_order.end(), 0);

	std::sort(
		largest_degree_order.begin(), largest_degree_order.end(),
		[&G, &degrees](auto const &a, auto const &b) {return 
                                                degrees[a] > degrees[b]
                                          ;}
	);

    std::unordered_set<size_type> uncolored_vertices(largest_degree_order.begin(), largest_degree_order.end());
    std::vector<std::unordered_set<size_type>> distinct_neighbor_colors(V);

    size_type vertex_order_locator = 0;

    // Take the vertex with maximal degree and
    // color it. Update distinct_neighbor_colors and
    // increment max_color count
    Vertex current = largest_degree_order[0];
    put(color, current, max_color);
    for (boost::tie(adj, adj_end) = boost::adjacent_vertices(current, G); adj != adj_end;
         ++adj)
        distinct_neighbor_colors[*adj].insert(max_color);
    
    uncolored_vertices.erase(current);
    max_color += 1;

    
    // saturation for a vertex holds the count of
    // distinct colors of its neighbors
    std::vector<size_type> saturation(V);
    for (size_type i=0; i < V; i++)
    {
        saturation[i] = distinct_neighbor_colors[i].size();
    }

    // now lets find an uncolored vertex with maximum saturation.
    // TODO : in case of tie, we'll choose the vertex with maximum degree.
    while (!uncolored_vertices.empty())
    {
        // std::cout << "left: " << uncolored_vertices.size() << " last vertex colored: " << current << std::endl;
        size_type max_sat = 0;
        size_type max_sat_v = 0;
        for (auto& i : uncolored_vertices)
        {
            if (saturation[i] > max_sat)
            {
                max_sat = saturation[i];
                max_sat_v = i;
            }
        }

        // tie-breaking
        std::vector<size_type> max_sat_candidates;
        for (auto& i : uncolored_vertices)
        {
            if (saturation[i] == max_sat)
            {
                max_sat_candidates.push_back(i);
            }
        }

        size_type max_sat_deg = 0;
        size_type max_sat_deg_v = 0;
        for (auto& c : max_sat_candidates)
        {
            if (degrees[c] > max_sat_deg)
            {
                max_sat_deg = degrees[c];
                max_sat_deg_v = c;
            }
        }
        current = max_sat_deg_v;

    // Determine the color for every vertex one by one
    // for (size_type i = 0; i < V; i++)
    // {
    //     Vertex current = get(order, i);
        // Vertex current = my_order[i];
        typename GraphTraits::adjacency_iterator v, vend;

        // Mark the colors of vertices adjacent to current.
        // i can be the value for marking since i increases successively
        for (boost::tie(v, vend) = adjacent_vertices(current, G); v != vend;
             ++v)
            mark[get(color, *v)] = current;

        // Next step is to assign the smallest un-marked color
        // to the current vertex.
        size_type j = 0;

        // Scan through all useable colors, find the smallest possible
        // color that is not used by neighbors.  Note that if mark[j]
        // is equal to i, color j is used by one of the current vertex's
        // neighbors.
        while (j < max_color && mark[j] == current)
            ++j;

        if (j == max_color) // All colors are used up. Add one more color
            ++max_color;

        // At this point, j is the smallest possible color
        put(color, current, j); // Save the color of vertex current
        for (boost::tie(adj, adj_end) = boost::adjacent_vertices(current, G); adj != adj_end;
         ++adj)
            distinct_neighbor_colors[*adj].insert(j);
            for (size_type i=0; i < V; i++)
            {
                saturation[i] = distinct_neighbor_colors[i].size();
            }
        
        uncolored_vertices.erase(current);

    }

    return max_color;
}

template < class VertexListGraph, class ColorMap >
typename boost::property_traits< ColorMap >::value_type dsatur_sequential_vertex_coloring(
    const VertexListGraph& G, ColorMap color)
{
    typedef typename graph_traits< VertexListGraph >::vertex_descriptor
        vertex_descriptor;
    typedef typename graph_traits< VertexListGraph >::vertex_iterator
        vertex_iterator;

    std::pair< vertex_iterator, vertex_iterator > v = vertices(G);
#ifndef BOOST_NO_TEMPLATED_ITERATOR_CONSTRUCTORS
    std::vector< vertex_descriptor > order(v.first, v.second);
#else
    std::vector< vertex_descriptor > order;
    order.reserve(std::distance(v.first, v.second));
    while (v.first != v.second)
        order.push_back(*v.first++);
#endif
    return dsatur_sequential_vertex_coloring(G,
        make_iterator_property_map(order.begin(), identity_property_map(),
            graph_traits< VertexListGraph >::null_vertex()),
        color);
}
}

namespace verifier
{   
    template <class Graph, class ColorMap>
    bool verify_k_color (const Graph& G, ColorMap& color)
    {
        typedef typename boost::graph_traits< Graph >::vertex_iterator vertex_iterator;
        typedef typename boost::graph_traits<Graph>::edge_iterator edge_iterator;

        vertex_iterator vi, vend;
        edge_iterator ei, ei_end;

        for (boost::tie(ei, ei_end) = edges(G); ei != ei_end;
             ++ei)
        {
            // std::cout << source(*ei, G) << " " << target(*ei, G) << std::endl;
            if (get(color, source(*ei, G)) == get(color, target(*ei, G)))
            {
                return false;
            }
        }
        return true;

    }
}

#endif
