To unzip all projects you can use unpack.sh.

Status: 
  Shell       [checked]
  Grep        [unchecked]
  Find        [unchecked]
Description: 
  
  Shell - simplified version of terminal
    Usage: ./my_shell - launch shell 

  Grep - simplified version of grep
    Usage: ./my_grep [-r] [-E] [-i] pattern [ files ... ] 
    Options:
      -r - recursive 
      -E - extended 
      -i - ignore case
    Description: Grep finds lines containing pattern in given files

  Find - simplified version of find 
    Usage: ./my_find path [--inum num] [--name name] [--size [-=+]size] [--nlinks num] [--exec path]
    Options:
    --inum num - files with the number of inode equal to num
    --name name - files with the names equal to name
    --size [-=+]size - files with the size less/equal/grt size
    --nlinks num - files with num hardlinks
    --exec path - call path command on the founded files
