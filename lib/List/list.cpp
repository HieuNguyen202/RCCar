#include "list.h"

Node *Node::remove(){
    if (this->next == NULL || this->prev == NULL) { // Not a node of a list, just return.
        return this;
    } else if (IS_HEAD(this)) { // head node
        HEAD_GET_LIST(this)->trimHead();
    } else if (IS_TAIL(this)) { // tail node
        TAIL_GET_LIST(this)->trimTail();
    } else { // a random node in the middle of the list
        this->next->prev = this->prev;
        this->prev->next = this->next;
    }
    this->next = this->prev = NULL;
    return this;
}

void List::append(Node *n){
    if (!n)
        return;
    if(m_head == NULL) { // empty list
        m_head = m_tail = n;
        m_head->prev = m_self;
    } else { // has at least one element
        n->prev = m_tail;          // n is the new tail, so its prev is the old tail
        m_tail = m_tail->next = n; // Update new tail and the old tail's next
    }
    m_tail->next = m_self;  // signal end of list
}

Node* List::trimHead(){
    Node* ret;
    if(m_head == NULL) { // empty list
        return NULL;
    } else if (m_head == m_tail){ // one-element list
        ret = m_head;
        m_head = m_tail = NULL;
    } else { // two or more element list
        ret = m_head;
        m_head = m_head->next;
        m_head->prev = m_self;
    }
    ret->next = ret->prev = NULL;
    return ret;
}

Node* List::trimTail(){
    Node* ret;
    if(m_head == NULL) { // empty list
        return NULL;
    } else if (m_head == m_tail){ // one-element list
        ret = m_head; // m_head and m_tail point to the same thing
        m_head = m_tail = NULL;
    } else { // two or more element list
        ret = m_tail;
        m_tail = m_tail->prev;
        m_tail->next = m_self;
    }
    ret->next = ret->prev = NULL;
    return ret;
}

void List::steal(Node* n){
    append(n->remove());
}

void List::append(void *value){
    Node *n = new Node(value);
    append(n);
}

List::List(){
    m_self = new Node(this);
    m_head = m_tail = NULL;
}

int List::count(){
    int count = 0;
    Node *n;
    if ( m_head == NULL ) // empty list
        return 0;
    /* The below while loop is terminated as soon as n is m_tail, so we increment count before hand*/
    count = 1;
    n = m_head;
    while ( n != m_tail ) {
        n = n->next;
        count++;
    }
    return count;
}   