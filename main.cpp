#include <iostream>
#include "Loader.h"
#include "Graph.h"
#include "Smallest_Degree_Last.h"

int main() {

    Graph G;
    std::string fin = "test.graph";
    Loader loader(G,fin,2);
    loader.start();
    G.print_graph();
    Smallest_Degree_Last sdl(G, 2);
    sdl.start();
    std::cout<<"\n";
    G.print_graph();
    return 0;
}
