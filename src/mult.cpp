#include "graph.hpp"
#include <iostream>
#include <stdexcept>
#include <string>

bool wirings[3][4] = {{0,0,1,1}, {0,1,0,1}, {0,1,1,0}};

Graph multipede_construction(Graph& base_graph, int m, int n) {
    std::unordered_set<Edge> multi_edges;

    auto adj = base_graph.adjs();

    for (int i = 1; i <= m; i++) {
        auto edges = adj[i];
        if (edges.size() != 3) {
            throw std::runtime_error("nodes in M do not have exactly 3 neighbours.");
        }

        int c = 0;
        for (Edge edge : edges) {
            auto vs = edge.vertices();
            int j = (((vs[0].get_id() != i) ? vs[0] : vs[1]).get_id()) - m;

            for (int inner = 0; inner <= 3; inner++) {
                multi_edges.insert({4*(i-1) + inner + 1, 4*m + wirings[c][inner]*n + j});
            }

            c += 1;
        }
    }

    return Graph(4*m + 2*n, multi_edges);
}

int main(int argc, char** argv) {
    if (argc != 4) {
        std::cout << "Invalid input: ./construction [m] [n]\n";
        return 1;
    }

    std::string filename = argv[1];
    int m = std::stoi(argv[2]);
    int n = std::stoi(argv[3]);

    Graph base_graph(filename);
    Graph mult_graph = multipede_construction(base_graph, m, n);

    mult_graph.exportDreadnaut("mult.dre");
    mult_graph.exportDIMACS("mult.dmc");
    return 0;
} 
