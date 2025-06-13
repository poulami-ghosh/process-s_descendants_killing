#include<stdio.h>
#include<sys/types.h>

#ifndef STRUCTURE_DEFINITION_H
#define STRUCTURE_DEFINITION_H

#define MAX_COMM_LEN 25

//___________TREE NODE STRUCTURE REPRESENTING A PROCESS_________________________

struct TreeNode {
    pid_t pid;
    struct TreeNode* parent;
    struct TreeNode** children;
    int child_count;
    int child_capacity;
    char proc_name[MAX_COMM_LEN];
};

//_____________________PROCESS TREE STRUCTURE____________________________________

struct PIDTree {
    struct TreeNode** nodes;    //ARRAY OF ALL NODES
    int node_count;
    int node_capacity;
    struct TreeNode* root;        
};

#endif





