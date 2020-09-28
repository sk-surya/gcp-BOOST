// Implementation for class Graph 
#include "Graph.h"

// void Surya::Graph::populateAdjMatrix(std::vector<std::vector<int>> const &adjList)
// {
//     adjMatrix.resize(vertex_count, std::vector<char>(vertex_count, 0));
    
//     for (int i = 0; i < vertex_count; i++)
//     {
//      // TODO   
//     }
// }


// Surya::Graph::Graph(std::vector<Edge> const &edges, int N, bool undirected)
// {
//     // resize the std::vector to N elements of type std::vector<Pair>
//     // NOTE : adjList[0] represents Vertex 1 in problem
//     vertex_count = N;
//     adjList.resize(N);
//     is_undirected = undirected;
//     undirected_edge_list = edges;
//     // add edges to the directed graph
//     for (auto &edge : edges)
//     {
//         int src = edge.src;
//         int dest = edge.dest;

//         // insert at the end
//         adjList[src - 1].push_back(dest - 1);
//         edge_count += 1;

//         if (is_undirected)
//         {
//             adjList[dest - 1].push_back(src - 1);
//             edge_count += 1;
//         }
//     }
// }

// int Surya::Graph::vertexCount() const
// {
//     return vertex_count;
// }


int Surya::Graph::edgeCount() const
{
    return edge_count;
}

std::vector<Surya::Edge> Surya::Graph::undirectedEdgeList() const
{
    return undirected_edge_list;
}

void Surya::Graph::printGraph()
{
    for (int i = 0; i < vertex_count; i++)
    {
        // print all neighbouring vertices of given vertex
        std::cout << i << ": ";

        for (auto& v : adjList[i])
            std::cout << v << " ";
        std::cout << "\n";
    }
}