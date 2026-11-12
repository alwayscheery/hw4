#ifndef AVLBST_H
#define AVLBST_H

#include <iostream>
#include <exception>
#include <cstdlib>
#include <cstdint>
#include <algorithm>
#include "bst.h"

struct KeyError { };

/**
* A special kind of node for an AVL tree, which adds the balance as a data member, plus
* other additional helper functions. You do NOT need to implement any functionality or
* add additional data members or helper functions.
*/
template <typename Key, typename Value>
class AVLNode : public Node<Key, Value>
{
public:
    // Constructor/destructor.
    AVLNode(const Key& key, const Value& value, AVLNode<Key, Value>* parent);
    virtual ~AVLNode();

    // Getter/setter for the node's height.
    int8_t getBalance () const;
    void setBalance (int8_t balance);
    void updateBalance(int8_t diff);

    // Getters for parent, left, and right. These need to be redefined since they
    // return pointers to AVLNodes - not plain Nodes. See the Node class in bst.h
    // for more information.
    virtual AVLNode<Key, Value>* getParent() const override;
    virtual AVLNode<Key, Value>* getLeft() const override;
    virtual AVLNode<Key, Value>* getRight() const override;

protected:
    int8_t balance_;    // effectively a signed char
};

/*
  -------------------------------------------------
  Begin implementations for the AVLNode class.
  -------------------------------------------------
*/

/**
* An explicit constructor to initialize the elements by calling the base class constructor
*/
template<class Key, class Value>
AVLNode<Key, Value>::AVLNode(const Key& key, const Value& value, AVLNode<Key, Value> *parent) :
    Node<Key, Value>(key, value, parent), balance_(0)
{

}

/**
* A destructor which does nothing.
*/
template<class Key, class Value>
AVLNode<Key, Value>::~AVLNode()
{

}

/**
* A getter for the balance of a AVLNode.
*/
template<class Key, class Value>
int8_t AVLNode<Key, Value>::getBalance() const
{
    return balance_;
}

/**
* A setter for the balance of a AVLNode.
*/
template<class Key, class Value>
void AVLNode<Key, Value>::setBalance(int8_t balance)
{
    balance_ = balance;
}

/**
* Adds diff to the balance of a AVLNode.
*/
template<class Key, class Value>
void AVLNode<Key, Value>::updateBalance(int8_t diff)
{
    balance_ += diff;
}

/**
* An overridden function for getting the parent since a static_cast is necessary to make sure
* that our node is a AVLNode.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getParent() const
{
    return static_cast<AVLNode<Key, Value>*>(this->parent_);
}

/**
* Overridden for the same reasons as above.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getLeft() const
{
    return static_cast<AVLNode<Key, Value>*>(this->left_);
}

/**
* Overridden for the same reasons as above.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getRight() const
{
    return static_cast<AVLNode<Key, Value>*>(this->right_);
}


/*
  -----------------------------------------------
  End implementations for the AVLNode class.
  -----------------------------------------------
*/


template <class Key, class Value>
class AVLTree : public BinarySearchTree<Key, Value>
{
public:
    virtual void insert (const std::pair<const Key, Value> &new_item); // TODO
    virtual void remove(const Key& key);  // TODO
protected:
    virtual void nodeSwap( AVLNode<Key,Value>* n1, AVLNode<Key,Value>* n2);

    // Add helper functions here
    void rotateLeft(AVLNode<Key, Value>* node);
    void rotateRight(AVLNode<Key, Value>* node);


};
template<class Key, class Value>
void AVLTree<Key, Value>::rotateLeft(AVLNode<Key, Value>* node)
{
    AVLNode<Key, Value>* promoted = node->getRight();
    AVLNode<Key, Value>* middle = promoted->getLeft();
    AVLNode<Key, Value>* parent = node->getParent();

    if (parent == NULL) {
        this->root_ = promoted;
    } else if (parent->getLeft() == node) {
        parent->setLeft(promoted);
    } else {
        parent->setRight(promoted);
    }

    promoted->setParent(parent);
    promoted->setLeft(node);
    node->setParent(promoted);
    node->setRight(middle);

    if (middle != NULL) {
        middle->setParent(node);
    }

    int oldNodeBalance = node->getBalance();
    int oldPromotedBalance = promoted->getBalance();

    node->setBalance(oldNodeBalance - 1 -
                     std::max(static_cast<int>(oldPromotedBalance), 0));
    promoted->setBalance(oldPromotedBalance - 1 +
                         std::min(static_cast<int>(node->getBalance()), 0));
}
template<class Key, class Value>
void AVLTree<Key, Value>::rotateRight(AVLNode<Key, Value>* node)
{
    AVLNode<Key, Value>* promoted = node->getLeft();
    AVLNode<Key, Value>* middle = promoted->getRight();
    AVLNode<Key, Value>* parent = node->getParent();

    if (parent == NULL) {
        this->root_ = promoted;
    } else if (parent->getLeft() == node) {
        parent->setLeft(promoted);
    } else {
        parent->setRight(promoted);
    }

    promoted->setParent(parent);
    promoted->setRight(node);
    node->setParent(promoted);
    node->setLeft(middle);

    if (middle != NULL) {
        middle->setParent(node);
    }

    int oldNodeBalance = node->getBalance();
    int oldPromotedBalance = promoted->getBalance();

    node->setBalance(oldNodeBalance + 1 -
                     std::min(static_cast<int>(oldPromotedBalance), 0));
    promoted->setBalance(oldPromotedBalance + 1 +
                         std::max(static_cast<int>(node->getBalance()), 0));
}
/*
 * Recall: If key is already in the tree, you should 
 * overwrite the current value with the updated value.
 */
template<class Key, class Value>
void AVLTree<Key, Value>::insert (const std::pair<const Key, Value> &new_item)
{
    // TODO
  if (this->root_ == NULL) {
    this->root_ = new AVLNode<Key, Value>(
        new_item.first, new_item.second, NULL);
    return;
}

AVLNode<Key, Value>* current =
    static_cast<AVLNode<Key, Value>*>(this->root_);
AVLNode<Key, Value>* inserted = NULL;

while (true) {
    if (new_item.first == current->getKey()) {
        current->setValue(new_item.second);
        return;
    }

    if (new_item.first < current->getKey()) {
        if (current->getLeft() == NULL) {
            inserted = new AVLNode<Key, Value>(
                new_item.first, new_item.second, current);
            current->setLeft(inserted);
            break;
        }
        current = current->getLeft();
    } else {
        if (current->getRight() == NULL) {
            inserted = new AVLNode<Key, Value>(
                new_item.first, new_item.second, current);
            current->setRight(inserted);
            break;
        }
        current = current->getRight();
    }
}

AVLNode<Key, Value>* child = inserted;
AVLNode<Key, Value>* parent = current;

while (parent != NULL) {
    if (child == parent->getLeft()) {
        parent->updateBalance(-1);
    } else {
        parent->updateBalance(1);
    }

    int balance = parent->getBalance();

    if (balance == 0) {
        break;
    }

    if (balance == -1 || balance == 1) {
        child = parent;
        parent = parent->getParent();
        continue;
    }

    if (balance == -2) {
        AVLNode<Key, Value>* left = parent->getLeft();

        if (left->getBalance() > 0) {
            rotateLeft(left);
        }
        rotateRight(parent);
    } else if (balance == 2) {
        AVLNode<Key, Value>* right = parent->getRight();

        if (right->getBalance() < 0) {
            rotateRight(right);
        }
        rotateLeft(parent);
    }

    break;
}
}

/*
 * Recall: The writeup specifies that if a node has 2 children you
 * should swap with the predecessor and then remove.
 */
template<class Key, class Value>
void AVLTree<Key, Value>:: remove(const Key& key)
{
    // TODO
  AVLNode<Key, Value>* node =
    static_cast<AVLNode<Key, Value>*>(this->internalFind(key));

if (node == NULL) {
    return;
}

if (node->getLeft() != NULL && node->getRight() != NULL) {
    AVLNode<Key, Value>* pred =
        static_cast<AVLNode<Key, Value>*>(
            BinarySearchTree<Key, Value>::predecessor(node));
    this->nodeSwap(node, pred);
}

AVLNode<Key, Value>* parent = node->getParent();
AVLNode<Key, Value>* child =
    (node->getLeft() != NULL) ? node->getLeft() : node->getRight();

if (child != NULL) {
    child->setParent(parent);
}

if (parent == NULL) {
    this->root_ = child;
    delete node;
    return;
}

bool decreasedLeft = (parent->getLeft() == node);

if (decreasedLeft) {
    parent->setLeft(child);
} else {
    parent->setRight(child);
}

delete node;

while (parent != NULL) {
    if (decreasedLeft) {
        parent->updateBalance(1);
    } else {
        parent->updateBalance(-1);
    }

    int balance = parent->getBalance();

    if (balance == 1 || balance == -1) {
        break;
    }

    if (balance == 0) {
        AVLNode<Key, Value>* oldParent = parent;
        parent = parent->getParent();

        if (parent != NULL) {
            decreasedLeft = (parent->getLeft() == oldParent);
        }
        continue;
    }

    if (balance == 2) {
        AVLNode<Key, Value>* right = parent->getRight();
        int rightBalance = right->getBalance();

        if (rightBalance < 0) {
            rotateRight(right);
            rotateLeft(parent);
        } else {
            rotateLeft(parent);
            if (rightBalance == 0) {
                break;
            }
        }
    } else { // balance == -2
        AVLNode<Key, Value>* left = parent->getLeft();
        int leftBalance = left->getBalance();

        if (leftBalance > 0) {
            rotateLeft(left);
            rotateRight(parent);
        } else {
            rotateRight(parent);
            if (leftBalance == 0) {
                break;
            }
        }
    }

    AVLNode<Key, Value>* newSubtreeRoot = parent->getParent();
    parent = newSubtreeRoot->getParent();

    if (parent != NULL) {
        decreasedLeft = (parent->getLeft() == newSubtreeRoot);
    }
}
}

template<class Key, class Value>
void AVLTree<Key, Value>::nodeSwap( AVLNode<Key,Value>* n1, AVLNode<Key,Value>* n2)
{
    BinarySearchTree<Key, Value>::nodeSwap(n1, n2);
    int8_t tempB = n1->getBalance();
    n1->setBalance(n2->getBalance());
    n2->setBalance(tempB);
}


#endif
