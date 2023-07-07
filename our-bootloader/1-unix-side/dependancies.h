#ifndef GRAPH_H_
#define GRAPH_H_


typedef struct Node {
    int vertex;
    char *fileName;
    int status;
    struct Node **children;
    struct Node **parents;
    struct Node *next_job;
    struct Node *previous_job;
    int numChildren;
    int numParents;
    int dependancies;
} Node;

typedef struct graph {
    int numVertices;
    int numAvailableJobs;
    Node **adjLists;
    Node *availableJobsStart;
    Node *availableJobsEnd;
} Graph;

enum DepType {
    CHILD,
    PARENT
};

Node *createNode(int v, char *file);

Graph *createGraph();

Node *grabAvailableJob(Graph *);

void addNode(Graph *graph, int v, char *file);

void printEntireJobList(Graph *graph);

int findNodeInGraph(Graph* graph, char* filename);

void addDep(Node *first, Node *second, int isParent);

void addEdge(Graph *graph, char* parent, char *child);

void markNodeInProgress(Graph *graph, int job);

void markNodeAsFinished(Graph *graph, int job);

int isJobAvailable(Graph *graph);

void addJobToQueue(Node **, Node *);

void notifyChilden(Graph *graph, Node *parent);

void printGraph(Graph *graph);

void setUpGraph(Graph *graph);

#endif /* GRAPH_H_ */
