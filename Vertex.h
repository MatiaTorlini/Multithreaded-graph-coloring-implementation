//
// Created by Goro on 21/02/2022.
//

#ifndef PDS_PROJECT_SMART_VERTEX_H
#define PDS_PROJECT_SMART_VERTEX_H

#include <memory>
#include <list>
#include <mutex>
#include <shared_mutex>

class Vertex : public std::enable_shared_from_this<Vertex> {
    long id;
    int color;
    std::atomic<int> i;
    std::shared_mutex m;
    int weight;
    std::atomic<int> tmp_degree;
    long random;
    std::list<std::shared_ptr<Vertex>> neighbors;

public:
    Vertex(long i) : id(i), color(-1), weight(-1){};

    std::shared_ptr<Vertex> self(){
        return shared_from_this();
    }

    void add_neighbor(const std::shared_ptr<Vertex>& neigh) {
        neighbors.push_back(neigh);
    }

    void print_vertex() const {
        std::cout<<"[id : "<<id<<"][weight : "<<weight<<"][degree : "<<tmp_degree<<"]\n";
    }

    void print_vertex_adjs() {
        std::cout<<"{";
        for (const auto& v : neighbors)
            std::cout<<v->id<<" ";
        std::cout<<"}\n";
    }

    void init_tmp_degree() {
        tmp_degree = neighbors.size();
    }

    int get_color() const{
        return color;
    };

    int get_weight() const{
        return weight;
    };

    bool compare_degrees(int degree)
    {
        std::shared_lock<std::shared_mutex> lock(m);
        return tmp_degree <= degree;
    }

    void decrease_tmp_degree()
    {
        --tmp_degree;
    }

    void set_weight(int _weight)
    {
        weight = _weight;
    }

    void self_remove()
    {
        for (auto &n : neighbors)
        {
            std::scoped_lock<std::shared_mutex> lock(m);
            n->decrease_tmp_degree();
        }
    }

    static bool compare_colors(std::shared_ptr<Vertex>& v1, std::shared_ptr<Vertex>& v2) {
        return v1->get_color()<v2->get_color();
    }
};



#endif //PDS_PROJECT_SMART_VERTEX_H
