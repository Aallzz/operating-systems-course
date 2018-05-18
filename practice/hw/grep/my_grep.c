#define _GNU_SOURCE 1		/* for getline() */
#include <stdio.h>
#include <errno.h>
#include <regex.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <string.h>
#include <stdlib.h>

char *myname;			
int ignore_case = 0;
int extended = 0;	
int errors = 0;	
int recursive_search = 0; 
regex_t pattern;

#define N 1000

char  queue[N][N];
int qh = 0;
int qt = 0;

struct node {
  struct node* next;
  int value;
};
struct node *hash_table[N];

void add_value_in_hash_table(int pos, int value) {
  struct node *new_node = (struct node*)malloc(sizeof(struct node));
  new_node->next = hash_table[pos];
  new_node->value = value;
  hash_table[pos] = new_node;
}

int delete_from_hash_table(int pos) {
  if (hash_table[pos] == NULL) 
    return 0;
  struct node *old_node = hash_table[pos];
  hash_table[pos] = hash_table[pos]->next;
  free(old_node);
  return 1;
}

int check_value_in_hash_table(int pos, int value) {
  struct node* current_node = hash_table[pos];
  while (current_node != NULL) {
    if (current_node->value == value) 
      return 1;
    current_node = current_node->next;
  }
  return 0;
}

void clean_hash_table() {
  for (int i = 0; i < N; ++i) {
    while (delete_from_hash_table(i));
  }
}

int calc_hash(int value) {
  return (value * 11) % N;
}

void compile_pattern(const char *pat);
void process(const char *name, FILE *fp);
void usage(void);
void sprint(char *filename, char * dirToOpen, int level );
void revursive_walk(); 
int is_regular_file(const char *path);

int main(int argc, char **argv)
{
	int c;
	int i;
	myname = argv[0];
	while ((c = getopt(argc, argv, ":ier")) != -1) {
	  switch (c) {
        case 'r':
          recursive_search = 1;
          break;
		case 'i':
		  ignore_case = 1;
		  break;
		case 'e':
		  extended = 1;
		  break;
		case '?':
		  usage();
		  break;
		}
	}
  
	if (optind == argc)
	  usage();

	compile_pattern(argv[optind]);	
	if (errors)			
	  return 1;
	else
	  optind++;

	if (optind == argc)	
	  process("standard input", stdin);
	else {
	  for (i = optind; i < argc; i++) {
	  	if (strcmp(argv[i], "-") == 0)
	  	  process("standard input", stdin);
		else
          if (recursive_search) {
	        strcpy(queue[qt++], argv[i]);           
          } else {
            FILE *fp;
            if ((fp = fopen(argv[i], "r")) != NULL) {
              process(argv[i], fp);
              fclose(fp);
            } else {
              fprintf(stderr, "%s: %s: could not open: %s\n",
                      "grep", argv[i], strerror(errno));
              errors++;
            }
          }
	  }
	  if (recursive_search) {
		revursive_walk();
	  }
	}
	regfree(& pattern);
    clean_hash_table();  
	return errors != 0;
}

void compile_pattern(const char *pat)
{
  int flags = REG_NOSUB;	
  int ret;
#define MSGBUFSIZE	512	
  char error[MSGBUFSIZE];
  
  if (ignore_case)
  	flags |= REG_ICASE;
  if (extended)
	flags |= REG_EXTENDED;

  ret = regcomp(& pattern, pat, flags);
  if (ret != 0) {
  	(void) regerror(ret, & pattern, error, sizeof error);
	fprintf(stderr, "%s: pattern `%s': %s\n", myname, pat, error);
	errors++;
  }
}

int get_inodev(FILE* fp) {
  int fd = fileno(fp);	
  struct stat buf;
  int ret;
 
  ret = fstat(fd, &buf);
  if ( ret <0 ) {
    perror ("fstat");
    return -1;
  }

  return buf.st_ino + buf.st_dev;
}

void process(const char *name, FILE *fp)
{
  char *buf = NULL;
  size_t size = 0;
  char error[MSGBUFSIZE];
  int ret;
  
  if (check_value_in_hash_table(calc_hash(get_inodev(fp)), get_inodev(fp))) {
    return ;
  }
  add_value_in_hash_table(calc_hash(get_inodev(fp)), get_inodev(fp));

  while (getline(& buf, &size, fp) != -1) {
  	ret = regexec(& pattern, buf, 0, NULL, 0);
  	if (ret != 0) {
      if (ret != REG_NOMATCH) {
	    (void) regerror(ret, & pattern, error, sizeof error);
		fprintf(stderr, "%s: file %s: %s\n", myname, name, error);
		free(buf);
		errors++;
		return;
	  }
	} else
	  printf("%s: %s", name, buf);	
  }
  free(buf);
}

void usage(void)
{
  fprintf(stderr, "usage: %s [-i] [-E] [-r] pattern [ files ... ]\n", myname);
  exit(1);
}

int is_regular_file(const char* path) {
  struct stat path_stat;
  stat(path, &path_stat);
  return S_ISREG(path_stat.st_mode);
}

int get_inodev_by_name(const char* path) {
  struct stat path_stat;
  stat(path, &path_stat);
  return path_stat.st_ino + path_stat.st_dev;
}

void revursive_walk() {
  while (qt != qh) {
    char dir[N];
    strcpy(dir, queue[qh++]);
    DIR *dp;
    struct dirent *entry;
    struct stat statbuf;
    char *subdir;
    
    if (is_regular_file(dir)) {
      FILE *fp;
      if ((fp = fopen(dir, "r")) != NULL) {
        process(dir, fp);
        fclose(fp);
      } else {
        fprintf(stderr, "%s: %s: could not open: %s\n",
                "grep", dir, strerror(errno));
        errors++;
      }
      continue;
    }

    if((dp = opendir(dir)) == NULL) {
      fprintf(stderr,"cannot open directory: %s\n", dir);
      continue;
    }
    
    if (check_value_in_hash_table(calc_hash(get_inodev_by_name(dir)), 
                                  get_inodev_by_name(dir))) {  
      closedir(dp);
      continue;
    }
    add_value_in_hash_table(calc_hash(get_inodev_by_name(dir)),     
                            get_inodev_by_name(dir));
     
    while((entry = readdir(dp)) != NULL) {
      if(strcmp(".", entry->d_name) == 0 || strcmp("..", entry->d_name) == 0)
        continue;
      subdir = (char*)malloc(strlen(dir) + strlen(entry->d_name) + 2);
      *subdir = '\0';
      strcpy(subdir, dir);
      strcat(subdir, "/");
      strcat(subdir, entry->d_name);
      if(stat(subdir, &statbuf) == 0) {
        if(statbuf.st_mode & S_IFDIR) {
          strcpy(queue[qt++], subdir);
        } else {
          FILE *fp;
          if ((fp = fopen(subdir, "r")) != NULL) {
            process(subdir, fp);
            fclose(fp);
          } else {
            fprintf(stderr, "%s: %s: could not open: %s\n",
                    "grep", dir, strerror(errno));
            errors++;
          }
        }
      }
      free(subdir);
    }
    closedir(dp);
  }
}
