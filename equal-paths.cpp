#ifndef RECCHECK
//if you want to add any #includes like <iostream> you must do them here (before the next endif)

#endif

#include "equal-paths.h"
using namespace std;


// You may add any prototypes of helper functions here
int checkPaths(Node* node)
{
  if (node == nullptr) {
      return 0;
  }

    // A leaf has a path length of 0 edges to itself.
  if (node->left == nullptr && node->right == nullptr) {
      return 0;
    }

  int leftLength = checkPaths(node->left);
  int rightLength = checkPaths(node->right);

    // A missing child is not a path to a leaf, so use the other side.
  if (node->left == nullptr) {
      return rightLength < 0 ? -1 : rightLength + 1;
  }
  if (node->right == nullptr) {
      return leftLength < 0 ? -1 : leftLength + 1;
  }

    // Both children exist: their leaf paths must be valid and equal.
  if (leftLength < 0 || rightLength < 0 || leftLength != rightLength) {
      return -1;
  }

  return leftLength + 1;
}


bool equalPaths(Node * root)
{
    // Add your code below
  return checkPaths(root) >= 0;

}

