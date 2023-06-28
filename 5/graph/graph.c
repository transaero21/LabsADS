#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#include "graph.h"
#include "../queue/queue.h"
#include "../include/utils.h"

Graph *initGraph() {
    return calloc(1, sizeof(Graph));
}

void destroyGraph(Graph *graph) {
    for (int i = 0; i < graph->size; i++) {
        for (Node *node = graph->vertices[i]->edge, *next; node; node = next) {
            next = node->next;
            free(node->obj), free(node);
        }
    }
    for (int i = 0; i < graph->size; i++) {
        free(graph->vertices[i]->id), free(graph->vertices[i]);
    }
    free(graph->vertices);
    free(graph);
}

Vertex *initVertex(const char *id, enum TYPE type) {
    Vertex *vertex = calloc(1, sizeof(Vertex));
    vertex->id = strdup(id);
    vertex->type = type;
    return vertex;
}

Edge *initEdge(Vertex *vertex, int weight) {
    Edge *edge = calloc(1, sizeof(Edge));
    edge->vertex = vertex;
    edge->weight = weight;
    return edge;
}

Node *initNode(Edge *edge, Node *next) {
    Node *node = calloc(1, sizeof(Node));
    node->obj = edge;
    node->next = next;
    return node;
}

EData initEData(Edge *edge, int from) {
    EData eData = {.weight = edge->weight, .from = from, .to = edge->vertex};
    return eData;
}

int getVertexIndexById(Graph *graph, const char *id, int *i) {
    int exists = 0;
    for (*i = 0; *i < graph->size; (*i)++) {
        if ((exists = !strcmp(graph->vertices[*i]->id, id))) break;
    }
    return exists;
}

int getVertexIndex(Graph *graph, Vertex *vertex) {
    int i = 0;
    for (; i < graph->size; i++) {
        if (graph->vertices[i] == vertex) break;
    }
    return i;
}

char *getTypeName(enum TYPE type) {
    switch (type) {
        case ENTER: return "Enter";
        case EXIT: return "Exit";
        case TRANSITION: return "Transition";
    }
}

char *getTypeColor(enum TYPE type) {
    switch (type) {
        case ENTER: return "red";
        case EXIT: return "green";
        case TRANSITION: return "blue";
    }
}

int getTypeValue(char *string) {
    toLower(string);
    if (!strcmp(string, "enter")) return ENTER;
    if (!strcmp(string, "exit")) return EXIT;
    if (!strcmp(string, "transition")) return TRANSITION;
    return -1;
}

int insertVertex(Graph *graph, const char *id, enum TYPE type) {
    for (int i = 0; i < graph->size; i++) {
        if (!strcmp(graph->vertices[i]->id, id)) return 0;
    }

    graph->vertices = realloc(graph->vertices, ++graph->size * sizeof(Vertex *));
    graph->vertices[graph->size - 1] = initVertex(id, type);
    return 1;
}

int insertEdge(Graph *graph, const char *from, const char *to, int weight) {
    int iFrom, iTo;
    if (!(getVertexIndexById(graph, from, &iFrom) && getVertexIndexById(graph, to, &iTo))) return 0;
    if (iFrom == iTo) return 0;

    Node *node = graph->vertices[iFrom]->edge;
    for (; node; node = node->next) {
        if (!strcmp(((Edge *) node->obj)->vertex->id, to)) break;
    }
    if (node) return 0;

    Edge *edge = initEdge(graph->vertices[iTo], weight);
    graph->vertices[iFrom]->edge = initNode(edge, graph->vertices[iFrom]->edge);
    return 1;
}

int deleteVertex(Graph *graph, const char *id) {
    int i;
    if (!getVertexIndexById(graph, id, &i)) return 0;

    for (int j = 0; j < graph->size; j++) {
        for (Node *node = graph->vertices[j]->edge, *prev = NULL; node;) {
            Edge *edge = node->obj;
            if (!strcmp(edge->vertex->id, id)) {
                if (prev) prev->next = node->next;
                else graph->vertices[j]->edge = node->next;
                free(edge), free(node);
                if (prev) node = prev->next;
                else node = graph->vertices[j]->edge;
            } else {
                prev = node, node = node->next;
            }
        }

    }

    for (Node *node = graph->vertices[i]->edge, *next; node; node = next) {
        next = node->next;
        free(node->obj), free(node);
    }
    free(graph->vertices[i]->id), free(graph->vertices[i]);
    for (int j = i + 1; j < graph->size; ++j) {
        graph->vertices[j - 1] = graph->vertices[j];
    }
    graph->size--;

    return 1;
}

int deleteEdge(Graph *graph, const char *from, const char *to) {
    int i;
    if (!(getVertexIndexById(graph, from, &i))) return 0;

    for (Node *node = graph->vertices[i]->edge, *prev = NULL; node; prev = node, node = node->next) {
        Edge *edge = node->obj;
        if (!strcmp(edge->vertex->id, to)) {
            if (prev) prev->next = node->next;
            else graph->vertices[i]->edge = node->next;
            free(edge), free(node);
            return 1;
        }
    }
    return 0;
}

int updateVertex(Graph *graph, const char *id, enum TYPE nType) {
    int i;
    if (!getVertexIndexById(graph, id, &i)) return 0;
    graph->vertices[i]->type = nType;
    return 1;
}

int updateEdge(Graph *graph, const char *from, const char *to, int nWeight) {
    int i;
    if (!(getVertexIndexById(graph, from, &i))) return 0;

    for (Node *node = graph->vertices[i]->edge; node; node = node->next) {
        Edge *edge = node->obj;
        if (!strcmp(edge->vertex->id, to)) {
            edge->weight = nWeight;
            return 1;
        }
    }
    return 0;
}

void printAdjacency(Graph *graph) {
    for (int i = 0; i < graph->size; i++) {
        Vertex *vertex = graph->vertices[i];
        printf("%s (%s): ", vertex->id, getTypeName(vertex->type));
        for (Node *node = vertex->edge; node; node = node->next) {
            Edge *edge = node->obj;
            printf("%s (%d)", edge->vertex->id, edge->weight);
            if (node->next) printf(" -> ");
        }
        printf("\n");
    }
}

void printPicture(Graph *graph) {
    if (system("which dot > /dev/null 2>&1")) {
        printf("Failed to print: \"dot\" not found!\n");
        return;
    }
    FILE *file = fopen(DOT_FILE, "w");
    if (!file) {
        printf("Failed to print: failed to create file!\n");
        return;
    }

    fprintf(file, "digraph picture {\n");
    fprintf(file, "\tnode [shape=ellipse]\n");
    fprintf(file, DOT_LEGEND);
    for (int i = 0; i < graph->size; i++) {
        Vertex *vertex = graph->vertices[i];
        fprintf(file, "\t\"%s\" [color=\"%s\"]\n", vertex->id, getTypeColor(vertex->type));
        for (Node *node = vertex->edge; node; node = node->next) {
            Edge *edge = node->obj;
            fprintf(file, "\t\"%s\" -> \"%s\" [label=%d]\n", vertex->id, edge->vertex->id, edge->weight);
        }
    }

    fprintf(file, "}\n");
    fflush(file);
    system(DOT_CMD);
    if (!system("which catimg > /dev/null 2>&1")) system(IMG_CMD);
    else printf("Failed to print: \"catimg\" not found!\n");
    fclose(file);
}

enum STATUS reachabilityCheck(Graph *graph, const char *from) {
    int i = 0, isOk = 0;
    if (getVertexIndexById(graph, from, &i) && graph->vertices[i]->type != ENTER) return NOT_FOUND;

    Queue *queue = calloc(1, sizeof(Queue *));
    push(queue, graph->vertices[i]);
    int *visited = calloc(graph->size, sizeof(int));
    visited[i] = 1;

    while (!isEmpty(queue)) {
        Vertex *current = pop(queue);
        Node *node = current->edge;
        while (node) {
            Edge *edge = node->obj;
            if (edge->vertex->type == EXIT) {
                isOk = 1;
                break;
            }
            int j = getVertexIndex(graph, edge->vertex);
            if (!visited[j]) {
                visited[j] = 1;
                push(queue, edge->vertex);
            }
            node = node->next;
        }
        if (isOk) break;
    }

    while (!isEmpty(queue)) pop(queue);
    free(visited), free(queue);
    return isOk ? REACHABLE : NOT_REACHABLE;
}

void pushFront(Vertex ***array, Vertex *vertex) {
    int i = 0;
    if (*array) while ((*array)[i]) i++;
    *array = realloc(*array, (i + 2) * sizeof(Node *));
    for (int j = i; j >= 1; j--) (*array)[j] = (*array)[j - 1];
    (*array)[0] = vertex;
    (*array)[i + 1] = NULL;
}

Vertex **shortestPath(Graph *graph, const char *from, const char *to) {
    int iFrom, iTo;
    if (!(getVertexIndexById(graph, from, &iFrom) && getVertexIndexById(graph, to, &iTo))) return NULL;

    int dist[graph->size + 1], prev[graph->size + 1];
    for (int i = 0; i < graph->size + 1; i++) dist[i] = INT_MAX, prev[i] = -1;
    dist[iFrom] = 0;

    for (int i = 0; i < graph->size - 1; i++) {
        for (int j = 0; j < graph->size; j++) {
            Vertex *vertex = graph->vertices[j];
            for (Node *node = vertex->edge; node; node = node->next) {
                Edge *edge = node->obj;
                int k = getVertexIndex(graph, edge->vertex);
                if (dist[j] != INT_MAX && dist[k] > dist[j] + edge->weight) {
                    dist[k] = dist[j] + edge->weight;
                    prev[k] = j;
                }
            }
        }
    }

    if (dist[iTo] == INT_MAX) return NULL;

    Vertex **array = NULL;
    pushFront(&array, graph->vertices[iTo]);
    while (iTo != iFrom) {
        pushFront(&array, graph->vertices[prev[iTo]]);
        iTo = prev[iTo];
    }

    return array;
}

void dfs(Graph *graph, int from, int *visited) {
    visited[from] = 1;
    Node *node = graph->vertices[from]->edge;
    while (node != NULL) {
        Edge *edge = (Edge*) node->obj;
        int nextVertex = getVertexIndex(graph, edge->vertex);
        if (!visited[nextVertex]) dfs(graph, nextVertex, visited);
        node = node->next;
    }
}

int verifyGraph(Graph *graph) {
    int isOk = 0;

    for (int i = 0; i < graph->size; ++i) {
        if (graph->vertices[i]->type != ENTER) continue;
        int *visited = calloc(graph->size, sizeof(int));
        dfs(graph, i, visited);
        for (int j = 0; j < graph->size; j++) {
            if (!visited[j] && graph->vertices[j]->type == EXIT) {
                isOk++;
                break;
            }
        }
        free(visited);
    }

    return isOk;
}

int compareEdges(const void *a, const void *b) {
    return ((EData *) b)->weight - ((EData *) a)->weight;
}

void minimumSpanningTree(Graph *graph) {
    EData *array = NULL;
    int size = 0;

    for (int i = 0; i < graph->size; i++) {
        Vertex *vertex = graph->vertices[i];
        for (Node *node = vertex->edge; node; node = node->next) {
            EData new = initEData((Edge *) node->obj, i);
            array = realloc(array, ++size * sizeof(EData));
            array[size - 1] = new;
        }
    }
    qsort(array, size, sizeof(EData), compareEdges);

    for (int i = 0; i < size; i++) {
        for (Node *node = graph->vertices[array[i].from]->edge, *prev = NULL; node; prev = node, node = node->next) {
            if (((Edge *) node->obj)->vertex == array[i].to) {
                int oldRet = verifyGraph(graph);
                if (prev) prev->next = node->next;
                else graph->vertices[array[i].from]->edge = node->next;
                if (verifyGraph(graph) != oldRet) {
                    if (prev) prev->next = node;
                    else graph->vertices[array[i].from]->edge = node;
                } else free(node->obj), free(node);
                break;
            }
        }
    }
    free(array);
}

char **splitLine(char *line, int *size) {
    char **pieces = NULL;

    char *piece = strtok(line, SEP);
    while (piece) {
        pieces = realloc(pieces, ++(*size) * sizeof(char *));
        pieces[*size - 1] = piece;
        piece = strtok(NULL, SEP);
    }

    return pieces;
}

void import(Graph *graph, FILE *file) {
    int size = 0, i, weight;

    for (char *line; (line = readLine(file)); free(line), size = 0) {
        char **pieces = splitLine(line, &size);
        if (size < 1) continue;
        toLower(pieces[0]);
        if (!strtok(pieces[0], "vertex") && size == 3) {
            i = getTypeValue(pieces[size - 1]);
            if (i > 0) insertVertex(graph, pieces[1], (enum TYPE) i);
        } else if (!strtok(pieces[0], "edge") && size == 4) {
            i = stringToInt(pieces[size - 1], &weight);
            if (i) insertEdge(graph, pieces[1], pieces[2], weight);
        }
        free(pieces);
    }
}

void dump(Graph *graph, FILE *file) {
    for (int i = 0; i < graph->size; i++) {
        Vertex *vertex = graph->vertices[i];
        fprintf(file, "vertex %s %s\n", vertex->id, getTypeName(vertex->type));
    }
    fprintf(file, "\n");
    for (int i = 0; i < graph->size; i++) {
        Vertex *vertex = graph->vertices[i];
        for (Node *node = vertex->edge; node; node = node->next) {
            Edge *edge = node->obj;
            fprintf(file, "edge %s %s %d\n", vertex->id, edge->vertex->id, edge->weight);
        }
    }
}
