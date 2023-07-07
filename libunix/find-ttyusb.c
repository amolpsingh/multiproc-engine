// engler, cs140e: your code to find the tty-usb device on your laptop.
#include <assert.h>
#include <fcntl.h>
#include <string.h>
#include "libunix.h"

#define _SVID_SOURCE
#include <dirent.h>
#include <sys/stat.h>
static const char *ttyusb_prefixes[] = {
	"ttyUSB",	// linux
    "ttyACM",   // linux
	// "cu.SLAB_USB", // mac os
    "cu.usbserial", // mac os
    // if your system uses another name, add it.
	0
};

static int filter(const struct dirent *d) {
    // scan through the prefixes, returning 1 when you find a match.
    // 0 if there is no match.
    const char **t = ttyusb_prefixes;
    while (*t != 0) {
        //todo: how do u use dirent?
        if (strncmp(*t, (d)->d_name, strlen(*t)) == 0) {
            return 1;
        }
        t++;
    }
    return 0;
}

// find the TTY-usb device (if any) by using <scandir> to search for
// a device with a prefix given by <ttyusb_prefixes> in /dev
// returns:
//  - device name.
// error: panic's if 0 or more than 1 devices.
char *find_ttyusb(void) {
    // use <alphasort> in <scandir>
    // return a malloc'd name so doesn't corrupt.
    struct dirent **namelist;
    char dev[100] = "/dev/";
    int n = scandir(dev, &namelist, filter, alphasort);
    if (n == 0) {
       perror("No devices found!");
    }
    else if (n > 1) {
        perror("Multiple devices found!");
    }
    else if (n < 1) {
        perror("Directory can't be opened or not enough memory available!");
    }
    //todo: is this correct for namelist?
    char* final_dirc = strcat(dev, (*namelist)->d_name);
    char* name = strdup(final_dirc);
    
    // FILE *fp = open(final_dirc, );
    return name;
}

int lastModSort(const struct dirent **e1, const struct dirent **e2) {
    //todo: double deref?
    char dev1[100] = "/dev/";
    char dev2[100] = "/dev/";
    const char *a = (*e1)->d_name;
    const char *b = (*e2)->d_name;
    //do i need to append dev?
    strcat(dev1, a);
    strcat(dev2, b);
    struct stat stA;
    struct stat stB;
    stat(dev1, &stA);
    stat(dev2, &stB);
    //not handling case of equaltimes
    return stA.st_mtime > stB.st_mtime;
}
// return the most recently mounted ttyusb (the one
// mounted last).  use the modification time 
// returned by state.
char *find_ttyusb_last(void) {
    struct dirent **namelist;
    char dev[100] = "/dev/";
    int n = scandir(dev, &namelist, filter, lastModSort);
    if (n == 0) {
       perror("No devices found!");
    }
    else if (n < 1) {
        perror("Directory can't be opened or not enough memory available!");
    }
    //todo: is this correct for namelist?
    char* final_dirc = strcat(dev, (*namelist)->d_name);
    char* name = strdup(final_dirc);
    // FILE *fp = fopen(final_dirc, "rw");
    return name;
}
int firstModSort(const struct dirent **e1, const struct dirent **e2) {
    char dev1[100] = "/dev/";
    char dev2[100] = "/dev/";
    //todo: double deref?
    const char *a = (*e1)->d_name;
    const char *b = (*e2)->d_name;
    //do i need to append dev?
    strcat(dev1, a);
    strcat(dev2, b);
    struct stat stA;
    struct stat stB;
    stat(dev1, &stA);
    stat(dev2, &stB);
    //not handling case of equaltimes
    return stA.st_mtime < stB.st_mtime;
}
// return the oldest mounted ttyusb (the one mounted
// "first") --- use the modification returned by
// stat()
char *find_ttyusb_first(void) {
    struct dirent **namelist;
    char dev[100] = "/dev/";
    int n = scandir(dev, &namelist, filter, firstModSort);
    if (n == 0) {
       perror("No devices found!");
    }
    else if (n < 1) {
        perror("Directory can't be opened or not enough memory available!");
    }
    //todo: is this correct for namelist?
    char* final_dirc = strcat(dev, (*namelist)->d_name);
    char* name = strdup(final_dirc);
    // char* final_dirc = strcat(dev, name);
    // FILE *fp = fopen(final_dirc, "rw");
    return name;
}

char **find_ttyusb_last_n(int n) {
    char **ret_arr = malloc(n * sizeof(char *));
    for (int i = 0; i < n; i++) {
        char dev[100] = "/dev/";
        struct dirent **namelist;
        int num_devs = scandir(dev, &namelist, filter, lastModSort);
        if (num_devs == 0) {
            perror("No devices found!");
            break;
        }
        if (i >= num_devs) {
            perror("Not enough devices!");
            break;
        }

        char *final_dirc = strcat(dev, namelist[i]->d_name);
        char *name = strdup(final_dirc);
        printf("USB FOUND: %s\n", name);
        ret_arr[i] = name;
        free(namelist[i]);
        free(namelist);
    }
    return ret_arr;
}
