#ifndef _GRAPH_H
#define _GRAPH_H

#include "pch.h"

namespace Surya{
struct Edge
{
    int src, dest;
    Edge()
    {
        src = -1;
        dest = -1;
    }
    Edge(int src_in, int dest_in)
    {
        src = src_in;
        dest = dest_in;
    }
};

class Graph
	{
	private:
		int vertex_count{0};
		int edge_count{0};
		bool is_undirected{true};
		std::vector<Edge> undirected_edge_list;

		void populateAdjMatrix(std::vector<std::vector<int>> const &adjList);
	public:
		// construct a std::vector of std::vectors of pairs to represent an adjacency list
		
		// Graph Constructor
		std::vector<std::vector<int>> adjList;
		std::vector<std::vector<char>> adjMatrix;

		Graph(std::vector<Edge> const &edges, int N, bool undirected);

		int vertexCount() const;

		// bool isUndirected() const;

		int edgeCount() const;

		std::vector<Edge> undirectedEdgeList() const;
        void printGraph();


}; // class Graph
}

#endif