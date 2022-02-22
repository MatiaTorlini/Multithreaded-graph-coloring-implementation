//
// Created by rares on 19/02/2022.
//

#ifndef JONES_PLASSMAN_H_SMALLEST_DEGREE_LAST_H
#define JONES_PLASSMAN_H_SMALLEST_DEGREE_LAST_H

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
    vector<list<long int>> fragments_to_weight;
    vector<list<long int>> fragments_to_remove;
    atomic<bool> phase;
    atomic<bool> processed_flag;
    atomic<int> current_weight;
    atomic<int> current_degree;

public:
    Smallest_Degree_Last(Graph& _graph, int _n_threads) : graph(_graph), n_threads(_n_threads)
    {
        init();
    };

    void init()
    {
        fragments_to_weight.resize(n_threads); //resize initializes lists
        fragments_to_remove.resize(n_threads);
        processed_flag.store(false);
        phase.store(false);
        current_weight.store(0);
        current_degree.store(0);
    }

    void split_indexes()
    {
        if(graph.get_size() < n_threads)
            n_threads = graph.get_size()-1;
        else
        {
            int fragment_size = graph.get_size()/n_threads;
            int j=0;
            for(int i=0; i<n_threads-1; i++)
            {
                while(j < fragment_size*(i+1))
                {
                    fragments_to_weight[i].push_back(j);
                    ++j;
                }
            }
            while(j < graph.get_size())
            {
                fragments_to_weight[n_threads - 1].push_back(j);
                ++j;
            }
        }
    }

    int start() {

        cout<<"Weighting...\n";

        init();
        split_indexes();

        auto on_phase_completion = [this]() noexcept {
            if(!phase.load()) //Just weighted
            {
                if (processed_flag.load()) //At least one vertex was weighted
                {
                    ++current_weight;
                    phase.store(true); //Now it's time to remove
                }
                else //No vertices were weighted
                    ++current_degree; //Keep weighting
            }
            else //Just removed
            {
                processed_flag.store(false);
                phase.store(false); //Now it's time to weight
            }
        };

        std::barrier barrier(n_threads, on_phase_completion);

        auto thread_task = [this, &barrier] (int thread_id)
        {
            while(!fragments_to_weight[thread_id].empty()) {

                thread_weight(thread_id);

                if (!fragments_to_weight[thread_id].empty())
                {
                    barrier.arrive_and_wait();
                    if(phase.load())
                    {
                        thread_remove(thread_id);
                        barrier.arrive_and_wait();
                    }
                }
                else
                {
                    barrier.arrive_and_drop();
                }
            }
        };

        for(int i = 0; i < n_threads; i++)
            threads.emplace_back(thread([&thread_task, i](){thread_task(i);}));


        for(thread& t : threads)
            if(t.joinable())
                t.join();
        cout<<"Weighted! Max weight: "<<current_weight<<"\n";
        return 0;
    }

    void thread_weight(int thread_id)
    {
        for(auto itr = fragments_to_weight[thread_id].cbegin(); itr != fragments_to_weight[thread_id].cend(); itr++)
        {
            if (graph.try_to_weight(*itr, current_degree.load(), current_weight.load()))
            {
                processed_flag.store(true);
                fragments_to_remove[thread_id].push_back(*itr);
                fragments_to_weight[thread_id].erase(itr--);
            }
        }
    }

    void thread_remove(int thread_id)
    {
        if(!fragments_to_remove[thread_id].empty())
        {
            for(auto itr = fragments_to_remove[thread_id].cbegin(); itr != fragments_to_remove[thread_id].cend(); itr++)
            {
                graph.remove(*itr);
                fragments_to_remove[thread_id].erase(itr--);
            }
        }
    }
};

#endif //JONES_PLASSMAN_H_SMALLEST_DEGREE_LAST_H

