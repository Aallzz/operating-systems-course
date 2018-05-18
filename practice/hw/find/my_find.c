#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <getopt.h>

#include "lib/my_utility.h"

find_flags* flags;
list *files_list;
char* start_path;

int main(int argc, char* argv[]) {
  if (argc < 1) {
    printf("Usage: my_find path opts");
    return 0;
  }
  flags = make_find_flags();
  start_path = argv[1];
  parse_opt_args(flags, argc, argv);
  files_list = make_list();
  find_dfs(start_path, files_list); 
  filter_files_list(files_list, flags);
  apply_to_files_list(files_list, flags);
  print_list(files_list, stdout);
  delete_list(files_list);
  return 0;
}
