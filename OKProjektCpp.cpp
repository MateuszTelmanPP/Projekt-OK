#include <iostream>
#include <ctime>
#include <cstdlib>
#include <chrono>
#include <fstream>
#include "Graph.h"

#define averageNumber 3
#define density 0.1

int main()
{
	srand(time(0));

	std::ofstream out;
	out.open("data.txt");

	double a1 = 0, a2 = 0, a3 = 0, a4 = 0;
	const int n[7] = { 500, 1000, 2000, 4000, 6000, 8000, 10000 };

	//graph.printBothGraphs();

	for (int i = 0; i < 7; i++)
	{
		std::cout << i << ": ";
		for (int j = 0; j < averageNumber; j++)
		{
			std::cout << j << " ";

			Graph graph(n[i]);
			graph.generateGraph(density);

			auto start = std::chrono::steady_clock::now();
			graph.dijkstra(0);
			auto finish = std::chrono::steady_clock::now();
			a1 += (double)std::chrono::duration_cast<std::chrono::nanoseconds>(finish - start).count();

			start = std::chrono::steady_clock::now();
			graph.dijkstraParallel(0);
			finish = std::chrono::steady_clock::now();
			a2 += (double)std::chrono::duration_cast<std::chrono::nanoseconds>(finish - start).count();

			start = std::chrono::steady_clock::now();
			graph.bellmanFord(0);
			finish = std::chrono::steady_clock::now();
			a3 += (double)std::chrono::duration_cast<std::chrono::nanoseconds>(finish - start).count();

			start = std::chrono::steady_clock::now();
			graph.bellmanFordParallel(0, graph.dist);
			finish = std::chrono::steady_clock::now();
			a4 += (double)std::chrono::duration_cast<std::chrono::nanoseconds>(finish - start).count();
		}

		out << n[i] << "\t" << (a1 / averageNumber) / 1000000000 << "\t" << (a2 / averageNumber) / 1000000000 << "\t" << (a3 / averageNumber) / 1000000000 << "\t" << (a4 / averageNumber) / 1000000000 << "\n";
		a1 = a2 = a3 = a4 = 0;
		std::cout << std::endl;
	}

	out.close();
}