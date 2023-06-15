#include <stdio.h>
#include <stdlib.h>

#include "include/menu.h"
#include "include/utils.h"

void initMenu(Graph *graph) {
    int opt, ret;

    insertVertex(graph, "node1", (enum TYPE) 1);
    insertVertex(graph, "node2", (enum TYPE) 2);
    insertVertex(graph, "node3", (enum TYPE) 3);
    insertVertex(graph, "node4", (enum TYPE) 1);
    insertVertex(graph, "node5", (enum TYPE) 3);
    insertVertex(graph, "node6", (enum TYPE) 3);
    insertVertex(graph, "node7", (enum TYPE) 1);

    insertEdge(graph, "node1", "node2", 8);
    insertEdge(graph, "node1", "node4", 4);
    insertEdge(graph, "node2", "node7", 7);
    insertEdge(graph, "node2", "node1", 7);
    insertEdge(graph, "node3", "node1", 5);
    insertEdge(graph, "node2", "node5", 7);
    insertEdge(graph, "node4", "node1", 4);
    insertEdge(graph, "node5", "node1", 7);
    insertEdge(graph, "node2", "node3", 8);
    insertEdge(graph, "node2", "node6", 1);
    insertEdge(graph, "node6", "node1", 1);

    do {
        system("clear");
        printf(MENU_OPTIONS);
        ret = readInt(&opt, isMenuOption);
        if (ret) {
            system("clear");
            ret = opts[opt - 1](graph);
            if (ret) pressEnterToContinue();
        }
    } while (ret);
}

int mInsertVertex(Graph *graph) {
    int ret = 0, type;
    char *id = NULL;

    printf("Enter id of new vertex: ");
    if ((id = readLine(stdin))) {
        printf("Enter type of new vertex (1 - Enter, 2 - Exit, 3 - Transition): ");
        if (readInt(&type, isTypeOption)) {
            ret = insertVertex(graph, id, (enum TYPE) type);
            if (ret) printf("\nInserted successfully!\n");
            else printf("\nVertex with this id already exists!\n");
            ret = 1;
        }
        free(id);
    }

    return ret;
}

int mInsertEdge(Graph *graph) {
    char *from = NULL, *to = NULL;
    int ret = 0, weight = 0;

    printf("Enter id of a vertex to start with: ");
    if ((from = readLine(stdin))) {
        printf("Enter id of a vertex on which to end: ");
        if ((to = readLine(stdin))) {
            printf("Enter weight of new edge: ");
            if (readInt(&weight, NULL)) {
                ret = insertEdge(graph, from, to, weight);
                if (ret) printf("\nInserted successfully!\n");
                else printf("\nEdge already exists or vertex with such ids doesn't exists!\n");
                ret = 1;
            }
        }
    }
    free(from), free(to);

    return ret;
}

int mDeleteVertex(Graph *graph) {
    char *id = NULL;
    int ret = 0;

    printf("Enter id of a vertex to delete: ");
    if ((id = readLine(stdin))) {
        ret = deleteVertex(graph, id);
        if (ret) printf("\nDeleted successfully!\n");
        else printf("\nThere is no vertex with such id\n");
        ret = 1;
    }
    free(id);

    return ret;
}

int mDeleteEdge(Graph *graph) {
    char *from = NULL, *to = NULL;
    int ret = 0;

    printf("Enter id of a vertex to start with: ");
    if ((from = readLine(stdin))) {
        printf("Enter id of a vertex on which to end: ");
        if ((to = readLine(stdin))) {
            ret = deleteEdge(graph, from, to);
            if (ret) printf("\nDeleted successfully!\n");
            else printf("\nEdge or vertex with such ids doesn't exists!\n");
            ret = 1;
        }
    }
    free(from), free(to);

    return ret;
}

int mUpdateVertex(Graph *graph) {
    int ret = 0, type;
    char *id = NULL;

    printf("Enter id of a vertex to update: ");
    if ((id = readLine(stdin))) {
        printf("Enter type of vertex (1 - Enter, 2 - Exit, 3 - Transition): ");
        if (readInt(&type, isTypeOption)) {
            ret = updateVertex(graph, id, type);
            if (ret) printf("\nUpdated successfully!\n");
            else printf("\nThere is no vertex with such id\n");
            ret = 1;
        }
    }
    free(id);

    return ret;
}

int mUpdateEdge(Graph *graph) {
    char *from = NULL, *to = NULL;
    int ret = 0, weight = 0;

    printf("Enter id of a vertex to start with: ");
    if ((from = readLine(stdin))) {
        printf("Enter id of a vertex on which to end: ");
        if ((to = readLine(stdin))) {
            printf("Enter a new weight of edge: ");
            if (readInt(&weight, NULL)) {
                ret = updateEdge(graph, from, to, weight);
                if (ret) printf("\nUpdated successfully!\n");
                else printf("\nEdge or vertex with such ids doesn't exist!\n");
                ret = 1;
            }
        }
    }
    free(from), free(to);

    return ret;
}

int mPrintAdjacency(Graph *graph) {
    printAdjacency(graph);
    return 1;
}

int mPrintPicture(Graph *graph) {
    printPicture(graph);
    return 1;
}

int mReachabilityCheck(Graph *graph) {
    char *from = NULL;
    int ret = 0;

    printf("Enter id of a vertex to start with: ");
    if ((from = readLine(stdin))) {
        switch (reachabilityCheck(graph, from)) {
            case NOT_FOUND:
                printf("\nVertex is not an enter or doesn't exist!\n");
                break;
            case REACHABLE:
                printf("\nEnter is reachable to the exit!\n");
                break;
            case NOT_REACHABLE:
                printf("\nEnter is not reachable to the exit!\n");
                break;
        }
        ret = 1;
    }

    return ret;
}

int mShortestPath(Graph *graph) {
    char *from = NULL, *to = NULL;
    int ret = 0;

    printf("Enter id of a vertex to start with: ");
    if ((from = readLine(stdin))) {
        printf("Enter id of a vertex on which to end: ");
        if ((to = readLine(stdin))) {
            Vertex **array = shortestPath(graph, from, to);
            if (array) {
                printf("\nThe following route were found:\n");
                for (int i = 0; array[i]; i++) {
                    if (i) printf(" -> ");
                    printf("%s", array[i]->id);
                }
                printf("\n");
            } else printf("\nNo path was found\n");
            free(array), ret = 1;
        }
    }
    free(from), free(to);

    return ret;
}

int mExit() {
    return 0;
}

static int isMenuOption(int opt) {
    return 1 <= opt && opt <= 11;
}

static int isTypeOption(int type) {
    return 1 <= type && type <= 3;
}
