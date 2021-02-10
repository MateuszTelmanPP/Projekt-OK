#include "Graph.h"
#include <omp.h>
#include <iostream>

Graph::Graph(int vertices)
{
	V = vertices;
	dijkstraGraph = new int* [V];
	solutionDist = new int[V];
	dist = new int[V];
	prevDist = new int[V];
	sptSet = new bool[V];

	for (int i = 0; i < V; i++)
	{
		dijkstraGraph[i] = new int[V];

		for (int j = 0; j < V; j++)
		{
			dijkstraGraph[i][j] = 0;
		}

		dist[i] = inf;
		prevDist[i] = 0;
		sptSet[i] = false;
	}
}

void Graph::reset()
{
	for (int i = 0; i < V; i++)
	{
		dist[i] = inf;
		prevDist[i] = 0;
		sptSet[i] = false;
	}
}

void Graph::generateGraph()
{
	double p = rand() / double(RAND_MAX);
	generateGraph(p);
}

void Graph::generateGraph(double p)
{
	int i, j, t;
	std::vector<int> tabT;

	for (int i = 0; i < V; i++)
	{
		for (int j = 0; j < V; j++)
		{
			if (i != j)
			{
				if (rand() / double(RAND_MAX) <= p)
				{
					t = rand() % 10 + 1;
					dijkstraGraph[i][j] = t;
					tabT.clear();
					tabT.push_back(i);
					tabT.push_back(j);
					tabT.push_back(t);
					bellmanGraph.push_back(tabT);
				}
			}
		}
	}
}

void Graph::printBothGraphs()
{
	for (int i = 0; i < V; i++)
	{
		for (int j = 0; j < V; j++)
		{
			printf("%d\t", dijkstraGraph[i][j]);
		}
		printf("\n");
	}

	printf("\n");

	for (int i = 0; i < bellmanGraph.size(); i++)
	{
		printf("%d\t%d\t%d\n", bellmanGraph[i][0], bellmanGraph[i][1], bellmanGraph[i][2]);
	}

	printf("\n");
}

void Graph::printSolution()
{
	printf("Vertex\tDistance from source\n");

	for (int i = 0; i < V; i++)
	{
		dist[i] == inf ? printf("%d\t\t%s\n", i, "inf") : printf("%d\t\t%d\n", i, dist[i]);
	}

	printf("\n\n");
}

void Graph::checkSolution()
{
	bool same = true;

	if (!solutionInitialized)
	{
		for (int i = 0; i < V; i++)
		{
			solutionDist[i] = dist[i];
			solutionInitialized = true;
		}
	}
	else
	{
		for (int i = 0; i < V; i++)
		{
			if (solutionDist[i] != dist[i])
			{
				same = false;
				break;
			}
		}
	}

	if (!same)
	{
		printf("Incorrect result\n");
	}
}

int Graph::minDistance()
{
	int mini = inf;
	int min_index = 0;

	for (int i = 0; i < V; i++)
	{
		if (dist[i] < mini && sptSet[i] == false)
		{
			mini = dist[i];
			min_index = i;
		}
	}

	return min_index;
}

void Graph::dijkstra(int src)
{
	reset();
	dist[src] = 0;

	for (int i = 0; i < V; i++)
	{
		int u = minDistance();
		sptSet[u] = true;

		for (int j = 0; j < V; j++)
		{
			if (dijkstraGraph[u][j] > 0 && !sptSet[j] && dist[j] > dist[u] + dijkstraGraph[u][j])
			{
				dist[j] = dist[u] + dijkstraGraph[u][j];
			}
		}
	}

	checkSolution();
}

void Graph::bellmanFord(int src)
{
	reset();
	dist[src] = 0;
	bool endAlgorithm = false;

	for (int i = 0; i < V - 1; i++)
	{
		endAlgorithm = true;
		for (int j = 0; j < V; j++)
		{
			if (dist[j] != prevDist[j])
			{
				endAlgorithm = false;
				break;
			}
		}
		if (endAlgorithm)
		{
			break;
		}
		for (int j = 0; j < V; j++)
		{
			prevDist[j] = dist[j];
		}
		for (int j = 0; j < bellmanGraph.size(); j++)
		{
			int u = bellmanGraph[j][0];
			int v = bellmanGraph[j][1];
			int w = bellmanGraph[j][2];

			if (dist[u] != inf && dist[u] + w < dist[v])
			{
				dist[v] = dist[u] + w;
			}
		}
	}

	//printSolution();
	checkSolution();
}

void Graph::bellmanFordParallel(int src, int* dist)
{
	reset();
	dist[src] = 0;
	int i, j, u, v, w;
	bool endAlgorithm = false;
	for (int i = 0; i < V - 1; i++)
	{
		endAlgorithm = true;
		for (int j = 0; j < V; j++)
		{
			if (dist[j] != prevDist[j])
			{
				endAlgorithm = false;
				break;
			}
		}
		if (endAlgorithm)
		{
			break;
		}
		for (int j = 0; j < V; j++)
		{
			prevDist[j] = dist[j];
		}
#pragma omp parallel for shared(dist) private(j, u, v, w)
		for (int j = 0; j < bellmanGraph.size(); j++)
		{
			int u = bellmanGraph[j][0];
			int v = bellmanGraph[j][1];
			int w = bellmanGraph[j][2];

			if (dist[u] != inf && dist[u] + w < dist[v])
			{
				dist[v] = dist[u] + w;
			}
		}
	}

	//printSolution();
	checkSolution();
}

int Graph::minDistanceParallel()
{
	int mini = inf;
	int min_index = 0;

#pragma omp parallel
	{
		int mini_private = inf;
		int min_index_private = 0;
#pragma omp for nowait
		for (int i = 0; i < V; ++i)
		{
			if (dist[i] < mini_private && sptSet[i] == false)
			{
				mini_private = dist[i];
				min_index_private = i;
			}
		}
#pragma omp critical 
		{
			if (mini_private < mini)
			{
				mini = mini_private;
				min_index = min_index_private;
			}
		}
	}
	return min_index;
}

void Graph::dijkstraParallel(int src)
{
	reset();
	dist[src] = 0;
	int j;
	for (int i = 0; i < V; i++)
	{
		int u = minDistanceParallel();
		sptSet[u] = true;

		for (int j = 0; j < V; j++)
		{
			if (dijkstraGraph[u][j] > 0 && !sptSet[j] && dist[j] > dist[u] + dijkstraGraph[u][j])
			{
				dist[j] = dist[u] + dijkstraGraph[u][j];
			}
		}
	}

	//printSolution();
	checkSolution();
}