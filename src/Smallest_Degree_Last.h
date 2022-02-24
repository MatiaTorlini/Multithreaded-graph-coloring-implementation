//
// Created by Goro on 21/02/2022.
//

#ifndef PROGRAM_SMALLEST_DEGREE_LAST_H
#define PROGRAM_SMALLEST_DEGREE_LAST_H


#include "thread"
#include "vector"
#include "Graph.h"
#include "mutex"
#include "condition_variable"
#include "cassert"
#include <barrier>

using namespace std;

class Smallest_Degree_Last{
private:
    vector<thread> threads;
    Graph& graph;
    int n_threads;

    atomic<bool> processed_flag;
    atomic<int> current_weight;
    atomic<int> current_degree;

    std::vector<std::list<long>> to_remove;

    std::pair<long,long> split_indexes(long int size, int thread_id, int n) {

        int gap = size/n;
        long start;
        long stop;

        start = thread_id*(gap);
        if (thread_id==n_threads-1)
            stop = size;
        else
            stop = (thread_id+1)*(gap);
        return std::make_pair(start,stop);
    }

public:
    Smallest_Degree_Last(Graph& _graph, int _n_threads) : graph(_graph), n_threads(_n_threads)
    {
        init();
    };

    void init()
    {
        to_remove.resize(n_threads);
        processed_flag.store(false);
        current_weight.store(0);
        current_degree.store(0);
    }

    int start() {

        cout<<"Weighting...\n";
        //std::cout<<"is big "<<graph.get_size()<<"\n";

        init();

        auto on_phase_completion = [this]() noexcept {
                if (processed_flag.load()) //At least one vertex was weighted
                    ++current_weight;
                else //No vertices were weighted
                    ++current_degree; //Keep weighting
             //just removed
                processed_flag.store(false);
                for (auto& list : to_remove) {
                    for (auto& v : list)
                        graph.remove_sdl(v);
                    list.clear();
                }

        };

        std::barrier sync_point(n_threads, on_phase_completion);

        auto thread_task = [this, &sync_point] (int thread_id)
        {
            std::pair<long,long> indexes = split_indexes(graph.get_size(), thread_id, n_threads);
            long n_unweighted = indexes.second - indexes.first;
            Smart_index si(indexes.first,indexes.second);

            while(n_unweighted) {
                thread_weight(n_unweighted, si, to_remove[thread_id]);
                sync_point.arrive_and_wait();
            }
            sync_point.arrive_and_drop();
        };

        for(int i = 0; i < n_threads; i++)
            threads.emplace_back(thread([&thread_task, i](){
                thread_task(i);
            }));


        for(thread& t : threads)
            if(t.joinable())
                t.join();
        cout<<"Weighted! Max weight: "<<current_weight<<"\n";
        std::cout<<"-----------------------------------------------------------------------------"<<"\n";

        return 0;
    }

    void thread_weight(long& unweighted, Smart_index &idx, std::list<long>& _to_remove)
    {
        long j;
        for (long k=0;k<unweighted;k++) {
            j = idx.get_idx();
            if (graph.try_to_weight(j, current_degree.load(), current_weight.load())) {
                processed_flag.store(true);
                _to_remove.push_back(j);
                unweighted--;
                idx.update(true);
            } else
                idx.update(false);
        }
    }
};

#endif //PROGRAM_SMALLEST_DEGREE_LAST_H
