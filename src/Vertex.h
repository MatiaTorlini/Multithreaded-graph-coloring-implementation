//
// Created by Goro on 21/02/2022.
//

#ifndef PDS_PROJECT_SMART_VERTEX_H
#define PDS_PROJECT_SMART_VERTEX_H

#include <memory>
#include <list>
#include <set>
#include <shared_mutex>


class Vertex {
    long random;
    long id;
    int color;
    std::list<Vertex*> neighbors;
    std::list<Vertex*> neighbors_updated_jp;
//####################SDL################################
    std::atomic<int> degree;
    std::list<Vertex*> neighbors_updated_sdl;
    int weight;
    std::shared_mutex m;
//######################################################

public:
    Vertex(long i) : id(i), color(-1), random(-1) {
        degree.store(-1);
        weight = -1;
    }

    ///Sets the random of this vertex to r.
    void set_random(long r) {
        random = r;
    }

    ///Returns the color of this vertex.
    int get_color() const {
        return color;
    }

    ///Adds the neighbor pointed by neigh to the adjacency list of this vertex.
    void add_neighbor(Vertex* neigh) {
        neighbors.push_back(neigh);
        neighbors_updated_jp.push_back(neigh);
        neighbors_updated_sdl.push_back(neigh);
        degree++;
    }

    ///Returns true if the color of the first vertex pointed by v1 is smaller than the one pointed by v2.
    static bool has_smaller_color(Vertex* v1, Vertex* v2) {
        return v1->get_color()<v2->get_color();
    }

    ///Returns true if this vertex has the biggest random in its neighborhood.
    bool has_biggest_random() {
        for (const auto& n : neighbors_updated_jp)
            if (this->random < n->random) {
                return false;
            }
        return true;
    }

    ///Assigns the minimum available color in the neighborhood to this vertex.
    int assign_color() {
        color = 0;
        neighbors.sort(has_smaller_color);
        for (const auto& n : neighbors)
            if (this->color == n->color)
                this->color++;
            else if (this->color < n->color) {
                return this->color;
            }
        return this->color;
    }

    ///Returns true if the random of vertex pointed by v1 is smaller than the one pointed by v2.
    static bool compare_randoms(Vertex* v1, Vertex* v2) {
        return v1->random < v2->random;
    }

    ///Reorders adjacency list of this vertex basing on their random number.
    void reorder_adjs() {
        neighbors.sort(compare_randoms);
    }

    ///Removes a neighbor from the adjacency list dedicated to the color assignment algorithm.
    void delete_neighbor_jp(Vertex* n) {
        neighbors_updated_jp.remove(n);
    }

    ///Removes this vertex from the adjacency lists of its neighbors.
    ///(NOTE: the adjacency lists used is dedicated to the coloring algorithm)
    void remove_jp() {
        for (auto& n : neighbors)
            n->delete_neighbor_jp(this);
    }

    ///Returns true if this vertex has a unique color in its neighborhood.
    bool check_coloring() {
        for (const auto& n : neighbors)
            if (this->color == n->color)
                return false;
        return true;
    }

    ///Prints the id and the main attributes of this vertex.
    void print_vertex() const {
        std::cout<<"[id : "<<id<<"][color : "<<color<<"][random : "<<random<<"][weight : "<<weight<<"]\n";
    }

    ///Prints the original adjacency list of this vertex.
    void print_vertex_adjs() {
        std::cout<<"{";
        for (const auto& v : neighbors)
            std::cout<<v->id<<" ";
        std::cout<<"}\n";
    }

    //#############################SDL####################################

    ///Initializes the degree of this vertex to the count of its neighbors.
    void init_degree() {
        degree.store(neighbors.size());
    }

    ///Returns the weight of this vertex.
    int get_weight() const{
        return weight;
    };

    ///Locks a shared mutex, and returns true if the degree of this vertex is <= the parameter d.
    bool compare_degrees(int d)
    {
        std::shared_lock<std::shared_mutex> lock(m);
        return degree.load() <= d;
    }

    ///Decreases the degree of this vertex by one.
    void decrease_degree()
    {
        --degree;
    }

    ///Sets the weight of this vertex to the parameter w.
    void set_weight(int w)
    {
        weight = w;
    }

    ///Deletes the neighbor pointed by neigh from the adjacency list dedicated to the weighting algorithm.
    void delete_neighbor_sdl(Vertex* neigh){
        neighbors_updated_sdl.remove(neigh);
        decrease_degree();
    }

    ///Removes this vertex from the adjacency lists of its neighbors.
    ///(NOTE: the adjacency lists used is dedicated to the weighting algorithm)
    void remove_sdl() {
        for (auto& n : neighbors_updated_sdl) {
            n->delete_neighbor_sdl(this);
        }
    }

    ///Returns true if the vertex has weight > all the weights of its neighborhood (conflicts resolved by random numbers).
    bool has_biggest_weight() {
        for (auto& n : neighbors_updated_jp) {
            if (n->weight > this->weight) {
                return false;
            }
            if (n->weight == this->weight)
                if (n->random > this->random)
                    return false;
        }
        return true;
    }
};


#endif //PDS_PROJECT_SMART_VERTEX_H
