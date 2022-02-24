//
// Created by Goro on 24/02/2022.
//

#ifndef PROGRAM_RANDOM_ASSIGNER_H
#define PROGRAM_RANDOM_ASSIGNER_H

#include <random>

class Random_assigner {
    unsigned long seed;
    Graph &G;
    long start;
    long stop;
    int i;
public:
    Random_assigner(Graph& g, long s, long t, int id):G(g),start(s),stop(t),i(id){};
    void launch() {
        seed = start^stop<<i;
        std::mt19937 rand_gen(seed);
        for (long j=start; j<stop; j++)
            G.assign_random(j,rand_gen());
    }
};

#endif //PROGRAM_RANDOM_ASSIGNER_H
