#include <iostream>
#include "Loader.h"
#include "Graph.h"

int main() {

    Graph G;
    std::string fin = "test.graph";
    Loader loader(G,fin,2);
    loader.start();
    G.print_graph();

    return 0;
}
