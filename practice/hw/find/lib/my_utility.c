#include "my_utility.h"

#define true 1
#define false 0

find_flags* make_find_flags() {
  find_flags* flags = (find_flags*)malloc(sizeof(find_flags));
  flags->current_flags = 0;
  return flags;
};

void parse_opt_args(find_flags* flags, int argc, char* argv[]) {
  int opt_char;
  
  flags->current_flags = 0;
  while ((opt_char = getopt_long(argc, argv, find_opt, find_opt_long, NULL)) != -1) {
    switch (opt_char) {
      case 'i': flags->current_flags |= inum_flag;
                flags->inum_arg = atoi(optarg);
                break;
      case 'n': flags->current_flags |= name_flag;
                flags->name_arg = strdup(optarg);
                break;
      case 's': flags->current_flags |= size_flag;
                flags->size_arg = atoi(optarg);
                switch (optarg[0]) {
                  case '+': flags->current_flags |= size_less_flag;
                            break;
                  case '-': flags->current_flags |= size_grt_flag;
                            break;
                  case '=': flags->current_flags |= size_eq_flag;
                };
                break;
      case 'l': flags->current_flags |= nlinks_flag;
                flags->nlinks_arg = atoi(optarg);
                break;
      case 'e': flags->current_flags |= exec_flag;
                flags->path_arg = strdup(optarg);
                break;
      default: abort(); 
    }
  }
}

void find_dfs(const char* path, list* files_list) {
  struct stat path_stat;
  struct dirent *entry;
  DIR *dp;
  char *subpath;
  if (stat(path, &path_stat) != -1) {
    if (S_ISREG(path_stat.st_mode)) {
      push_front_list(files_list, path);
    } else {
      if ((dp = opendir(path)) == NULL) {
        fprintf(stderr, "Cannot open directory during find: %s\n", strerror(errno));
        return ;
      }
      while ((entry = readdir(dp)) != NULL) {
        if (strcmp(".", entry->d_name) == 0 ||
            strcmp("..", entry->d_name) == 0) {
          continue;
        }
        subpath = (char*)malloc(strlen(path) + strlen(entry->d_name) + 2);
        *subpath = '\0';
        strcpy(subpath, path);
        strcat(subpath, "/");
        strcat(subpath, entry->d_name);
        find_dfs(subpath, files_list);
        free(subpath);
      }
      closedir(dp);
    }
  } else {
    printf("Error during find operation with path \" %s \": %s", path, strerror(errno));
    return ; 
  }
};

int validate_by_inum(const char* path, find_flags* arg) {
  struct stat cur_stat;
  stat(path, &cur_stat);
  return cur_stat.st_ino == arg->inum_arg;
};

int validate_by_size(const char* path, find_flags* arg) {
  struct stat cur_stat;
  stat(path, &cur_stat);
  if (arg->current_flags & size_less_flag) return cur_stat.st_size < arg->size_arg;
  if (arg->current_flags & size_grt_flag) return cur_stat.st_size > arg->size_arg;
  if (arg->current_flags & size_eq_flag) return cur_stat.st_size == arg->size_arg;
  return false;
}

int validate_by_nlinks(const char* path, find_flags* arg) {
  struct stat cur_stat;
  stat(path, &cur_stat);
  return cur_stat.st_nlink == arg->nlinks_arg;
}

int validate_by_name(const char* path, find_flags* arg) {
  struct stat cur_stat;
  stat(path, &cur_stat);
  char* path_copy = strdup(path);
  char* file_name = basename(path_copy);
  int res = strcmp(file_name, arg->name_arg);
  free(path_copy);
  return res == 0;
}

void filter_files_list(list* lst, find_flags* flags) {
  if (flags->current_flags & inum_flag) {
    filter_list(lst, flags, validate_by_inum);
  }
  if (flags->current_flags & name_flag) {
    filter_list(lst, flags, validate_by_name);
  }
  if (flags->current_flags & size_flag) {                 /// equal
    filter_list(lst, flags, validate_by_size); 
  }
  if (flags->current_flags & nlinks_flag) {
    filter_list(lst, flags, validate_by_nlinks); 
  }
}

void apply_to_path(const char* path, const char* exec) {
  struct string_buffer buffer = create_buffer();
  int line_sz;
  char **arguments;
 

  line_sz = 0;
  arguments = NULL;
  buffer.buf = (char*)malloc((strlen(path) + strlen(exec) + 2)*sizeof(char));
  buffer.buf[0] = '\0';
  strcat(buffer.buf, exec);
  strcat(buffer.buf, " ");
  strcat(buffer.buf, path);
  split_buffer(&buffer, &arguments, &line_sz);

  pid_t pid = fork();
  
  if (pid < 0) {
    return ;
  }
   
  if (pid == 0) {
    if (execvp(arguments[0], arguments) < 0) {
      perror("");
      exit(errno);
    }
    
    exit(0);
  } else {
    int status;
    while (wait(&status) != pid);
    if (status < 0) {
      perror("error during exec");
    }
    if (arguments) {
      for (int i = 0; i < line_sz; ++i) {
        free(arguments[i]);
      }
      free(arguments);
    }
  }
  clear_buffer(&buffer);
}

void apply_to_files_list(const list* lst, find_flags* flags) {
  if (!(flags->current_flags & exec_flag)) return ;
  list_node* cur_node = lst->data;
  while (cur_node) {
    apply_to_path(cur_node->path, flags->path_arg); 
    cur_node = cur_node->next;
  }
}
  
#undef true
#undef false 
