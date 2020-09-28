// Header for graph coloring heuristics
// Plan - to have DSATUR & RLF heuristics

#pragma once
#ifndef _HEUGCOL_H
#define _HEUGCOL_H

#include "boost/graph/adjacency_list.hpp"
#include <limits>
#include <boost/limits.hpp>
#include "boost/graph/visitors.hpp"
#include <functional>

template <class Graph, class Order>
typename boost::graph_traits<Graph>::vertices_size_type
Greedy(const Graph& G, const Order& order);

template <class VertexListGraph, class Order, class Color>
typename boost::graph_traits<VertexListGraph>::vertices_size_type
sequential_vertex_color_ting(const VertexListGraph& G,
  Order order, Color color);
  
#endif