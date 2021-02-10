#pragma once
#include <vector>
#include <string>

#define inf 999999

class Graph
{
    int V;
    int** dijkstraGraph;
    std::vector<std::vector <int>> bellmanGraph;
    int* solutionDist;
    bool solutionInitialized = false;
    bool* sptSet;
    int x = 0;

    void reset();
    void checkSolution();
    int minDistance();
    int minDistanceParallel();
    void bellmanFordParallelE(int a, int b);

public:
    Graph(int vertices);
    int* dist;
    int* prevDist;
    void generateGraph();
    void generateGraph(double p);
    void printBothGraphs();
    void printSolution();
    void dijkstra(int src);
    void dijkstraParallel(int src);
    void bellmanFord(int src);
    void bellmanFordParallel(int src, int*);
};

