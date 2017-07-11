#ifndef GRAPH_H_INCLUDED
#define GRAPH_H_INCLUDED

#include <iostream>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <vector>
#include <ctime>
#include <climits>

using namespace std;

class Graph
{
public:

    ///constructor
    Graph(int gSize);
    ///desctructor
    ~Graph();

    ///variables
    //number of vertices and edges
    int V, E;

    ///functions
    //verify if edge exists
    bool existEdge(vector<int> par);
    //insert element on graph
    void insertEdge(vector<int> par);
    //remove edge from graph
    void removeEdge(vector<int> par);
    //displays the adjacency matrix
    void disp();

    ///clique partitioning
    //option 1: Greedy with heuristics
    vector<vector<int> > cliquePartGreedy();
    //option 2: search for maxclique in every vertices
    vector<vector<int> > cliquePartBTH(double timelimit);
    //option 3: exact clique partitioning search
    vector<vector<int> > cliquePartBTE(double timelimit);
private:

    ///variables
    //adjacency matrix
    bool ** adjMat;

    ///auxiliar functions
    //gets degrees from all the vectors
    vector<int> vertDegree();
    //checks if a partition is a clique
    bool isClique(vector<int> part);
    //insert a node in partition
    vector<vector<int> > insertInPartition (vector<vector<int> > part, int elem);
};

#endif // GRAPH_H_INCLUDED
