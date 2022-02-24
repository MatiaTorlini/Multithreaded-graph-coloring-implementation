//
// Created by Goro on 23/02/2022.
//

#ifndef PROGRAM_SEQUENTIAL_GREEDY_COLORING_H
#define PROGRAM_SEQUENTIAL_GREEDY_COLORING_H

#include <random>
#include <algorithm>
#include <queue>
#include "Graph.h"

class Sequential_Greedy_Coloring {
    std::vector<Vertex*> V;
    Graph& G;
    unsigned seed;
public:
    Sequential_Greedy_Coloring(Graph& g):G(g){};
    void start() try {

        //CREATE VECTOR PERMUTATION
        V = G.vector_to_shuffle();
        seed = std::chrono::system_clock::now().time_since_epoch().count();
        std::shuffle(V.begin(), V.end(), std::default_random_engine(seed));

        //ITERATE AND ASSIGN COLORS
        for (long int idx = 0; idx<G.get_size(); idx++) {
            auto &v = *V[idx];
            G.color(idx);
        }
    }
    catch (const std::exception& e){
        std::cout<<"Something went wrong - exception raised\n";
    }
};

#endif //PROGRAM_SEQUENTIAL_GREEDY_COLORING_H
