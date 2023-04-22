#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "treemap.h"

typedef struct TreeNode TreeNode;


struct TreeNode {
    Pair* pair;
    TreeNode * left;
    TreeNode * right;
    TreeNode * parent;
};

struct TreeMap {
    TreeNode * root;
    TreeNode * current;
    int (*lower_than) (void* key1, void* key2);
};

int is_equal(TreeMap* tree, void* key1, void* key2){
    if(tree->lower_than(key1,key2)==0 &&  
        tree->lower_than(key2,key1)==0) return 1;
    else return 0;
}


TreeNode * createTreeNode(void* key, void * value) {
    TreeNode * new = (TreeNode *)malloc(sizeof(TreeNode));
    if (new == NULL) return NULL;
    new->pair = (Pair *)malloc(sizeof(Pair));
    new->pair->key = key;
    new->pair->value = value;
    new->parent = new->left = new->right = NULL;
    return new;
}

TreeMap * createTreeMap(int (*lower_than) (void* key1, void* key2)) {
    TreeMap * tree = (TreeMap*) malloc(sizeof(TreeMap));
    tree->root = NULL;
    tree->current = NULL;
    tree->lower_than = lower_than;
    //new->lower_than = lower_than;
    return tree;
}


void insertTreeMap(TreeMap * tree, void* key, void * value) {
    TreeNode* current = tree->root;
    TreeNode* parent = NULL;
    while (current != NULL) {
        parent = current;
        if (is_equal(tree, current->pair->key, key)) {
            tree->current = current;
            return;
        }
        else if (tree->lower_than(key, current->pair->key)) {
            current = current->left;
        }
        else {
            current = current->right;
        }
    }
    TreeNode* new_node = createTreeNode(key, value);
    if (new_node == NULL) {
        return;
    }
    if (parent == NULL) {
        tree->root = new_node;
    }
    else if (tree->lower_than(key, parent->pair->key)) {
        parent->left = new_node;
    }
    else {
        parent->right = new_node;
    }
    new_node->parent = parent;
    tree->current = new_node;
}


TreeNode * minimum(TreeNode * x){
    while (x->left != NULL) {
        x = x->left;
    }
    return x;
}


void removeNode(TreeMap * tree, TreeNode* node) {
    if (node->left == NULL && node->right == NULL) {
        if (node->parent == NULL) {
            tree->root = NULL;
        } else if (node == node->parent->left) {
            node->parent->left = NULL;
        } else {
            node->parent->right = NULL;
        }
        free(node);
    } else if (node->left == NULL || node->right == NULL) {
        TreeNode* child = node->left == NULL ? node->right : node->left;
        child->parent = node->parent;
        if (node->parent == NULL) {
            tree->root = child;
        } else if (node == node->parent->left) {
            node->parent->left = child;
        } else {
            node->parent->right = child;
        }
        free(node);
    } else {
        TreeNode* minNode = minimum(node->right);
        node->pair->key = minNode->pair->key;
        node->pair->value = minNode->pair->value;
        removeNode(tree, minNode);
    }
    tree->current = NULL;
}

void eraseTreeMap(TreeMap * tree, void* key){
    if (tree == NULL || tree->root == NULL) return;

    if (searchTreeMap(tree, key) == NULL) return;
    TreeNode* node = tree->current;
    removeNode(tree, node);

}




Pair * searchTreeMap(TreeMap * tree, void* key) {
    TreeNode* current = tree->root;
    while (current != NULL) {
        if (is_equal(tree, current->pair->key, key)) {
            tree->current = current;
            return current->pair;
        }
        else if (tree->lower_than(key, current->pair->key)) {
            current = current->left;
        }
        else {
            current = current->right;
        }
    }
    return NULL;
}


Pair * upperBound(TreeMap * tree, void* key) {
    TreeNode* node = tree->root;
    TreeNode* ub_node = NULL;

    while (node != NULL) {
        if (tree->lower_than(key, node->pair->key)) {
            ub_node = node;
            node = node->left;
        } else if (is_equal(tree, key, node->pair->key)) {
            return node->pair;
        } else {
            node = node->right;
        }
    }

    if (ub_node != NULL) {
        return ub_node->pair;
    } else {
        return NULL;
    }
}

Pair * firstTreeMap(TreeMap * tree) {
    if (tree->root == NULL) return NULL;
    TreeNode * current = minimum(tree->root);
    tree->current = current;
    return current->pair;
}

Pair * nextTreeMap(TreeMap * tree) {
    if (tree->current == NULL) return NULL;
    TreeNode * current = tree->current;
    if (current->right != NULL) {
        current = minimum(current->right);
    } else {
        TreeNode * parent = current->parent;
        while (parent != NULL && current == parent->right) {
            current = parent;
            parent = parent->parent;
        }
        current = parent;
    }
    tree->current = current;
    return (current == NULL) ? NULL : current->pair;
}
