//
// Created by Goro on 21/02/2022.
//

#ifndef PDS_PROJECT_SMART_LOADER_H
#define PDS_PROJECT_SMART_LOADER_H

#include <fstream>
#include <thread>
#include <atomic>
#include <mutex>
#include <iostream>
#include "Graph.h"

#define INPUT_PATH "../test/"

static std::atomic<long> shared_cnt;
static std::mutex m;

class safe_reader {

    std::string delimiter;
    std::string line;
    long int cnt;
    int pos;
    std::string token;
    std::ifstream& input;
    Graph &G;

    bool is_number(const std::string& s)
    {
        std::string::const_iterator it = s.begin();
        while (it != s.end() && std::isdigit(*it)) ++it;
        return !s.empty() && it == s.end();
    }

    bool get_line() {
        std::scoped_lock<std::mutex> lock(m);
        if (std::getline(input, line, '\n')) {
            cnt = shared_cnt.fetch_add(1);
            return true;
        }
        else {
            shared_cnt.store(0);
            return false;
        }
    }

public:
    safe_reader(std::ifstream& in, Graph& g):delimiter(" "), input(in), G(g){};
    void safe_read() {
        while(get_line()) {
            pos = 0;
            while (pos != std::string::npos) {
                pos = line.find(delimiter);
                token = line.substr(0, pos);
                if (is_number(token)) try {
                        G.add_neighbor(cnt, stoi(token)-1);
                    }
                    catch(std::exception& e) {
                        std::cout<<"exception safe reader\n";
                        std::cerr << e.what();
                    }
                line = line.substr(pos + 1, line.length());
            }
            G.init_tmp_degree(cnt);
        }
    }
};

class Loader{
    std::string& filename;
    int n_threads;
    std::string graph_size;
    std::vector<std::thread> threads;
    Graph& G;
public:
    Loader(Graph& g, std::string& fin, int n) : G(g), filename(fin), n_threads(n){};

    int start() try {
        std::ifstream input(INPUT_PATH + filename, std::ifstream::in);
        if (!input.is_open())
            return -2;
        std::cout<<"Building the graph...\n";
        //READ GRAPH SIZE
        std::getline(input, graph_size, '\n');
        int pos = graph_size.find_first_of(" ");
        graph_size = graph_size.substr(0,pos);
        G.set_size(stoi(graph_size));

        //init graph
        for (int i=0;i<stoi(graph_size);i++) {
            Vertex* v = new Vertex(i);
            std::shared_ptr<Vertex> ptr(v);
            G.add_vertex(ptr);
        }

        //LAUNCH READERS
        for(int i=0;i<n_threads;i++)
            threads.emplace_back(std::thread([this, &input](){
                safe_reader sf(input, G);
                sf.safe_read();
            }));
        for (auto& t : threads)
            if(t.joinable())
                t.join();
        std::cout<<"Done!\n";
        return 1;
    }
    catch(std::exception& e) {
        std::cout<<e.what();
        return -1;
    }
};

#endif //PDS_PROJECT_SMART_LOADER_H
