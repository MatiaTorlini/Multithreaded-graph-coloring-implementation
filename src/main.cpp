#include <iostream>

#include "Loader.h"
#include "Graph.h"
#include "Time_logger.h"
#include "Jones_Plassman.h"
#include "Sequential_greedy_coloring.h"
#include "Config.h"

void print_menu() {
    std::cout<<"-----------------------------------------------------------------------------"<<"\n";
    std::cout<<"Graph_coloring V0.8 offers different algorithms to solve the graph coloring\n"
               "problem on a given input graph. It supports files in format .txt and .graph.\n";
    std::cout<<"-----------------------------------------------------------------------------"<<"\n";
    std::cout<<"Synopsis"
               "[input_graph] [algorithm] [number_of_threads]\n\n";
    std::cout<<"  [input_graph] : filename (nb graph file must be in ./test folder; however, it\n"
                                  "is possible to change the inputs and outputs folder in Config.h);\n"
               "  [algorithm]:\n"
               "       0 for sequential greedy;\n"
               "       1 for Random heuristic ordering Jones-Plassman;\n"
               "       2 for Smalles-degree-last heuristic ordering Jones-Plassman;\n"
               "  [number_of_threads]: an integer number\n"
               "  (required with algorithms 1 and 2 only)\n";

    std::cout<<"-----------------------------------------------------------------------------"<<"\n";
    std::cout<<"Developed by Torlini Matia and Lorenzo Semeraro\n"
               "Politechnic univeristy of Turin, 2021.\n";
    std::cout<<"-----------------------------------------------------------------------------"<<"\n";

}

int main() {

    std::cout<<"-----------------------------------------------------------------------------"<<"\n";
    std::cout<<"                          Graph_coloring V0.8\n";
    std::cout<<"-----------------------------------------------------------------------------"<<"\n";
    std::cout<<"(? help)\n";

        std::string cmd = "rgg_n_2_15_s0.graph";
        std::string output_file;
        std::string input_file;
        int res = 0;

        int mode = 0;
        int n_threads = 0;

        TimeMeasurement tm;
        Graph G;

        Sequential_Greedy_Coloring SGC(G);
        Jones_Plassman JP(G);


    while(true) {
            std::cin >> input_file;
            if (input_file == "help") {
                print_menu();
                continue;
            }
            else {
                Loader loader(G, input_file, 4);
                res = loader.start();
                output_file = OUTPUT_FILE_PATH + input_file;
                if (!res)
                    break;

                std::cin>>mode;

                if (!mode) {
                    tm.start_TimeMeasurement();
                    SGC.start();
                    tm.stop_and_print_TimeMeasurement();
                }
                else {
                    std::cin>>n_threads;
                    tm.start_TimeMeasurement();
                    JP.start(n_threads,mode);
                    tm.stop_and_print_TimeMeasurement();
                }
                break;
            }
        }

        if(mode)
            std::cout<<"Number of threads used : "<<n_threads<<"\n";

        if(res) {
            G.print_n_colors();
            G.print_vertices_to_file(output_file);
            std::cout<<"You want to check the coloring? (Y/N)\n";
            std::cin >> input_file;
            if (input_file == "Y")
                G.check_coloring();
            std::cout<<"You can find the colors assigned in the output folder\n";
        }

    std::cout<<"-----------------------------------------------------------------------------"<<"\n";
    std::cout<<"-----------------------------------------------------------------------------"<<"\n";

    return 0;
}
