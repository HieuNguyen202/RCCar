#include <list.h>
#include <unity.h>

void test_node_default_constructor(){
    Node n;
    TEST_ASSERT_NULL(n.next);
    TEST_ASSERT_NULL(n.prev);
    TEST_ASSERT_NULL(n.value);
    TEST_ASSERT_EQUAL(0, n.ivalue);
}

void test_node_constructor(){
    int *i = new int(1);
    Node n(i);
    TEST_ASSERT_NULL(n.next);
    TEST_ASSERT_NULL(n.prev);
    TEST_ASSERT_EQUAL_PTR(i, n.value);
    TEST_ASSERT_EQUAL(*i, *((int*)n.value));
}

void test_node_constructor2(){
    int i = 1;
    Node n(i);
    TEST_ASSERT_NULL(n.next);
    TEST_ASSERT_NULL(n.prev);
    TEST_ASSERT_NULL(n.value);
    TEST_ASSERT_EQUAL(i, n.ivalue);
}

void test_list_constructor(){
    List l;
    TEST_ASSERT_NULL(l.head());
    TEST_ASSERT_NULL(l.tail());
    TEST_ASSERT_EQUAL_PTR(&l, l.self()->value);
}
#ifdef UNIT_TEST

void test_list_append_node(){
    List l;
    Node *n1 = new Node(new int(1));
    Node *n2 = new Node(new int(2));
    
    TEST_ASSERT_EQUAL(0, l.count());

    // One-element list
    l.append(n1);
    TEST_ASSERT_EQUAL(1, l.count());
    TEST_ASSERT_EQUAL_PTR(n1, l.head());
    TEST_ASSERT_EQUAL_PTR(n1, l.tail());
    TEST_ASSERT_TRUE(IS_HEAD(l.head()));
    TEST_ASSERT_TRUE(IS_TAIL(l.tail()));

    // Two-element list
    l.append(n2);
    TEST_ASSERT_EQUAL(2, l.count());
    TEST_ASSERT_EQUAL_PTR(n1, l.head());
    TEST_ASSERT_EQUAL_PTR(n2, l.tail());
    TEST_ASSERT_EQUAL_PTR(l.head()->next, l.tail());
    TEST_ASSERT_EQUAL(1, *(int*)l.head()->value);
    TEST_ASSERT_TRUE(IS_HEAD(l.head()));
    TEST_ASSERT_TRUE(IS_TAIL(l.tail()));
}

void test_list_append_value(){
    List l;
    int *v1 = new int(1);
    int *v2 = new int(2);
    TEST_ASSERT_EQUAL(0, l.count());
    // One-element list
    l.append(v1);
    TEST_ASSERT_EQUAL(1, l.count());
    TEST_ASSERT_EQUAL_PTR(v1, l.head()->value);
    TEST_ASSERT_EQUAL_PTR(v1, l.tail()->value);
    // Two-element list
    l.append(v2);
    TEST_ASSERT_EQUAL(2, l.count());
    TEST_ASSERT_EQUAL_PTR(v1, l.head()->value);
    TEST_ASSERT_EQUAL_PTR(v2, l.tail()->value);
    TEST_ASSERT_EQUAL(1, *(int*)l.head()->value);
}

void test_list_macro(){
    Node *n1 = NULL;
    Node *n2 = new Node(new int(1));
    Node *n3 = new Node(new int(1));
    Node *n4 = new Node(new int(1));
    TEST_ASSERT_FALSE(IS_STANDALONE(n1)); // NULL is not a standalone node
    TEST_ASSERT_TRUE(IS_STANDALONE(n2));  // true, has next
    n3->prev = n2;
    n3->next = n4;
    TEST_ASSERT_FALSE(IS_STANDALONE(n3));
    TEST_ASSERT_FALSE(IS_HEAD(n1)); // NULL is not head
    TEST_ASSERT_FALSE(IS_TAIL(n1)); // NULL is not tail
    TEST_ASSERT_TRUE(IS_HEAD(n3)); // n4 has next and prev NULL so qualify
    TEST_ASSERT_TRUE(IS_TAIL(n3)); // n2 has next and prev NULL so qualify
}

void test_list_trim_head(){
    List l;
    Node *n1 = new Node(new int(1));
    Node *n2 = new Node(new int(2));
    Node *n3 = new Node(new int(3));
    Node *n4 = new Node(new int(4));
    TEST_ASSERT_EQUAL(0, l.count());
    l.append(n1);
    TEST_ASSERT_EQUAL(1, l.count());
    l.append(n2);
    TEST_ASSERT_EQUAL(2, l.count());
    l.append(n3);
    TEST_ASSERT_EQUAL(3, l.count());
    l.append(n4);
    TEST_ASSERT_EQUAL(4, l.count());

    // Confirm original head and tail
    TEST_ASSERT_EQUAL_PTR(n1, l.head());
    TEST_ASSERT_EQUAL_PTR(n4, l.tail());
    TEST_ASSERT_TRUE(IS_HEAD(l.head()));
    TEST_ASSERT_TRUE(IS_TAIL(l.tail()));
    TEST_ASSERT_TRUE(IS_HEAD(n1));

    TEST_ASSERT_EQUAL_PTR(n1, l.trimHead());
    TEST_ASSERT_EQUAL(3, l.count());
    TEST_ASSERT_EQUAL_PTR(n2, l.head());
    TEST_ASSERT_EQUAL_PTR(n4, l.tail());
    TEST_ASSERT_TRUE(IS_HEAD(l.head()));
    TEST_ASSERT_TRUE(IS_TAIL(l.tail()));

    TEST_ASSERT_EQUAL_PTR(n2, l.trimHead());
    TEST_ASSERT_EQUAL(2, l.count());
    TEST_ASSERT_EQUAL_PTR(n3, l.head());
    TEST_ASSERT_EQUAL_PTR(n4, l.tail());
    TEST_ASSERT_TRUE(IS_HEAD(l.head()));
    TEST_ASSERT_TRUE(IS_TAIL(l.tail()));

    TEST_ASSERT_EQUAL_PTR(n3, l.trimHead());
    TEST_ASSERT_EQUAL(1, l.count());
    TEST_ASSERT_EQUAL_PTR(n4, l.head());
    TEST_ASSERT_EQUAL_PTR(n4, l.tail());
    TEST_ASSERT_TRUE(IS_HEAD(l.head()));
    TEST_ASSERT_TRUE(IS_TAIL(l.tail()));

    TEST_ASSERT_EQUAL_PTR(n4, l.trimHead());
    TEST_ASSERT_EQUAL(0, l.count());
    TEST_ASSERT_TRUE(l.empty());
}

void test_list_trim_tail(){
    List l;
    Node *n1 = new Node(new int(1));
    Node *n2 = new Node(new int(2));
    Node *n3 = new Node(new int(3));
    Node *n4 = new Node(new int(4));
    TEST_ASSERT_EQUAL(0, l.count());
    l.append(n1);
    TEST_ASSERT_EQUAL(1, l.count());
    l.append(n2);
    TEST_ASSERT_EQUAL(2, l.count());
    l.append(n3);
    TEST_ASSERT_EQUAL(3, l.count());
    l.append(n4);
    TEST_ASSERT_EQUAL(4, l.count());

    // Confirm original head and tail
    TEST_ASSERT_EQUAL_PTR(n1, l.head());
    TEST_ASSERT_EQUAL_PTR(n4, l.tail());
    TEST_ASSERT_TRUE(IS_HEAD(l.head()));
    TEST_ASSERT_TRUE(IS_TAIL(l.tail()));
    TEST_ASSERT_TRUE(IS_HEAD(n1));

    TEST_ASSERT_EQUAL_PTR(n4, l.trimTail());
    TEST_ASSERT_EQUAL(3, l.count());
    TEST_ASSERT_EQUAL_PTR(n1, l.head());
    TEST_ASSERT_EQUAL_PTR(n3, l.tail());
    TEST_ASSERT_TRUE(IS_HEAD(l.head()));
    TEST_ASSERT_TRUE(IS_TAIL(l.tail()));

    TEST_ASSERT_EQUAL_PTR(n3, l.trimTail());
    TEST_ASSERT_EQUAL(2, l.count());
    TEST_ASSERT_EQUAL_PTR(n1, l.head());
    TEST_ASSERT_EQUAL_PTR(n2, l.tail());
    TEST_ASSERT_TRUE(IS_HEAD(l.head()));
    TEST_ASSERT_TRUE(IS_TAIL(l.tail()));

    TEST_ASSERT_EQUAL_PTR(n2, l.trimTail());
    TEST_ASSERT_EQUAL(1, l.count());
    TEST_ASSERT_EQUAL_PTR(n1, l.head());
    TEST_ASSERT_EQUAL_PTR(n1, l.tail());
    TEST_ASSERT_TRUE(IS_HEAD(l.head()));
    TEST_ASSERT_TRUE(IS_TAIL(l.tail()));

    TEST_ASSERT_EQUAL_PTR(n1, l.trimTail());
    TEST_ASSERT_EQUAL(0, l.count());
    TEST_ASSERT_TRUE(l.empty());
}

void test_list_remove(){
    List l;
    Node *n1 = new Node(new int(1));
    Node *n2 = new Node(new int(2));
    Node *n3 = new Node(new int(3));
    Node *n4 = new Node(new int(4));
    TEST_ASSERT_EQUAL(0, l.count());
    l.append(n1);
    TEST_ASSERT_EQUAL(1, l.count());
    l.append(n2);
    TEST_ASSERT_EQUAL(2, l.count());
    l.append(n3);
    TEST_ASSERT_EQUAL(3, l.count());
    l.append(n4);
    TEST_ASSERT_EQUAL(4, l.count());

    // Confirm original head and tail
    TEST_ASSERT_EQUAL_PTR(n1, l.head());
    TEST_ASSERT_EQUAL_PTR(n4, l.tail());
    // Remove a middle node
    TEST_ASSERT_EQUAL_PTR(n2, n2->remove());
    TEST_ASSERT_EQUAL(3, l.count());
    TEST_ASSERT_NULL(n2->next);
    TEST_ASSERT_NULL(n2->prev);
    TEST_ASSERT_EQUAL_PTR(n1, l.head());
    TEST_ASSERT_EQUAL_PTR(n4, l.tail());
    TEST_ASSERT_TRUE(IS_HEAD(l.head()));
    TEST_ASSERT_TRUE(IS_TAIL(l.tail()));

    // Remove a head node
    TEST_ASSERT_EQUAL_PTR(n1, n1->remove());
    TEST_ASSERT_EQUAL(2, l.count());
    TEST_ASSERT_NULL(n1->next);
    TEST_ASSERT_NULL(n1->prev);
    TEST_ASSERT_EQUAL_PTR(n3, l.head());
    TEST_ASSERT_EQUAL_PTR(n4, l.tail());
    TEST_ASSERT_TRUE(IS_HEAD(l.head()));
    TEST_ASSERT_TRUE(IS_TAIL(l.tail()));

    // Remove a tail node
    TEST_ASSERT_EQUAL_PTR(n4, n4->remove());
    TEST_ASSERT_EQUAL(1, l.count());
    TEST_ASSERT_NULL(n4->next);
    TEST_ASSERT_NULL(n4->prev);
    TEST_ASSERT_EQUAL_PTR(n3, l.head());
    TEST_ASSERT_EQUAL_PTR(n3, l.tail());
    TEST_ASSERT_TRUE(IS_HEAD(l.head()));
    TEST_ASSERT_TRUE(IS_TAIL(l.tail()));

    // Remove the last node
    TEST_ASSERT_EQUAL_PTR(n3, n3->remove());
    TEST_ASSERT_EQUAL(0, l.count());
    TEST_ASSERT_NULL(n3->next);
    TEST_ASSERT_NULL(n3->prev);
    TEST_ASSERT_TRUE(l.empty());
    TEST_ASSERT_NULL(l.head());
    TEST_ASSERT_NULL(l.tail());
    TEST_ASSERT_NOT_NULL(l.self());
}

void test_list_steal(){
    List l1;
    List l2;
    Node *n1 = new Node(new int(1));
    Node *n2 = new Node(new int(2));
    Node *n3 = new Node(new int(3));
    Node *n4 = new Node(new int(4));
    TEST_ASSERT_EQUAL(0, l1.count());
    l1.append(n1);
    TEST_ASSERT_EQUAL(1, l1.count());
    l1.append(n2);
    TEST_ASSERT_EQUAL(2, l1.count());
    l1.append(n3);
    TEST_ASSERT_EQUAL(3, l1.count());
    l1.append(n4);
    TEST_ASSERT_EQUAL(4, l1.count());

    // Steal a middle node
    TEST_ASSERT_EQUAL(0, l2.count());
    l2.steal(n2);
    TEST_ASSERT_EQUAL(3, l1.count());
    TEST_ASSERT_EQUAL(1, l2.count());
    TEST_ASSERT_EQUAL_PTR(n1, l1.head());
    TEST_ASSERT_EQUAL_PTR(n4, l1.tail());
    TEST_ASSERT_EQUAL_PTR(n2, l2.head());
    TEST_ASSERT_EQUAL_PTR(n2, l2.tail());

    // Steal a head node
    l2.steal(n1);
    TEST_ASSERT_EQUAL(2, l1.count());
    TEST_ASSERT_EQUAL(2, l2.count());
    TEST_ASSERT_EQUAL_PTR(n3, l1.head());
    TEST_ASSERT_EQUAL_PTR(n4, l1.tail());
    TEST_ASSERT_EQUAL_PTR(n2, l2.head());
    TEST_ASSERT_EQUAL_PTR(n1, l2.tail());

    // Steal a tail node
    l2.steal(n4);
    TEST_ASSERT_EQUAL(1, l1.count());
    TEST_ASSERT_EQUAL(3, l2.count());
    TEST_ASSERT_EQUAL_PTR(n3, l1.head());
    TEST_ASSERT_EQUAL_PTR(n3, l1.tail());
    TEST_ASSERT_EQUAL_PTR(n2, l2.head());
    TEST_ASSERT_EQUAL_PTR(n4, l2.tail());

    // Steal a the last node
    l2.steal(n3);
    TEST_ASSERT_EQUAL(0, l1.count());
    TEST_ASSERT_EQUAL(4, l2.count());
    TEST_ASSERT_TRUE(l1.empty());
    TEST_ASSERT_NULL(l1.tail());
    TEST_ASSERT_EQUAL_PTR(n2, l2.head());
    TEST_ASSERT_EQUAL_PTR(n3, l2.tail());
}

void process() {
    UNITY_BEGIN();
    RUN_TEST(test_node_default_constructor);
    RUN_TEST(test_node_constructor);
    RUN_TEST(test_node_constructor2);
    RUN_TEST(test_list_constructor);
    RUN_TEST(test_list_append_node);
    RUN_TEST(test_list_append_value);
    RUN_TEST(test_list_macro);
    RUN_TEST(test_list_trim_head);
    RUN_TEST(test_list_trim_tail);
    RUN_TEST(test_list_remove);
    RUN_TEST(test_list_steal);
    UNITY_END();
}

#ifdef ARDUINO

#include <Arduino.h>
void setup() {
    process();
}

void loop() {
    digitalWrite(13, HIGH);
    delay(100);
    digitalWrite(13, LOW);
    delay(500);
}

#else

int main(int argc, char **argv) {
    process();
    return 0;
}

#endif
#endif /* UNIT_TEST */
