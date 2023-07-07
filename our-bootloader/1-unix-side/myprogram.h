#ifndef __MYPROGRAM__
#define __MYPROGRAM__

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdarg.h>
#include <string.h>
#include <sys/stat.h>
#include <termios.h>
#include "libunix.h"
#include <sys/types.h> 
#include <sys/wait.h> 
#include "simple-boot.h"
#include "dependancies.h"

//our version of my-install
void send_to_pi(uint32_t pi_num, char *filename, char **dev_names);

// returns 1 if empty and 0 if not empty
uint32_t check_pi_done(uint32_t pi_num);

//parse a txt file to get a dependency graph
Graph *convert_txt_dependency_graph(char *fileName);


#endif
