#include <iostream>
#include "Loader.h"
#include "Graph.h"
#include "Smallest_Degree_Last.h"

int main() {

    Graph G;
    std::string fin = "rgg_n_2_15_s0.graph";
    Loader loader(G,fin,2);
    loader.start();
    //G.print_graph();

    std::cout<<"\n";
    Smallest_Degree_Last sdl(G, 2);

    sdl.start();
    //G.print_graph();
    //G.check_weights();
    return 0;
}
