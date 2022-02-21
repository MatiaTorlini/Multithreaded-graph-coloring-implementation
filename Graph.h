//
// Created by Goro on 21/02/2022.
//

#ifndef PDS_PROJECT_SMART_GRAPH_H
#define PDS_PROJECT_SMART_GRAPH_H

#include "Vertex.h"
#include <vector>
#include <set>
#include <string>


class Graph{
    size_t size;
    std::vector<std::shared_ptr<Vertex>> vertices;
    std::set<int> colors;
public:
    Graph(){

    };

    void set_size(long s) {
        size = s;
    }

    void add_vertex(std::shared_ptr<Vertex> v) {
        vertices.push_back(v);
    }

    void add_neighbor(long id, long neigh)  {
        vertices[id].get()->add_neighbor(vertices[neigh]);
    }

    void print_graph() {
        for (auto v : vertices) {
            v->print_vertex();
            v->print_vertex_adjs();
        }
    }

};

#endif //PDS_PROJECT_SMART_GRAPH_H
