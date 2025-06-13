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

#ifndef BUILD_TREE_FROM_PROC_H
#define BUILD_TREE_FROM_PROC_H

#include "structure_definition.h"
#include "create_and_destroy_tree.h"

#define MAX_PATH 256
#define MAX_LINE 1024

int is_number(const char* str);
int read_proc_stat(pid_t pid, pid_t* parentPID);
int read_proc_comm(pid_t pid, char* comm);
int build_tree(struct PIDTree* tree, pid_t root_pid);
void add_child(struct TreeNode* parent, struct TreeNode* child);
struct TreeNode* find_node(struct PIDTree* tree, pid_t pid);
int add_node(struct PIDTree* tree, struct TreeNode* node);


//_________________FUNCTION TO CHECK IF A STRING IS A NUMBER________________________

int is_number(const char* str) {
    if (!str || !*str) return 0;
    while (*str) {
        if (!isdigit(*str)) return 0;
        str++;
    }
    return 1;
}

//____________________FUNCTION TO READ PARENT PID FROM PROC/PID/STAT________________

int read_proc_stat(pid_t pid, pid_t* parentPID) {
    char path[MAX_PATH];
    char line[MAX_LINE];
    FILE* file;
    int field_count = 0;
    char* token;
    snprintf(path, sizeof(path), "/proc/%d/stat", pid);
    file = fopen(path, "r");
    if (!file) 
        return 0; //PROCESS MIGHT DISAPPEAR
    if (!fgets(line, sizeof(line), file)) {
        fclose(file);
        return 0;
    }
    fclose(file);
    //4TH FIELD OF PROC STAT IS PARENT PID
    token = strtok(line, " ");
    while (token && field_count < 4) {
        field_count++;
        if (field_count == 4) {
            *parentPID = (pid_t)atoi(token);
            return 1;
        }
        token = strtok(NULL, " ");
    }
    return 0;
}

//____________________FUNCTION TO READ PROCESS NAME FROM /PROC/PID/COMM___________

int read_proc_comm(pid_t pid,char* proc_name){
       char path[MAX_PATH];
       FILE* file;
       snprintf(path,sizeof(path),"/proc/%d/comm",pid);
       file=fopen(path,"r");
       if(!file)
           return 0;
       if(fgets(proc_name,MAX_COMM_LEN,file)){
           size_t len =strlen(proc_name);
           if(len>0 && proc_name[len-1]=='\n'){
                 proc_name[len-1]='\0';
                 }
            fclose(file);
            return 1;
            }
       fclose(file);
       return 0;
       }          
       
//______________FUNCTION TO ADD CHILD TO THE PARENT NODE____________________________

void add_child(struct TreeNode* parent, struct TreeNode* child) {
    if (!parent || !child) return;
    
    //RESIZING CHILDREN ARRAY IF NEEDED
    if (parent->child_count >= parent->child_capacity) {
        parent->child_capacity *= 2;
        parent->children = realloc(parent->children, 
                                 sizeof(struct TreeNode*) * parent->child_capacity);
        if (!parent->children) {
            fprintf(stderr, "Memory reallocation failed\n");
            return;
        }
    }
    
    parent->children[parent->child_count++] = child;
    child->parent = parent;
}

//____________________FUNCTION TO FIND NODE BY PID__________________________________

struct TreeNode* find_node(struct PIDTree* tree, pid_t pid) {
       for (int i = 0; i < tree->node_count; i++) {
              if (tree->nodes[i]->pid == pid) {
              return tree->nodes[i];
            }
       }
        return NULL;
}

//_________________________FUNCTION TO ADD NODE TO THE TREE__________________________

int add_node(struct PIDTree* tree, struct TreeNode* node) {
    if (tree->node_count >= tree->node_capacity) {
        return 0;  //TREE IS FULL
    }
    
    tree->nodes[tree->node_count++] = node;
    return 1;
}

//_______________________FUNCTION TO BUILD PROCESS TREE FROM /PROC FILE______________
 
int build_tree(struct PIDTree* tree, pid_t root_pid) {
    DIR* proc_dir;
    struct dirent* entry;
    pid_t pid, parentPID;
    struct TreeNode* node;
    struct TreeNode* parent_node;
    char proc_name[MAX_COMM_LEN];
    
    //OPENING /PROC DIRECTORY
    proc_dir = opendir("/proc");
    if (!proc_dir) {
        perror("Failed to open /proc");
        return 0;
    }
    
    //CREATING ALL NODES
    while ((entry = readdir(proc_dir)) != NULL) {
        if (!is_number(entry->d_name)) continue;
        
        pid = (pid_t)atoi(entry->d_name);
        if (pid <= 0) continue;
        
        if (read_proc_stat(pid, &parentPID)) {
            node = create_node(pid);
            if (node) {
                if(read_proc_comm(pid,proc_name)){
                     strncpy(node->proc_name,proc_name,MAX_COMM_LEN-1);
                     node->proc_name[MAX_COMM_LEN-1]='\0';
                     }
                 else
                     strcpy(node->proc_name,"[unknown]");
                add_node(tree, node);
                if (pid == root_pid) {
                    tree->root = node;
                }
            }
        }
    }
    
    rewinddir(proc_dir);
    
    //BUILDING PARENT-CHILD RELATIONSHIP
    while ((entry = readdir(proc_dir)) != NULL) {
        if (!is_number(entry->d_name)) continue;
        
        pid = (pid_t)atoi(entry->d_name);
        if (pid <= 0) continue;
        
        if (read_proc_stat(pid, &parentPID) && parentPID > 0) {
            node = find_node(tree, pid);
            parent_node = find_node(tree, parentPID);
            
            if (node && parent_node) {
                add_child(parent_node, node);
            }
        }
    }
    closedir(proc_dir);
    
    if (!tree->root) {
        fprintf(stderr, "Root process %d not found\n", root_pid);
        return 0;
    }
    
    return 1;
}


#endif














 
