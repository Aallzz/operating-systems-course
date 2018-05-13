#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>
#include <stdio.h>

int main(int argc, char *argv[]) {
  struct stat s;
  char *ptype;
  
  if (argc < 2) {
    printf("Usage: main file");
    return 0;
  }

  lstat(argv[1], &s);
  if (S_ISREG(s.st_mode)) ptype = "Regular file";
  else if (S_ISDIR(s.st_mode)) ptype = "Directory";
  else if (S_ISCHR(s.st_mode)) ptype = "Sym link";
  else if (S_ISBLK(s.st_mode)) ptype = "Block devise";
  else if (S_ISSOCK(s.st_mode)) ptype = "Socket";
  else if (S_ISFIFO(s.st_mode)) ptype = "FIFO";
  else ptype = "Unknown type";

  printf("type = %s\n", ptype);
  printf("perm = %o\n", s.st_mode);
  printf("inode = %d\n", s.st_ino);
  printf("nlink = %d\n", s.st_nlink);
  printf("dev = (%d, %d)\n", major(s.st_dev), minor(s.st_dev));
  printf("UID = %d\n", s.st_uid);
  printf("GID = %d\n", s.st_gid);
  printf("rdev = (%d, %d)\n", major(s.st_rdev), minor(s.st_rdev));
  printf("size = %d\n", s.st_size);
  printf("atime = %s", ctime(&s.st_atime));
  printf("mtime = %s", ctime(&s.st_mtime));
  printf("ctime = %s", ctime(&s.st_ctime));
  return 0;
}
