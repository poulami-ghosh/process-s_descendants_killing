#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>

#include "structure_definition.h"
#include "print_tree.h"
#include "create_and_destroy_tree.h"
#include "build_tree_from_proc.h"
#include "kill_descendats.h"

//_________________________________MAIN FUNCTION____________________________________

int main(int argc, char* argv[]) {
    //__________ASKING FOR COMMANDLINE ARGUMENTS_______________
    if (argc < 2) {
        printf("enter the taget pid also along with ./a.out\n");
        return 1;
    }    
    // CHECKING FOR /PROC ACCESSS
    if (access("/proc", R_OK) != 0) {
        perror("Error: No permission to read /proc filesystem");
        return 1;
    }
    
    //__________CREATING AND PRINTING THE WHOLE TREE_____________
    if(strcmp(argv[1],"whole_tree")==0){
          struct PIDTree* whole_tree = create_tree();
          if (!whole_tree) {
               fprintf(stderr, "Failed to create tree structure\n");
               return 1;
              }
          if (!build_tree(whole_tree, 1)) {
              fprintf(stderr, "Failed to build process tree\n");
              destroy_tree(whole_tree);
              return 1;
              }
          printf("\nThe whole Process Tree is:\n");
          printf("----------------------------------------\n");
          print_tree(whole_tree);
          destroy_tree(whole_tree);
     }       
    
    //_________CREATING AND BUILDING TREE WITH TARGET PID__________
    else if(strcmp(argv[1],"target_tree")==0 && argc==3){
          pid_t target_pid = (pid_t)atoi(argv[2]);
          struct PIDTree* target_tree = create_tree();
          if (!target_tree) {
             fprintf(stderr, "Failed to create tree structure\n");
             return 1;
           }
          if (!build_tree(target_tree, target_pid)) {
              fprintf(stderr, "Failed to build process tree\n");
              destroy_tree(target_tree);
              return 1;
          }
    
    // PRINTING THE TARGET TREE
         printf("\nThe targeted Process Tree is:\n");
         printf("----------------------------------------\n");
         print_tree(target_tree);
         destroy_tree(target_tree);
    }      
    //___________KILL OPERATION_____________________________________
    else if(strcmp(argv[1],"kill")==0 && argc==3){
          pid_t target_pid = (pid_t)atoi(argv[2]);
          struct PIDTree* target_tree = create_tree();
          if (!target_tree) {
             fprintf(stderr, "Failed to create tree structure\n");
             return 1;
          }
         if (!build_tree(target_tree, target_pid)) {
              fprintf(stderr, "Failed to build process tree\n");
              destroy_tree(target_tree);
              return 1;
              }
        kill_descendants(target_tree);
        destroy_tree(target_tree);
    }
    return 0;
}
