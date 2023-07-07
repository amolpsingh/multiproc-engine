#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dependancies.h>


Node *createNode(int v, char *file)
{
    Node *newNode = malloc(sizeof(Node));
    newNode->vertex = v;
    newNode->fileName = malloc(strlen(file) + 1);
    strcpy(newNode->fileName, file);
    newNode->status = 0;

    newNode->children = NULL;
    newNode->parents = NULL;
    newNode->previous_job = NULL;
    newNode->next_job = NULL;
    newNode->dependancies = 0;
    return newNode;
}

Graph *createGraph()
{
    Graph *graph = malloc(sizeof(Graph));
    graph->numVertices = 0;
    graph->numAvailableJobs = 0;
    graph->adjLists = NULL;
    graph->availableJobsStart = NULL;
    graph->availableJobsEnd = NULL;
    return graph;
}

void addNode(Graph *graph, int v, char *file) {
    // Create the new node
    if(findNodeInGraph(graph, file) != -1) return; //already have node;
    Node *newNode = createNode(v, file);

    // Allocate memory for the new adjacency list
    Node **newAdjList = (Node **) realloc(graph->adjLists, (graph->numVertices + 1) * sizeof(Node *));

    if (newAdjList == NULL) {
        printf("Error: could not allocate memory for new adjacency list.\n");
        return;
    }

    // Update the graph's adjacency list and node count
    graph->adjLists = newAdjList;
    graph->adjLists[graph->numVertices] = newNode;
    printf("file:%s\n", file);
    graph->numVertices++;
    printf("numvertices:%d\n", graph->numVertices);
    
}

int findNodeInGraph(Graph* graph, char* filename) {
    printf("Looking to see if file %s is already present\n", filename);
    for (int i = 0; i < graph->numVertices; i++) {
        printf("Looking at: %s\n",graph->adjLists[i]->fileName);
        if (strcmp(graph->adjLists[i]->fileName, filename) == 0) {
            return i;
        }
    }
    return -1;
}


void addJobToQueue(Node **start, Node *newJob){
    
    newJob->next_job = NULL;

    if (*start == NULL) {
        newJob->previous_job = NULL;
        *start = newJob;
        printf("Adding this job to queue %s.\n", (*start)->fileName);
        return;
    }

    Node *lastNode = *start;
    printf("\tThese jobs are in the queue already: %s\n.", lastNode->fileName);
    while (lastNode->next_job != NULL) {
        lastNode = lastNode->next_job;
        printf("\tThese jobs are in the queue already: %s\n.", lastNode->fileName);
    }
    
    lastNode->next_job = newJob;
    newJob->previous_job = lastNode;
    printf("Adding this job to queue %s.\n", newJob->fileName);
}

void setUpGraph(Graph *graph){
    for(int i=0; i< graph->numVertices; i++){
        Node *node = graph->adjLists[i];
        printf("This file %s, has this many (%d) dependancies\n", node->fileName, node->numParents);
        if(node->numParents == 0){
            addJobToQueue(&(graph->availableJobsStart), node);
            graph->numAvailableJobs++;
        }
    }
}

void printEntireJobList(Graph *graph){

    Node *curr = graph->availableJobsStart;
    printf("here's the job list!\n");
    while(curr != NULL){
        printf("\t Job: %s", curr->fileName);
        curr = curr->next_job;
    }
    printf("Done!\n");
}

void removeJobFromQueue(Graph *graph, Node *node){
    if (node == NULL) {
            return;
    } 
    if (node->previous_job != NULL) {
        node->previous_job->next_job = node->next_job;
    }
    if (node->next_job != NULL) {
        node->next_job->previous_job = node->previous_job;
        graph->availableJobsStart = node->next_job;
    } else{
        graph->availableJobsStart = NULL;
    }
    graph->numAvailableJobs--;
    printEntireJobList(graph);
}

void addDep(Node *first, Node *second, int isParent){

    return;
}

void addEdge(Graph *graph, char* parent, char *child){

    Node *n_parent = NULL;
    Node *n_child = NULL;
    for(int i = 0; i < graph->numVertices; i++){
        if(strcmp(graph->adjLists[i]->fileName, parent) == 0 ){
            n_parent = graph->adjLists[i];
        }  
        if(strcmp(graph->adjLists[i]->fileName, child) == 0){
            n_child = graph->adjLists[i];
        } 
        if(n_parent != NULL && n_child != NULL) break;
    }

        Node **temp = n_child->parents;
        n_child->parents = (Node **)malloc((n_child->numParents + 1) * sizeof(Node *));
        for (int i = 0; i < n_child->numParents; i++)
        {
            n_child->parents[i] = temp[i];
        }
        free(temp);

        n_child->parents[n_child->numParents] = n_parent;
        printf("Incrementing this file's parents: %s\n", n_child->fileName);
        n_child->numParents++;
        n_child->dependancies++; //make sure to add dependancies if adding child

        temp = n_parent->children;
        n_parent->children = (Node **)malloc((n_parent->numChildren + 1) * sizeof(Node *));
        for (int i = 0; i < n_parent->numChildren; i++)
        {
            n_parent->children[i] = temp[i];
        }
        free(temp);
        n_parent->children[n_parent->numChildren] = n_child;
        n_parent->numChildren++;
}

void markNodeInProgress(Graph *graph, int job)
{
    Node *node = graph->adjLists[job];
    node->status = 1;
    removeJobFromQueue(graph, node);
}

void markNodeAsFinished(Graph *graph, int job)
{
    Node *node = graph->adjLists[job];
    node->status = 2;
    notifyChilden(graph, node);
}

int isJobAvailable(Graph *graph){ 
    return graph->availableJobsStart != NULL;
}

Node *grabAvailableJob(Graph *graph){
    if(!isJobAvailable(graph)) return NULL;
    printf("About to give you this job: %s\n", graph->availableJobsStart->fileName);
    return graph->availableJobsStart;
}


void notifyChilden(Graph *graph, Node *parent){
    for(int i = 0; i < parent->numChildren; i++){
        parent->children[i]->dependancies--;
        if(parent->children[i]->dependancies == 0){
            addJobToQueue(&(graph->availableJobsStart), parent->children[i]);
        }
    }
}


void printGraph(Graph *graph)
{
    printf("Printing Graph:\n");
    for (int i = 0; i < graph->numVertices; i++)
    {
        Node *node = graph->adjLists[i];
        printf("%d (%s) --> ", node->vertex, node->fileName);
        node = node->next_job;
        while (node != NULL)
        {
            printf("%d (%s) ", node->vertex, node->fileName);
            node = node->next_job;
        }
        printf("\n");
    }
}
