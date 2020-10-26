# gcp-BOOST
An Implementation of the following paper:
"An Improved DSATUR‐Based Branch‐and‐Bound Algorithm for the Vertex Coloring Problem"

Paper Abstract:

Given an undirected graph, the Vertex Coloring Problem(VCP) consists of assigning a color to each vertex ofthe graph in such a way that two adjacent vertices donot share the same color and the total number of colorsis minimized. DSATUR-based Branch-and-Bound algo-rithm (DSATUR) is an effective exact algorithm for theVCP. One of its main drawback is that a lower bound iscomputed only once and it is never updated. We intro-duce a reduced graph which allows the computation oflower bounds at nodes of the branching tree. We com-pare the effectiveness of different classical VCP bounds,plus a new lower bound based on the 1 -to- 1 map-ping between VCPs and Stable Set Problems. Our newDSATUR outperforms the state of the art for random VCPinstances with high density, signiﬁcantly increasing thesize of instances solved to proven optimality.


The work is not fully completed yet. Work In Progress.
I tried using C++. I wanted to learn Generic Programming in C++. I have used Boost graph Library build the algorithm.
Optimization solver - GUROBI.
