#include "graph.hpp"
#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

#include <sys/stat.h>

int main(int argc, char** argv) {
    if (argc != 3) {
        std::cout << "Usage [n] [name]" << std::endl;
        return 1;
    }

    int n = std::stoi(argv[1]);
    std::string name = argv[2];
    if (n % 2 == 1) {
        std::cout << "n must be even" << std::endl;
        return 2;
    }

    // generate base graph
    std::vector<Edge> reg_edges;
    for (int i = 1; i < n; i++) {
        reg_edges.push_back({i, i+1});
    }
    reg_edges.push_back({n, 1}); 
    for (int i = 1; i <= n/2; i++) {
        reg_edges.push_back({i, i+n/2});
    }

    int e_size = 3*n/2;
    // generate permutation
    std::vector<int> sigma(e_size);
    for (int i = 0; i < e_size; i++) {
        sigma[i] = i;
    }
    std::random_shuffle(sigma.begin(), sigma.end());

    // connect V with E
    std::unordered_set<Edge> base_edges;
    for (int i = 0; i < e_size; i++) {
        Edge edge = reg_edges[i];
        for (Vertex v : edge.vertices()) {
            base_edges.insert({v.get_id(), i+1 + 2*n});
        }
    }
    // connect V' with E
    for (int i = 0; i < e_size; i++) {
        Edge edge = reg_edges[sigma[i]];
        for (Vertex v : edge.vertices()) {
            base_edges.insert({v.get_id() + n, i+1 + 2*n});
        }
    }

    Graph base_graph(n + n + e_size, base_edges);
   
    mkdir(("./" + name).c_str(), 0777);
    base_graph.exportDIMACS(name + "/base.dmc");
    base_graph.exportDreadnaut(name + "/base.dre");

    return 0;
}
