To unzip all projects you can use unpack.sh.

Status: 
  Shell       [checked]
  Grep        [unchecked]
  Find        [unchecked]
  JIT         [unchecked]
  Lib         [unchecked]

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

  Jit - simplified jit-compiler for primitive math expressions in reverse polish notation
    Usage: ./my_jit <file with expression> <value to pass>
    Description: expression in file can consist of float numbers, basic math operations (+ - * /), and letter x for value, that will be passed in my_jit program. Every token in the expression must be separated with spaces. 
    Additional functionality: jit_function class has special function [void change(int pos, char new_op)]to change one operation in expression without changing of the rest code

  Lib - example of library usage.
    Usage: ./my_lib <circle/square> <ind> 
