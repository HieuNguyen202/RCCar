#ifndef LIST_H
#define LIST_H

/* Hugh Nguyen - htnguyen203@gmail.com - March 8, 2020
 * An implementation of a doubly-linked list. This implementation features
 * a steal() method which allows a list to steal a node from another list
 * without having the second list's handle. Both lists' head and tail are
 * properly updated after the operation.
 * The List class has the head and tail nodes both point to a `m_self` node,
 * which stores the handle to the list itself. This allows us to update the
 * stolen-from list's head and tail pointers after the node is being removed.
*/

#include <stddef.h>
#define IS_HEAD(n) ((n) && (n)->prev && IS_STANDALONE((n)->prev))
#define IS_TAIL(n) ((n) && (n)->next && IS_STANDALONE((n)->next))
#define IS_STANDALONE(n) ((n) && (n)->next == NULL && (n)->prev == NULL)
#define IS_SELF(n) IS_STANDALONE(n)
#define HEAD_GET_LIST(n) ( (List*) ((n)->prev->value) ) /* must run IS_HEAD first to make sure it's a head node */
#define TAIL_GET_LIST(n) ( (List*) ((n)->next->value) ) /* must run IS_TAIL first to make sure it's a tail node */

typedef struct Node {
    Node *next;
    Node *prev;
    void* value;
    int ivalue; // integer value
    Node(int value){
        this->next = NULL;
        this->prev = NULL;
        this->value = NULL;
        this->ivalue = value;
    }
    Node(void* value){
        this->next = NULL;
        this->prev = NULL;
        this->value = value;
        this->ivalue = 0;
    }
    Node(){
        this->next = NULL;
        this->prev = NULL;
        this->value = NULL;
        this->ivalue = 0;
    }
    // Remove itself from its list if the node is a node in list
    Node* remove();
} Node;

class List {
    public:
        /* Constructor */
        List();
        /* Allocate a node for the given value and append it to the list */
        void append(void *value);
        /* Append a node to the list */
        void append(Node *n);
        /* Remove the head node */
        Node* trimHead();
        /* Remove the tail node */
        Node* trimTail();
        /* Remove a node from the list */
        Node* remove(Node *n);
        /* Completely remove a node from its list and append it to this list */
        void steal(Node* n);
        /* Getter for m_head */
        Node *head() { return m_head; };
        /* Getter for m_tail */
        Node *tail() { return m_tail; };
        /* Getter for m_self */
        Node *self() { return m_self; };
        /* Return whether the list is empty or not */
        bool empty() { return m_head == NULL; };
        /* Return list count */
        int count();

    private:
        // m_self is a special node. Its value points to this list.
        // its next and previous in NULL, which can be used to signify end of the list.
        Node *m_head, *m_tail, *m_self;
};

#endif //LIST_H