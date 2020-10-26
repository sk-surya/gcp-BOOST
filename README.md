# gcp-BOOST
An Implementation of the following paper:
"An Improved DSATUR‐Based Branch‐and‐Bound Algorithm for the Vertex Coloring Problem"

Paper Abstract:

Given an undirected graph, the Vertex Coloring Problem (VCP) consists of assigning a color to each vertex of the graph in such a way that two adjacent vertices do not share the same color and the total number of colors is minimized. DSATUR-based Branch-and-Bound algorithm (DSATUR) is an effective exact algorithm for the VCP. One of its main drawback is that a lower bound is computed only once and it is never updated. We introduce a reduced graph which allows the computation of lower bounds at nodes of the branching tree. We compare the effectiveness of different classical VCP bounds, plus a new lower bound based on the 1-to-1 mapping between VCPs and Stable Set Problems. Our new DSATUR outperforms the state of the art for random VCP instances with high density, signiﬁcantly increasing the size of instances solved to proven optimality.


The work is not fully completed yet. Work In Progress.
I tried using C++. I wanted to learn Generic Programming in C++. I have used Boost graph Library build the algorithm.
Optimization solver - GUROBI.
