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

    size_t get_size() const {
        return size;
    }

    void add_vertex(const std::shared_ptr<Vertex>& v) {
        vertices.push_back(v);
    }

    void add_neighbor(long id, long neigh)  {
        vertices[id].get()->add_neighbor(vertices[neigh]);
    }

    void print_graph() {
        for (auto v : vertices) {
            v->print_vertex();
            //v->print_vertex_adjs();
        }
    }

    void init_tmp_degree(long int id) {
        vertices[id]->init_tmp_degree();
    }

    bool try_to_weight(long int id, int degree, int weight){
        if(vertices[id]->compare_degrees(degree))
        {
            vertices[id]->set_weight(weight);
            return true;
        }
        return false;
    }

    void remove(long int id) {
        vertices[id]->self_remove();
    }

    void check_weights(){
        if(
            vertices[5]->get_weight() == 0 and
            vertices[3]->get_weight() == 1 and
            vertices[2]->get_weight() == 1 and
            vertices[0]->get_weight() == 1 and
            vertices[4]->get_weight() == 2 and
            vertices[1]->get_weight() == 2)
            std::cout<<"(OK)\n";
        else std::cout<<"weights are wrong.\n";

    }

    /*
    int sdl_assign_color(long int id) {
        long int conflict = vertices[id]->conflict();
        if (conflict)
            vertices[conflict]->assign_color();
        vertices[id]->assign_color();
    }
    */

};

#endif //PDS_PROJECT_SMART_GRAPH_H
