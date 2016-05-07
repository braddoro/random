// Pick random values

#include <time.h>
#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>
#include <limits.h>
#include <sys/stat.h>
#include <sys/types.h>

#define PACKAGE "raval"
#define VERSION "0.0.1"

/* status epilepticus, print help and exit with `exval' */
void print_help(int exval);
/* picks a radom value withing range low-high*/
int get_rand_val(int low, int high);
/* ! definitely not cryptographic secure ! */
/* value returned seeds the rand() function */
unsigned time_seed(void);

int main(int argc, char *argv[]) {
 int opt = 0;          /* holds option */
 int max = 10;         /* upper limit */
 int min = 0;          /* minimum limit */
 int many = 1;         /* the number of numbers to pick ... */
 char *ptr = NULL;     /* separator for number range */

 while((opt = getopt(argc, argv, "hvn:r:")) != -1) {
  switch(opt) {
   case 'h':
    print_help(0);
    break;
   case 'v':
   exit(0);
   case 'n':
    many = atoi(optarg);
    break;
   case 'r':
    if((ptr = strchr(optarg, ':')) == NULL) {
     fprintf(stderr, "%s: Error - range `LOW:HIGH'\n\n", PACKAGE);
     print_help(1);
    } else {
     ptr++, max = atoi(ptr);
     ptr--, ptr = '\0';
     min = atoi(optarg);
     if(min >= max || min < 0 || max < 0) {
      fprintf(stderr, "%s: Error - range `LOW:HIGH'\n\n", PACKAGE);
      print_help(1);
     }
    }
    break;
   case '?':
    fprintf(stderr, "%s: Error - No such option: `%c'\n\n", PACKAGE, optopt);
    print_help(1);
   case ':':
    fprintf(stderr, "%s: Error - option `%c' needs an argument\n\n", PACKAGE, optopt);
    print_help(1);
  }
 }

 /* first seed the random function */
 srand((time_seed()));

 /* print the random values */
 for(; many > 0; many--)
  printf("%4d\n", get_rand_val(min, max));

 return 0;
}

/* picks a radom value withing range low-high*/
int get_rand_val(int low, int high) {
 int k = 0;
 double d = 0;

 d = (double)rand() / ((double)RAND_MAX + 1);
 k = (int)(d * (high - low + 1));

 return(low + k);
}

/* ! definitely not cryptographic secure ! */
/* value returned seeds the rand() function */
unsigned time_seed(void) {
 int retval = 0;
 int fd;

 /* just in case open() fails.. */
 if(open("/dev/urandom", O_RDONLY) == -1) {
  retval = (((int)time(NULL)) & ((1 << 30) - 1)) + getpid();
 } else {
  read(fd, &retval, 4);
  /* positive values only */
  retval = abs(retval) + getpid();
  close(fd);
 }

 return retval;
}

void print_help(int exval) {
 printf("%s,%s print a random number\n", PACKAGE, VERSION);
 printf("Usage: %s OPTION...\n\n", PACKAGE);

 printf(" -h         print this help and exit\n");
 printf(" -v         print version and exit\n\n");

 printf(" -n INT     return `INT' numbers\n");
 printf(" -r INT:INT keep the number within range `LOW:HIGH',\n");
 printf("            default=(1:10)\n\n");


 exit(exval);
}
