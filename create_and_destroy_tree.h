#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#ifndef CREATE_AND_DESTROY_TREE_H
#define CREATE_AND_DESTROY_TREE_H
#include "structure_definition.h"

#define MAX_PROCESSES 5000

//_____________________CREATING A NODE OF A PROCESS TREE___________________________

struct TreeNode* create_node(pid_t pid){
        struct TreeNode* node=malloc(sizeof(struct TreeNode));
        if(!node){
             fprintf(stderr,"memory allocation failed to create node\n");
             return NULL;
             }
        node->pid=pid;
        node->parent=NULL;
        node->children=malloc(sizeof(struct TreeNode*)*12);
        node->child_count=0;
        node->child_capacity=12;
        if(!node->children){
             free(node);
             return(NULL);
             }
        return node;
}             
             
//______________________CREATING A PROCESS TREE____________________________________
             
struct PIDTree* create_tree(void){
        struct PIDTree* tree=malloc(sizeof(struct PIDTree));
        if(!tree){
              fprintf(stderr,"memory allocation failed to create tree\n");
              return NULL;
              }
         tree->nodes=malloc(sizeof(struct PIDTree*)*MAX_PROCESSES);
         if(!tree->nodes){
              fprintf(stderr,"memory allocation failed to create tree->nodes\n");
              return NULL;
              }
         tree->node_count=0;
         tree->node_capacity=MAX_PROCESSES;
         tree->root=NULL;
         return tree;
}

//_______________________DESTROYING A NODE________________________________________

void destroy_node(struct TreeNode* node){
               if(!node)   return;

               for(int i=0;i< node->child_count;i++)
                    destroy_node(node->children[i]);
               free(node->children);
               free(node);          
}

//_____________________DESTROYING A TREE___________________________________________

void destroy_tree(struct PIDTree* tree){
               if(!tree)    return;
               
               for(int i=0; i< tree->node_count;i++){
                     struct TreeNode* node=tree->nodes[i];
                     if(node && node->parent==NULL)
                           destroy_node(node);
                     }
              free(tree->nodes);
              free(tree);            
}

#endif




     
          
          
               


