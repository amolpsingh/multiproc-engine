#include "myprogram.h"
#define MAX_FILENAME_LEN 100
#define NUM_PIS 2 // ONLY EDIT HERE IF WANT TO CHANGE NUMBER OF PIS
#define SPACE " "
int trace_p = 0; 
pid_t pi_pids[NUM_PIS];


int main(int argc, char *argv[]) {

    if(argc < 2){
        printf("Sorry, please make sure to feed in a file with dependancies");
        return 0;
    }

    for (int i = 0; i < NUM_PIS; i++) {
        pi_pids[i] = -1;
    }
    char *filename = argv[1];

    Graph *graph = convert_txt_dependency_graph(filename);

    setUpGraph(graph);
    printEntireJobList(graph);
    int arr_pis[NUM_PIS];

    for (int i = 0; i < NUM_PIS; i++) {
        arr_pis[i] = -1;
    }

    char **dev_names = find_ttyusb_last_n(NUM_PIS);

    uint32_t num_programs_to_run = graph->numVertices; 
    printf("num vertices:%d\n", num_programs_to_run);
    uint32_t num_programs_sent = 0;
    uint32_t num_programs_done = 0;
    printf("3\n");
    while (num_programs_done < num_programs_to_run) {
        for (int i = 0; i < NUM_PIS; i++) {
            // printf("i=%d\n", i);
            if(check_pi_done(i) || pi_pids[i] == -1) {
                int vertex = -1;
                vertex = arr_pis[i];
                if (vertex != -1) {
                    printf("Marking job as finished: %d!\n", vertex);
                    markNodeAsFinished(graph, vertex);
                    num_programs_done++;
                    printf("num_programs_done=%d\n", num_programs_done);
                }
                if(isJobAvailable(graph)){ //do a check here to see if there is a program available to send over. 
                    Node *job = grabAvailableJob(graph);
                    send_to_pi(i, job->fileName, dev_names); //put a check here to see if there is even any jobs to do. 
                    markNodeInProgress(graph, job->vertex);
                    num_programs_sent++;
                    printf("num_programs_sent=%d\n", num_programs_sent);
                    arr_pis[i] = job->vertex;
                }
            } 
        } 
    }  
    printf("13\n");
    return 0;  
}

// basically my-install
void send_to_pi(uint32_t pi_num, char *filename, char **dev_names) {
    char *dev_name = dev_names[pi_num];
    pid_t child_pid  = fork();
    if (child_pid != 0) {
        printf("In the Parent\n");
        pi_pids[pi_num] = child_pid;
        return;
    }
    int tty = open_tty(dev_name);
    if(tty < 0)
        panic("can't open tty <%s>\n", dev_name);
    unsigned baud_rate = B115200;
    double timeout_tenths = 2*5;
    int fd = set_tty_to_8n1(tty, baud_rate, timeout_tenths);
    if(fd < 0)
        panic("could not set tty: <%s>\n", dev_name);

    // 3. read in program [probably should just make a <file_size>
    //    call and then shard out the pieces].
    unsigned nbytes;
    uint8_t *code = read_file(&nbytes, filename);

    // 4. let's send it!
    debug_output(" tty-usb=<%s> program=<%s>: about to boot\n", 
                 dev_name, filename);

    if (child_pid == 0) {
        simple_boot(fd, code, nbytes);
        pi_echo(0, fd, dev_name);
        exit(0); //should never get here.
    }
}

// returns 1 if done and 0 if not done
uint32_t check_pi_done(uint32_t pi_num) {
    int status;
    waitpid(pi_pids[pi_num], &status, WNOHANG);
    if (WIFEXITED(status)) {
        return 1;
    }
    return 0;
}

Graph *convert_txt_dependency_graph(char *fileName) {
  FILE *file = fopen(fileName, "r");
  
  if (file == NULL) {
    printf("Error: File not found.\n");
    return NULL;
  }
    
  char buffer[1000];
  char *token;
  char *filename;
  char *dependency;
  char *dependencyList;
  int numDependencies;
  
  // Map to store the dependencies
  char **map[MAX_FILENAME_LEN];
  int numFiles = 0;

  Graph *graph = createGraph();
  // Read each line of the file
  while (fgets(buffer, 1000, file) != NULL) {
    // Split the line into tokens
    filename = strtok(buffer, ":");
    addNode(graph, graph->numVertices, filename);

    dependencyList = strtok(NULL, ":");
    // printf("Dependancy:%s\n", dependencyList);
    fflush(stdout);
    dependencyList++;
    int len = strlen(dependencyList);
    /* get the first token */
    dependency = strtok(dependencyList, ", \n");
    /* walk through other tokens */
    
    while(dependency != NULL) {
        if (strcmp(dependency, "[]") == 0) break;
        // printf( "Here:%s\n", dependency);
        addNode(graph, graph->numVertices, dependency);
        addEdge(graph, dependency, filename);
        dependency = strtok(NULL, ", \n");
    }
  }
  fclose(file);
  printf("1");
  return graph;
}

