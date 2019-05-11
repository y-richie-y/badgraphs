#include <algorithm>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <utility>

#include <boost/functional/hash.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>

class Vertex {
private:
    int id;
public:
    Vertex(int _id=0) : id(_id) {}

    int get_id() const {
        return id;
    }

    friend bool operator== (const Vertex&, const Vertex&);
    friend std::ostream& operator<< (std::ostream&, const Vertex&);
};

inline std::ostream& operator<< (std::ostream& strm, const Vertex& v) {
    return strm << "[Vertex " << v.id << ']';
}

inline 
bool operator== (const Vertex& lhs, const Vertex& rhs) {
    return lhs.id == rhs.id;
}

class Edge {
private:
    std::vector<Vertex> vs;
public:
    Edge(Vertex v1, Vertex v2) : vs{v1, v2} {  }

    const std::vector<Vertex> vertices() const {
        return vs;
    }

    bool contains(const Vertex& v) const {
        return vs[0] == v || vs[1] == v;
    }

    friend bool operator== (const Edge&, const Edge&);
    friend std::ostream& operator<< (std::ostream&, const Edge&);
};

std::ostream& operator<< (std::ostream& strm, const Edge& e) {
    auto vs = e.vertices();
    return strm << "[Edge " << vs[0] << ' ' << vs[1] << ']';
}

inline 
bool operator== (const Edge& lhs, const Edge& rhs) {
    return lhs.vs[0] == rhs.vs[0] && lhs.vs[1] == rhs.vs[1];
}

namespace std {
  template <> struct hash<Vertex> {
    size_t operator()(const Vertex& v) const {
        return hash<int>{}(v.get_id());
    }
  };

  template <> struct hash<Edge> {
    size_t operator()(const Edge& e) const {
        size_t seed = 0;

        for (Vertex v : e.vertices()) {
            boost::hash_combine<size_t>(seed, hash<Vertex>{}(v));
        }

        return seed;
    }
  };
}

class Graph {
private:
    // how to make v_size final?
    std::unordered_map<Vertex, std::unordered_set<Edge>> adj;
    int v_size, e_size;
public:
    Graph(std::string inputFile) {
        std::ifstream fin(inputFile);
        std::string skip;
        
        fin >> skip >> skip >> v_size >> e_size;

        for (int i = 0; i < e_size; i++) {
            int u, v;
            fin >> skip >> u >> v;
            if (u <= v_size && v <= v_size) {
                addEdge({u, v});
            } else {
                throw std::runtime_error("Vertices out of range.");
            }
        }
    }    


    Graph(int _v_size, std::unordered_set<Edge> edges) : v_size{_v_size} {
        e_size = edges.size();

        for (Edge edge : edges) {
            auto vs = edge.vertices();
            if (vs[0].get_id() <= v_size && vs[1].get_id() <= v_size) {
               addEdge(edge); 
            } else {
                throw std::runtime_error("Vertices out of range.");
            }
        }
    }

    std::unordered_set<Edge> edges() {
        std::unordered_set<Edge> all_edges;

        for (auto entry : adj) {
            auto edges = entry.second;
            for (Edge edge : edges) {
                all_edges.insert(edge);
            }
        }

        return all_edges;
    }

    std::unordered_map<Vertex, std::unordered_set<Edge>> adjs() {
        return adj;
    }

    // we don't increase e_size here
    void addEdge(Edge e) {
        auto vs = e.vertices();
        adj[vs[0]].insert(e);
        adj[vs[1]].insert(e);
   }

    void exportDIMACS(std::string filename) {
        std::ofstream fout (filename);
        fout << "p edge " << v_size << " " << e_size << '\n';
        for (auto entry : adj) {
            Vertex v = entry.first;
            auto edges = entry.second;
            for (Edge edge : edges) {
                // check v if necessary
                auto vs = edge.vertices();
                if (v.get_id() == vs[0].get_id()) {
                    fout << "e " << vs[0].get_id() << " " << vs[1].get_id() << "\n";
                }
            }
        }
    }


typedef boost::property_tree::ptree ptree;

private:
    // Basic skeleton required for a GraphML file
    static ptree GraphMLTemplate() {
        ptree pt;
        
        ptree graphml;
        graphml.put("<xmlattr>.xmlns", "http://graphml.graphdrawing.org/xmlns");
        graphml.put("<xmlattr>.xmlns:xsi", "http://www.w3.org/2001/XMLSchema-instance");
        graphml.put("<xmlattr>.xsi:schemaLocation", "http://graphml.graphdrawing.org/xmlns http://graphml.graphdrawing.org/xmlns/1.0/graphml.xsd");
        
        pt.add_child("graphml", graphml);

        ptree colour_key;
        colour_key.put("<xmlattr>.id", "color");
        colour_key.put("<xmlattr>.for", "node");
        colour_key.put(ptree::path_type("<xmlattr>/attr.name", '/'), "color");
        colour_key.put(ptree::path_type("<xmlattr>/attr.type", '/'), "string");

        pt.add_child("graphml.key", colour_key);
        pt.put("graphml.graph.<xmlattr>.edgedefault", "undirected"); //make graph undirected
    
        return pt;
    }

public:
    void exportGraphML(std::string filename, std::function<std::string(int)> node_colour = [](int i){ return "black"; }) {
        auto pt = GraphMLTemplate();

        for (int i = 1; i <= v_size; i++) {
            ptree node;
            node.put("<xmlattr>.id", i);

            node.put("data.<xmlattr>.key", "color");
            node.put("data", node_colour(i));
                        
            pt.add_child("graphml.graph.node", node);
        }

        for (auto entry : adj) {
            auto edges = entry.second;
            for (Edge edge : edges) {
                auto vs = edge.vertices();
                ptree node;
                node.put("<xmlattr>.source", vs[0].get_id());
                node.put("<xmlattr>.target", vs[1].get_id());
                pt.add_child("graphml.graph.edge", node);
            }
        }

        std::ofstream o(filename);
        boost::property_tree::write_xml(o, pt);
    }

    void exportDreadnaut(std::string filename) {
        std::ofstream o(filename);
        
        o << "$=1 n=" << v_size << " g";
        for (int v_id = 0; v_id < v_size; v_id++) {
            auto edges = adj[Vertex(v_id)];

            bool no_entries = true;
            for (Edge edge : edges) {
                auto vs = edge.vertices();
                int w_id = ((vs[0].get_id() != v_id) ? vs[0] : vs[1]).get_id();

                // only output edges that go to a higher valued vertex
                if (v_id < w_id) {
                    if (no_entries) {
                        o << '\n' << v_id << ":";
                        no_entries = false;
                    }
                    o << ' ' << w_id;
                }  
            }
        }
        o << ".\n$$" << std::endl;
    }
};
