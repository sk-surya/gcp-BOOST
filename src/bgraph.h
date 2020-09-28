#pragma once
#ifndef _BGRAPH_H
#define _BGRAPH_H

#include "boost/graph/adjacency_list.hpp"
#include "boost/graph/adjacency_matrix.hpp"
#include <boost/graph/copy.hpp>

struct VertexData
{
  std::string first_name;
  int num;
};

struct EdgeData
{
    std::string edge_name;
    double dist;
};

typedef boost::adjacency_list<boost::vecS, boost::vecS,
                              boost::undirectedS,
                              boost::no_property,
                              boost::no_property
                              > MyGraphAdjListType;

typedef boost::adjacency_matrix<boost::undirectedS
                              > MyGraphAdjMatType;

typedef boost::adjacency_list<boost::vecS, boost::vecS,
                              boost::bidirectionalS,
                              boost::no_property,
                              boost::no_property
                              > MyGraphAdjListD;


typedef boost::adjacency_list<boost::setS, boost::vecS,
                              boost::undirectedS,
                              boost::no_property,
                              boost::no_property
                              > MyGraphAdjListTypeNoDupEdge;

template <class Graph>
class MatrixAccessor
{
public:
    typedef typename Graph::Matrix Matrix; //actually a vector<
    typedef typename Matrix::const_reference const_reference;


    MatrixAccessor(const Graph* g)
        : m_g(g)
    {
        static_assert(boost::is_same<size_t, typename Graph::vertex_descriptor>::value, "Vertex descriptor should be of integer type");
    }

    const_reference operator()(const size_t& u, const size_t& v) const
    {
        return m_g->get_edge(u, v);
    }

    const Graph* m_g;
};

/* void use_matrix(const MatrixGraph & mg)
{
    MatrixAccessor<MatrixGraph> matr(&mg);
    assert(matr(0, 1) == 1);
    assert(matr(0, 2) == 0);
}                  */             

#endif