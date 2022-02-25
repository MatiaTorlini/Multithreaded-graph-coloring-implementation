//
// Created by Goro on 22/02/2022.
//

#ifndef PROGRAM_JONES_PLASSMAN_H
#define PROGRAM_JONES_PLASSMAN_H

#include <barrier>
#include "Smart_index.h"
#include "Smallest_Degree_Last.h"
#include "Random_assigner.h"


class Jones_Plassman {
private:
    Graph& graph;
    std::vector<std::pair<long int, long int>> indexes;
    int n_threads;
    std::vector<std::thread> threads;
    std::vector<std::list<long int>> to_remove;
    int mode;

    void split_indexes(long int size, std::vector<std::pair<long int,long int>> &_indexes, int n) {
        int gap = size/n;
        long int j=0;
        for (int i=0;i<n;i++) {
            _indexes.emplace_back(j, j + gap);
            j += gap;
        }
        _indexes[n - 1].second = size;
    }

public:
    Jones_Plassman(Graph& g): graph(g){};

    void start(int n, int md) {

        //INIT VARIABLES
        n_threads = n;
        mode = md;
        threads.reserve(n_threads);
        to_remove.resize(n_threads);

        //PARTITION GRAPH
        split_indexes(graph.get_size(), indexes, n_threads); //define data chunks for each thread

        //DEFINE FUNCTION TO BE PERFORMED WHEN A CYCLE IS COMPLETED
        auto update_graph = [this]() noexcept {
            for(auto &list : to_remove) {
                for(auto& elem : list)
                    graph.remove_jp(elem);
                list.clear();
            }
        };

        //DEFINE BARRIER SYNC POINT
        std::barrier sync_point1(n_threads, update_graph);
        std::barrier sync_point0(n_threads);



        if (mode==2) {
            Smallest_Degree_Last SDL(graph, n_threads);
            SDL.start();
        }

        //LAUNCH THREADS
        for (int i=0;i<n_threads;i++)
            threads.emplace_back(std::thread([this, i, &sync_point1, &sync_point0](){

                long start = indexes[i].first;
                long stop = indexes[i].second;
                long int chunk_size = stop-start;
                long int uncolored = chunk_size;
                long int j;
                Smart_index idx(start,stop);

                {
                    Random_assigner AR(graph, start, stop, i);
                    AR.launch();
                }
                sync_point0.arrive_and_wait();

                while(uncolored) {
                    for (long k=0;k<uncolored;k++) {
                        j = idx.get_idx();
                        if (mode == 1) {
                            if (graph.has_biggest_random(j)) {
                                graph.color(j);
                                to_remove[i].push_back(j);
                                uncolored--;
                                idx.update(true);
                            }
                            else
                                idx.update(false);
                        }
                        else if (mode == 2){
                            if (graph.has_biggest_weight(j)) {
                                graph.color(j);
                                to_remove[i].push_back(j);
                                uncolored--;
                                idx.update(true);
                            }
                            else
                                idx.update(false);
                        }
                        else if (mode == 3) {
                            if (graph.has_biggest_degree(j)) {
                                graph.color(j);
                                to_remove[i].push_back(j);
                                uncolored--;
                                idx.update(true);
                            }
                            else
                                idx.update(false);
                        }
                    }
                    sync_point1.arrive_and_wait();
                }
                sync_point1.arrive_and_drop();
            }));

        //JOIN THREADS
        for (auto& t:threads)
            if (t.joinable())
                t.join();
    }
};


#endif //PROGRAM_JONES_PLASSMAN_H
