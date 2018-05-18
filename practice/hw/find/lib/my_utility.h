#ifndef MY_FIND_UTILITY
#define MY_FIND_UTILITY 

#include <getopt.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <dirent.h>
#include <libgen.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>

#include "shelllib/my_utility.h"
#include "my_list.h"

#define MY_FIND_MAX_BUFFER_SIZE 1024

typedef struct find_flags {
  int current_flags;
  int inum_arg, size_arg, nlinks_arg;
  char *name_arg, *path_arg;
} find_flags;

static const int inum_flag      =   1;
static const int name_flag      =   2;
static const int size_flag      =   4;
static const int nlinks_flag    =   8;
static const int exec_flag      =   16;
static const int size_less_flag =   32;
static const int size_grt_flag  =   64;
static const int size_eq_flag   =   128;

static char const * find_opt = "i:n:s:l:e:";

static const struct option find_opt_long[] = 
{
  {"inum",    required_argument,  NULL,   'i'},
  {"name",    required_argument,  NULL,   'n'},
  {"size",    required_argument,  NULL,   's'},
  {"nlinks",  required_argument,  NULL,   'l'},
  {"exec",    required_argument,  NULL,   'e'},
  {NULL,      0,                  NULL,   0}
};

find_flags* make_find_flags();
void parse_opt_args(find_flags* flags, int argc, char* argv[]);
void find_dfs(const char* dir, list* files_list);
void filter_files_list(list* files_list, find_flags* flags);
void apply_to_files_list(const list* files_list, find_flags* flags);
#endif
