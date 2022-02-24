//
// Created by Goro on 21/02/2022.
//

#ifndef PDS_PROJECT_SMART_GRAPH_H
#define PDS_PROJECT_SMART_GRAPH_H

#include "Vertex.h"
#include <vector>
#include <set>
#include <string>
#include <iostream>

class Graph{
    size_t size;
    std::vector<std::shared_ptr<Vertex>> vertices;
    std::set<int> colors;

public:
    Graph(){};

    std::vector<Vertex*> vector_to_shuffle(){
        std::vector<Vertex*> V;
        for (const auto& v:vertices)
            V.push_back(v.get());
        return V;
    }


    size_t get_size() {
        return size;
    }

    void set_size(long s) {
        size = s;
    }

    void add_vertex(const std::shared_ptr<Vertex>& v) {
        vertices.push_back(v);
    }

    void add_neighbor(long id, long neigh)  {
        vertices[id].get()->add_neighbor(vertices[neigh].get());
    }

    void assign_random(long id, long ran) {
        vertices[id]->set_random(ran);
    }

    /*void assign_randoms() {
        for (const auto& v : vertices)
            v->set_random(xorshf96());
    }*/ //ASSIGN RANDOM WITH XORSHF96

    bool has_biggest_random(long id) {
        return vertices[id]->has_biggest_random();
    }
    bool has_biggest_weight(long id) {
        return vertices[id]->has_biggest_weight();
    }

    void color(long id) {
         colors.insert(vertices[id].get()->assign_color());
    }

    void remove_jp(long id) {
        vertices[id]->remove_jp();
    }

    void print_n_colors() {
        std::cout<<"#colors used : "<<colors.size()<<"\n";
    }

    void reorder_adjs() {
        for (auto& v : vertices)
            v->reorder_adjs();
    }

    void check_coloring() {
        for (auto& v : vertices)
            if (!v->check_coloring()) {
                std::cout<<"BAD COLORING\n";
                return;
            }
        std::cout<<"GOOD COLORING\n";
    }

    void print_graph() {
        for (auto& v : vertices) {
            v->print_vertex();
            //v->print_vertex_adjs();
        }
    }

    void print_vertices_to_file(const std::string& output_file) {
        std::ofstream out(output_file, std::ofstream::out);
        out<<size<<" "<<colors.size()<<"\n";
        for (auto &v : vertices)
            out<<std::to_string(v->get_color())+"\n";
    }

    //###########################SDL##################################

    //Initializes the degree of a vertex to the count of its neighbors.
    void init_degree(long int id) {
        vertices[id]->init_degree();
    }

    //Returns true if the degree of the vertex identified by id is <= the parameter d,
    // and assigns the weight w in this case
    bool try_to_weight(long int id, int d, int w){
        if(vertices[id]->compare_degrees(d))
        {
            vertices[id]->set_weight(w);
            return true;
        }
        return false;
    }

    //Removes the vertex identified by id from the updated version of the adjacency lists dedicated
    // to the weighting algorithm.
    void remove_sdl(long int id) {
        vertices[id]->remove_sdl();
    }

    //###############################################################
};

#endif //PDS_PROJECT_SMART_GRAPH_H
