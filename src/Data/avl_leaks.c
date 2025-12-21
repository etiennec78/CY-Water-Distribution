#include <stdlib.h>
#include <string.h>
#include "common.h"

int getHeight(NodeIndex *N) {
  if (N == NULL)
    return 0;
  return N->height;
}

NodeIndex *rightRotate(NodeIndex *y) {
  NodeIndex *x = y->left;
  NodeIndex *T2 = x->right;

  x->right = y;
  y->left = T2;

  y->height = max(getHeight(y->left), getHeight(y->right)) + 1;
  x->height = max(getHeight(x->left), getHeight(x->right)) + 1;

  return x;
}

NodeIndex *leftRotate(NodeIndex *x) {
  NodeIndex *y = x->right;
  NodeIndex *T2 = y->left;

  y->left = x;
  x->right = T2;

  x->height = max(getHeight(x->left), getHeight(x->right)) + 1;
  y->height = max(getHeight(y->left), getHeight(y->right)) + 1;

  return y;
}

int getBalance(NodeIndex *N) {
  if (N == NULL)
    return 0;
  return getHeight(N->left) - getHeight(N->right);
}

NodeIndex* balance_node(NodeIndex* node) {
  node->height = 1 + max(getHeight(node->left), getHeight(node->right));

  int balance = getBalance(node);

  if (balance > 1 && getBalance(node->left) >= 0)
    return rightRotate(node);

  if (balance > 1 && getBalance(node->left) < 0) {
    node->left = leftRotate(node->left);
    return rightRotate(node);
  }

  if (balance < -1 && getBalance(node->right) <= 0)
    return leftRotate(node);

  if (balance < -1 && getBalance(node->right) > 0) {
    node->right = rightRotate(node->right);
    return leftRotate(node);
  }

  return node;
}

void free_avl_leaks(NodeIndex* root) {
  if (root == NULL) return;

  free_avl_leaks(root->left);
  free_avl_leaks(root->right);
  if (root->component_ptr != NULL) {
    free(root->component_ptr);
  }
  free(root);
}

NetworkComponent* find_or_create_component(NodeIndex** root, char* id) {
  if (*root == NULL) {
    NetworkComponent* new_comp = malloc(sizeof(NetworkComponent));
    if (!new_comp) return NULL;
    strncpy(new_comp->id, id, 49);
    new_comp->id[49] = '\0';
    new_comp->leak_percent = 0;
    new_comp->first_child = NULL;
    new_comp->next_sibling = NULL;

    *root = malloc(sizeof(NodeIndex));
    if (!(*root)) { free(new_comp); return NULL; }
    strncpy((*root)->id, id, 49);
    (*root)->id[49] = '\0';
    (*root)->component_ptr = new_comp;
    (*root)->left = (*root)->right = NULL;
    (*root)->height = 1;
    return new_comp;
  }

  int cmp = strcmp(id, (*root)->id);
  if (cmp < 0) {
    NetworkComponent* res = find_or_create_component(&((*root)->left), id);
    *root = balance_node(*root);
    return res;
  }
  if (cmp > 0) {
    NetworkComponent* res = find_or_create_component(&((*root)->right), id);
    *root = balance_node(*root);
    return res;
  }
  return (*root)->component_ptr;
}


NetworkComponent* find_component_by_id(NodeIndex* root, char* target_id) {
  if (root == NULL) return NULL;
  int cmp = strcmp(target_id, root->id);

  if (cmp == 0) return root->component_ptr;
  if (cmp < 0) return find_component_by_id(root->left, target_id);
  return find_component_by_id(root->right, target_id);
}
