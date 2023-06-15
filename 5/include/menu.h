#ifndef MENU_H
#define MENU_H

#include "../graph/graph.h"

#define MENU_OPTIONS                         \
    "Menu options\n"                         \
    "\n"                                     \
    "┌────────────────────────────────┐\n"   \
    "│ 1. Insert vertex in graph      │\n"   \
    "├────────────────────────────────┤\n"   \
    "│ 2. Insert edge in graph        │\n"   \
    "├────────────────────────────────┤\n"   \
    "│ 3. Delete vertex from graph    │\n"   \
    "├────────────────────────────────┤\n"   \
    "│ 4. Delete edge from graph      │\n"   \
    "├────────────────────────────────┤\n"   \
    "│ 5. Update vertex info          │\n"   \
    "├────────────────────────────────┤\n"   \
    "│ 6. Update edge info            │\n"   \
    "├────────────────────────────────┤\n"   \
    "│ 7. Print adjacency list        │\n"   \
    "├────────────────────────────────┤\n"   \
    "│ 8. Print graph as picture      │\n"   \
    "├────────────────────────────────┤\n"   \
    "│ 9. Reachability check          │\n"   \
    "├────────────────────────────────┤\n"   \
    "│ 10. Find shortest path         │\n"   \
    "├────────────────────────────────┤\n"   \
    "│ 11. Minimum spanning tree      │\n"   \
    "├────────────────────────────────┤\n"   \
    "│ 12. Exit program               │\n"   \
    "└────────────────────────────────┘\n"   \
    "\n"                                     \
    "Type your option from 1 to 12, then press ENTER: "

void initMenu(Graph *graph);

int mInsertVertex(Graph *graph);
int mInsertEdge(Graph *graph);
int mDeleteVertex(Graph *graph);
int mDeleteEdge(Graph *graph);
int mUpdateVertex(Graph *graph);
int mUpdateEdge(Graph *graph);
int mPrintAdjacency(Graph *graph);
int mPrintPicture(Graph *graph);
int mReachabilityCheck(Graph *graph);
int mShortestPath(Graph *graph);
int mMinimumSpanningTree(Graph *graph);
int mExit();

static int (*opts[])(Graph *) = {
        mInsertVertex,
        mInsertEdge,
        mDeleteVertex,
        mDeleteEdge,
        mUpdateVertex,
        mUpdateEdge,
        mPrintAdjacency,
        mPrintPicture,
        mReachabilityCheck,
        mShortestPath,
        mMinimumSpanningTree,
        mExit
};

static int isMenuOption(int opt);
static int isTypeOption(int type);

#endif //MENU_H
