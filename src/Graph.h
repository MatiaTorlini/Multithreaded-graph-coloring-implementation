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
    std::mutex m;

public:
    Graph(){};

    /// Returns a vector of pointers to Vertices for being shuffled in the sequential greedy algorithm
    std::vector<Vertex*> vector_to_shuffle(){
        std::vector<Vertex*> V;
        for (const auto& v:vertices)
            V.push_back(v.get());
        return V;
    }

    ///Returns the number of vertices in the graph.
    size_t get_size() {
        return size;
    }

    ///Set the number of vertices in the graph.
    void set_size(long s) {
        size = s;
    }

    ///Adds a vertex to the graph
    void add_vertex(const std::shared_ptr<Vertex>& v) {
        vertices.push_back(v);
    }

    ///Adds the neighbor neigh to the adjacency list of the vertex identified by id.
    void add_neighbor(long id, long neigh)  {
        vertices[id].get()->add_neighbor(vertices[neigh].get());
    }

    ///Assigns the random number ran to the vertex identified by id.
    void assign_random(long id, long ran) {
        vertices[id]->set_random(ran);
    }

    ///Returns true if the vertex has the biggest random in its adjacency list.
    bool has_biggest_random(long id) {
        return vertices[id]->has_biggest_random();
    }

    ///Returns true if the vertex has the biggest degree in its adjacency list.
    bool has_biggest_degree(long id) {
        return vertices[id]->has_biggest_degree();
    }

    ///Returns true if the vertex has weight > all the weights of its neighborhood
    /// (conflicts resolved by random numbers).
    bool has_biggest_weight(long id) {
        return vertices[id]->has_biggest_weight();
    }

    ///Assigns the minimum available color in the neighborhood to the vertex identified by id and
    /// inserts it in the set of used colors.
    void color(long id) {
        int c = vertices[id].get()->assign_color();
        std::scoped_lock<std::mutex> lock(m);
        colors.insert(c);
    }

    ///Remove the vertex identified by id from the adjacency lists of its neighbors.
    ///(NOTE: the adjacency lists used is dedicated to the coloring algorithm)
    void remove_jp(long id) {
        vertices[id]->remove_jp();
    }

    ///Prints the number of color used.
    void print_n_colors() {
        std::cout<<"#colors used : "<<colors.size()<<"\n";
    }

    ///Reorders adjacency lists of all vertices basing on their random number.
    void reorder_adjs() {
        for (auto& v : vertices)
            v->reorder_adjs();
    }

    ///Returns true if the color of all vertices is unique in their neighborhood.
    void check_coloring() {
        for (auto& v : vertices)
            if (!v->check_coloring()) {
                std::cout<<"BAD COLORING\n";
                return;
            }
        std::cout<<"GOOD COLORING\n";
    }

    ///Prints all the vertices of the graph and their main attributes.
    void print_graph() {
        for (auto& v : vertices) {
            v->print_vertex();
            //v->print_vertex_adjs();
        }
    }

    ///Outputs the results of the algorithm to file.
    void print_vertices_to_file(const std::string& output_file) {
        std::ofstream out(output_file, std::ofstream::out);
        out<<size<<" "<<colors.size()<<"\n";
        for (auto &v : vertices)
            out<<std::to_string(v->get_color())+"\n";
    }

    //###########################SDL##################################

    ///Initializes the degree of a vertex to the count of its neighbors.
    void init_degree(long int id) {
        vertices[id]->init_degree();
    }

    ///Returns true if the degree of the vertex identified by id is lower or equal the parameter d,
    /// and assigns the weight w in this case
    bool try_to_weight(long int id, int d, int w){
        if(vertices[id]->compare_degrees(d))
        {
            vertices[id]->set_weight(w);
            return true;
        }
        return false;
    }

    ///Removes the vertex identified by id from the adjacency lists of its neighbors.
    ///(NOTE: the adjacency lists used is dedicated to the weighting algorithm)
    void remove_sdl(long int id) {
        vertices[id]->remove_sdl();
    }

    //###############################################################
};

#endif //PDS_PROJECT_SMART_GRAPH_H
