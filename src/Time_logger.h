//
// Created by Goro on 21/02/2022.
//

#ifndef PROGRAM_TIME_LOGGER_H
#define PROGRAM_TIME_LOGGER_H

#include <chrono>

class TimeMeasurement {
    std::chrono::time_point<std::chrono::high_resolution_clock> start;
    std::chrono::duration<double, std::milli> execution_time;
public :
    void start_TimeMeasurement(){
        start = std::chrono::high_resolution_clock::now();
    };

    void stop_and_print_TimeMeasurement() {
        execution_time =std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - start);
        std::cout<<"Execution time : "<<execution_time.count()<<" milliseconds\n";
    }
    double get_exec_time() {
        return execution_time.count();
    }
};

#endif //PROGRAM_TIME_LOGGER_H
