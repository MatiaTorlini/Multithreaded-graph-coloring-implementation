//
// Created by Goro on 21/02/2022.
//

#ifndef PROGRAM_SMART_INDEX_H
#define PROGRAM_SMART_INDEX_H

#include <vector>
#include <iostream>

class Smart_index {

    long int idx;
    long int size;
    long int start;
    std::vector<long int> next;
    std::vector<long int> prev;
public:
    Smart_index(long int start_index, long int end_index) {
        size = end_index-start_index;
        start = start_index;
        next.reserve(size);
        prev.reserve(size);
        for(long int i=start_index;i<end_index;i++) {
            if (i!=end_index-1)
                next.emplace_back(i+1);
            else
                next.emplace_back(start_index);
            if (i!=start_index)
                prev.emplace_back(i-1);
            else
                prev.emplace_back(end_index-1);
        }
        idx=start_index;
    }

    long int get_idx()  {
        return idx;
    }

    void update(bool processed) {
        long int target = idx-start;
        if (processed) {
            next[prev[target]-start] = next[target];
            prev[next[target]-start] = prev[target];
        }
        idx = next[target];
    }


    void print_support_table() {
        std::cout<<"\n";
        for (auto& n : next)
            std::cout<<n<<" ";
        std::cout<<"\n";
        for (auto& p : prev)
            std::cout<<p<<" ";
        std::cout<<"\n\n";
    }
};

#endif //PROGRAM_SMART_INDEX_H
