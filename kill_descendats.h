#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <ctype.h>

#ifndef KILL_DESCENDANTS_H
#define KILL_DESCENDANTS_H

#include "structure_definition.h"

//__________________FUNCTION TO KILL DESCENDANTS IN DFS-POST ORDER___________________

void kill_dfs_post_order(struct TreeNode* node) {
    if (!node) return;
    for (int i = 0; i < node->child_count; i++) {
        kill_dfs_post_order(node->children[i]);
        sleep(2);
    }
    if (kill(node->pid, SIGTERM) == 0) {
        printf("Successfully sent SIGTERM to PID %d (%s)\n", node->pid , node->proc_name);
    } else {
        if (errno == ESRCH) {
            fprintf(stderr, "Error: No such process with PID %d (%s)\n", node->pid,node->proc_name);
        } else if (errno == EPERM) {
            fprintf(stderr, "Error: Permission denied to kill PID %d (%s)\n", node->pid, node->proc_name);
        } else {
            fprintf(stderr, "Error killing PID %d (%s): %s\n", node->pid,node->proc_name, strerror(errno));
        }
    }

}
//_______________FUNCTION TO KILL ALL DESCENDANT PROCESSES__________________________

void kill_descendants(struct PIDTree* target_tree){
       if (!target_tree || !target_tree->root){ 
              printf("target tree is not created");
              return;
              }
       kill_dfs_post_order(target_tree->root);
}

#endif

