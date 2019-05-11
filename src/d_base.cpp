#include "graph.hpp"

#include <string>
#include <iostream>
#include <random>
#include <chrono>
#include <stdexcept>

#include <sys/stat.h>

struct Triple {
    std::vector<int> v;

    Triple(int v1, int v2, int v3) :v{v1, v2, v3} {
        std::sort(v.begin(), v.end());
    }

    const std::vector<int>& values() const {
        return v;
    }

    friend bool operator== (const Triple&, const Triple&);
};

inline bool operator== (const Triple& t1, const Triple& t2) {
    auto v1 = t1.values();
    auto v2 = t2.values();

    for (int i = 0; i < 3; i++) {
        if (v1[i] != v2[i]) {
            return false;
        }
    }

    return true;
}

namespace std {
    template <> struct hash<Triple> {
        size_t operator()(const Triple& t) const {
            size_t seed = 0;
            for (size_t v : t.values()) {
                boost::hash_combine(seed, v);
            }
            
            return seed;
        }
    };
}

unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
std::default_random_engine generator(seed);

Triple generate_random_triple(int n) {
    std::uniform_int_distribution<int> distribution(1, n);
    int v1 = distribution(generator);
    int v2 = distribution(generator);
    int v3 = distribution(generator);

    while (v1 == v2) {
        v2 = distribution(generator);
    }

    while (v1 == v3 || v2 == v3) {
        v3 = distribution(generator);
    }

    return Triple(v1, v2, v3);
}

std::unordered_set<Triple> generate_triples(int m, int n) {
    std::unordered_set<Triple> outTriples;
    while ((int) outTriples.size() != m) {
        outTriples.insert(generate_random_triple(n));
    }

    return outTriples; 
}

Graph generate_random_graph(int m, int n, std::unordered_set<Triple> outTriples) {
    std::unordered_set<Edge> edges;

    int i = 1;
    for (Triple outTriple : outTriples) {
        for (int v : outTriple.values()) {
            edges.insert({i, m+v});
        }

        i += 1;
    }

    return Graph(n+m, edges);
}

void export_triples(int m, int n, std::unordered_set<Triple> triples, std::string filename) {
    std::ofstream o(filename);
    o << m << ' ' << n << '\n';
    for (Triple triple : triples) {
        for (int v : triple.values()) {
            o << v << ' ';
        }
        o << '\n';
    }
}

// reducing uniquely satisfiable 3-XOR to unsatisfiable CNF
// reduce to DNF in between then use de morgan's theorem


// construction is [clauses] [variables]
// but SAT solver takes format [variables] [clauses]
void export_SAT_CNF(int m, int n, std::unordered_set<Triple> triples, std::string filename) {
    std::ofstream o(filename);
    o << "p cnf " << n << ' ' << 6*m + 1 << '\n';
    // clause to eliminate trivial zero
    for (int i = 1; i <= n; i++) {
        o << '-' << i << ' ';
    }
    o << "0\n";
    for (Triple triple : triples) {
        auto variables = triple.values();
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                o << ((i != j) ? "-" : "") << variables[j] << ' ';
            }
            o << "0\n";
            // 0 acts as a line terminator
        }

        for (int i = 0; i < 3; i++) {
            o << variables[i] << ' ';
        }
        o << "0\n";
    }
}

int main(int argc, char** argv) {
    if (argc != 4) {
        std::cout << "Invalid input: ./construction [m] [n] [name]" << std::endl;;
        return 1; 
    }

    int m = std::stoi(argv[1]);
    int n = std::stoi(argv[2]);
    std::string name = argv[3];

    // there are nC3 number of triples,
    // and each of the m nodes should map to a unique triple
    if (n * (n-1) * (n-2) / 6 < m) {
        std::cout << "n is too small relative to m." << std::endl;
        return 2;
    }

    auto triples = generate_triples(m, n);
    
    mkdir(("./" + name).c_str(), 0777);
    export_triples(m, n, triples, name + "/triples.tri");
    export_SAT_CNF(m, n, triples, name + "/clauses.cnf");
    Graph base_graph = generate_random_graph(m, n, triples);

    base_graph.exportDreadnaut(name + "/base.dre");
    base_graph.exportDIMACS(name + "/base.dmc");

    return 0;
} 
