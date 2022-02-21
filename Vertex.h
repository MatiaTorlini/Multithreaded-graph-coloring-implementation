//
// Created by Goro on 21/02/2022.
//

#ifndef PDS_PROJECT_SMART_VERTEX_H
#define PDS_PROJECT_SMART_VERTEX_H

#include <memory>
#include <list>

class Vertex {
    long id;
    int color;
    std::atomic<int> i;
    int weight;
    long random;
    std::list<std::shared_ptr<Vertex>> neighbors;
public:
    Vertex(long i) : id(i), color(-1){};
    void add_neighbor(std::shared_ptr<Vertex> neigh) {
        neighbors.push_back(neigh);
    }

    void print_vertex() {
        std::cout<<"[id : "<<id<<"][color : "<<color<<"]\n";
    }

    void print_vertex_adjs() {
        std::cout<<"{";
        for (auto v : neighbors)
            std::cout<<v->id<<" ";
        std::cout<<"}\n";
    }
};



#endif //PDS_PROJECT_SMART_VERTEX_H
