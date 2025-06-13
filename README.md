# process-s_descendants_killing
Goal of the project is to kill a process and all its (current) descendants.The program is taking the PID of the target process as its compulsory argument.
## Instructions to run the program :
   1.First download the all files(one .c fil and five .h files) and keep them in the same directory\
   2.Compile the file named process_tree_kill_main.c
## 3.(A) To view the full process tree rooted at pid 1:
  Run with command ./a.out   whole_tree
## 3.(B) To view the target tree (to be killed) rooted at given input pid (<target_pid>) by user:
  Run with command ./a.out   target_tree  <target_pid>
## 3.(C) To kill the subtree rooted at target tree:
  Run with command ./a.out   kill   <target_pid>
