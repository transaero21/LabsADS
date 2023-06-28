#ifndef GRAPH_H
#define GRAPH_H

enum TYPE {
    ENTER = 1,
    EXIT = 2,
    TRANSITION = 3
};

typedef struct Node {
    void *obj;
    struct Node *next;
} Node;

typedef struct Vertex {
    char *id;
    enum TYPE type;
    Node *edge;
} Vertex;

typedef struct Edge {
    Vertex *vertex;
    int weight;
} Edge;

typedef struct Graph {
    int size;
    Vertex **vertices;
} Graph;

#define DOT_LEGEND \
    "\t{ rank = sink;\n" \
    "\t\tLegend [shape=none, margin=0, label=<\n" \
    "\t\t<TABLE BORDER=\"0\" CELLBORDER=\"1\" CELLSPACING=\"0\" CELLPADDING=\"4\">\n" \
    "\t\t\t<TR>\n"  \
    "\t\t\t\t<TD COLSPAN=\"2\"><B>Legend</B></TD>\n" \
    "\t\t\t</TR>\n"  \
    "\t\t\t<TR>\n" \
    "\t\t\t\t<TD>Enter</TD>\n" \
    "\t\t\t\t<TD BGCOLOR=\"RED\"></TD>\n" \
    "\t\t\t</TR>\n" \
    "\t\t\t<TR>\n" \
    "\t\t\t\t<TD>Exit</TD>\n" \
    "\t\t\t\t<TD BGCOLOR=\"GREEN\"></TD>\n" \
    "\t\t\t</TR>\n"\
    "\t\t\t<TR>\n" \
    "\t\t\t\t<TD>Transition</TD>\n" \
    "\t\t\t\t<TD BGCOLOR=\"BLUE\"></TD>\n" \
    "\t\t\t</TR>\n" \
    "\t\t</TABLE>\n" \
    "\t>];\n" \
    "\t}\n"

#define DOT_FILE "/tmp/graph.dot"
#define PNG_FILE "/tmp/graph.png"

#define DOT_CMD ("sfdp -x -Goverlap=scale -Tpng " DOT_FILE " -o " PNG_FILE)
#define IMG_CMD ("catimg " PNG_FILE)

Graph *initGraph();
void destroyGraph(Graph *graph);

int insertVertex(Graph *graph, const char *id, enum TYPE type);
int deleteVertex(Graph *graph, const char *id);
int updateVertex(Graph *graph, const char *id, enum TYPE nType);

int insertEdge(Graph *graph, const char *from, const char *to, int weight);
int deleteEdge(Graph *graph, const char *from, const char *to);
int updateEdge(Graph *graph, const char *from, const char *to, int nWeight);

void printAdjacency(Graph *graph);
void printPicture(Graph *graph);

enum STATUS {
    NOT_FOUND = -1,
    REACHABLE = 0,
    NOT_REACHABLE = 1
};

typedef struct EData {
    int from;
    Vertex *to;
    int weight;
} EData;

enum STATUS reachabilityCheck(Graph *graph, const char *from);
Vertex **shortestPath(Graph *graph, const char *from, const char *to);
void minimumSpanningTree(Graph *graph);

#define SEP " \t\r\n\v\f"

void import(Graph *graph, FILE *file);
void dump(Graph *graph, FILE *file);

#endif //GRAPH_H
