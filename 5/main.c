#include <stdio.h>

#include "graph/graph.h"
#include "include/menu.h"

int main() {
    Graph *graph = initGraph();

    if (graph) {
        initMenu(graph);
        destroyGraph(graph);
    }
    printf("\nGoodbye...\n");

    return 0;
}
