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

using namespace std;

class Smallest_Degree_Last{
private:
    vector<thread> threads;
    condition_variable cv;
    mutex m;
    Graph& graph;
    int n_threads;
    atomic<int> waiting_threads;
    vector<list<long int>> fragments_to_weight;
    vector<list<long int>> fragments_to_remove;
    atomic<int> unassigned_vertices;
    atomic<bool> processed_flag;
    atomic<int> current_weight;
    atomic<int> current_degree;

    void split_indexes()
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

public:
    Smallest_Degree_Last(Graph& _graph, int _n_threads) : graph(_graph), n_threads(_n_threads)
    {
        fragments_to_weight.resize(n_threads); //resize initializes lists, reserve just allocates memory (sets capacity, not size)
        fragments_to_remove.resize(n_threads);
        processed_flag.store(false);
        waiting_threads.store(0);
        current_weight.store(0);
        current_degree.store(0);
        waiting_threads = n_threads;
        unassigned_vertices.store(graph.get_size());
        split_indexes();
    };

    int start() {
        waiting_threads = n_threads;
        for(int i = 0; i < n_threads; i++)
            threads.emplace_back(thread([this,i](){thread_task(i);}));
        for(thread& t : threads)
            if(t.joinable())
                t.join();
        return 0;
    }

    bool is_last_thread()
    {
        return waiting_threads.fetch_sub(1) == 1;
    }

    void thread_task(int thread_id)
    {
        while(unassigned_vertices > 0)
        {
            thread_weight(thread_id);
            {
                unique_lock<mutex> lock(m);
                if(!is_last_thread())
                    cv.wait(lock);
                else
                {
                    if(processed_flag.load())
                        ++current_weight;
                    else
                        ++current_degree;
                    waiting_threads = n_threads;
                    cv.notify_all();
                }
            }

            if(processed_flag.load())
            {
                thread_remove(thread_id);
                {
                    unique_lock<mutex> lock(m);
                    if(!is_last_thread())
                        cv.wait(lock);
                    else
                    {
                        processed_flag.store(false);
                        waiting_threads = n_threads;
                        cv.notify_all();
                    }
                }
            }
        }
    }

    void thread_weight(int thread_id)
    {
        for(auto itr = fragments_to_weight[thread_id].cbegin(); itr != fragments_to_weight[thread_id].cend(); itr++)
        {
            if (graph.try_to_weight(*itr, current_degree.load(), current_weight.load()))
            {
                processed_flag.store(true);
                --unassigned_vertices;
                fragments_to_remove[thread_id].push_back(*itr);
                fragments_to_weight[thread_id].erase(itr--);
            }
        }
    }

    void thread_remove(int thread_id)
    {
        for(long int& id:fragments_to_remove[thread_id])
            graph.remove(id);
        fragments_to_remove[thread_id].clear();
    }
};

#endif //JONES_PLASSMAN_H_SMALLEST_DEGREE_LAST_H

