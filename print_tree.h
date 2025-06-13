#include<stdio.h>
#ifndef PRINT_TREE_H
#define PRINT_TREE_H
#include "structure_definition.h"

//__________________FUNTION TO PRINT TREE STRUCTURE RECCURSSIVELY__________________

void print_tree_recursive(struct TreeNode* node, const char* prefix, int is_last) {
    if (!node) return;
    
    printf("%s%s%d (%s)\n", prefix, is_last ? "└──" : "├── ", node->pid,node->proc_name);
    
    char prefix_updated[512];
    snprintf(prefix_updated, sizeof(prefix_updated), "%s%s", prefix, is_last ? "    " : "│   ");
    
    for (int i = 0; i < node->child_count; i++) {
        int is_last_child = (i == node->child_count - 1);
        print_tree_recursive(node->children[i], prefix_updated, is_last_child);
    }
}

//______________________FUNCTION TO PRINT TREE_______________________________________

void print_tree(struct PIDTree* tree) {
    if (!tree || !tree->root) {
        printf("Tree is empty\n");
        return;
    }
    
    printf("Process Tree (Root: %d)\n", tree->root->pid);
    printf("----------------------------------------\n");
    print_tree_recursive(tree->root, "", 1);
}

#endif

