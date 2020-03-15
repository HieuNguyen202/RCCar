#if defined(UNIT_TEST) && defined(ARDUINO)

#include <light.h>
#include <unity.h>
#include <stddef.h>
#ifdef ARDUINO

// This should be user defined cluster
enum ClusterName {
    LEFTSIGNAL,
    RIGHTSIGNAL,
    HEAD,
    TAIL,
    LEFTSENSOR,
    RIGHTSENSOR,
    NUM_CLUSTERS
};

void test_light_constructor() {

    TEST_ASSERT_EQUAL(6, NUM_CLUSTERS);

    Light l1(-1, 0);
    TEST_ASSERT_EQUAL(0, l1.ledCount());
    TEST_ASSERT_EQUAL(0, l1.clusterCount());
    TEST_ASSERT_NULL(l1.nodes());
    TEST_ASSERT_NULL(l1.clusters());

    // typical list
    Light l3(10, NUM_CLUSTERS);
    TEST_ASSERT_EQUAL(10, l3.ledCount());
    TEST_ASSERT_EQUAL(NUM_CLUSTERS, l3.clusterCount());

    // Default nodes
    Node *nodes = l3.nodes();
    for (int i = 0; i < l3.ledCount(); i++)
    {
        TEST_ASSERT_EQUAL(i, nodes[i].ivalue);
        TEST_ASSERT_NULL(nodes[i].next);
        TEST_ASSERT_NULL(nodes[i].prev);
        TEST_ASSERT_NULL(nodes[i].value);
    }

    // Default clusters
    Cluster *clusters = l3.clusters();  
    TEST_ASSERT_EQUAL_PTR(clusters, l3.cluster(0));
    for (int i = 0; i < l3.clusterCount(); i++)
    {
        TEST_ASSERT_EQUAL(OFF, l3.cluster(i)->effect->mode);
        TEST_ASSERT_EQUAL(0,   l3.cluster(i)->effect->interval);
        TEST_ASSERT_EQUAL(0,   l3.cluster(i)->effect->t0);
        TEST_ASSERT_TRUE(l3.cluster(i)->leds.empty());
    }
    TEST_ASSERT_EQUAL_PTR(l3.cluster(0), l3.cluster(LEFTSIGNAL));
    TEST_ASSERT_EQUAL_PTR(l3.cluster(1), l3.cluster(RIGHTSIGNAL));
    TEST_ASSERT_EQUAL_PTR(l3.cluster(2), l3.cluster(HEAD));
    TEST_ASSERT_EQUAL_PTR(l3.cluster(3), l3.cluster(TAIL));
    TEST_ASSERT_EQUAL_PTR(l3.cluster(4), l3.cluster(LEFTSENSOR));
    TEST_ASSERT_EQUAL_PTR(l3.cluster(5), l3.cluster(RIGHTSENSOR));
}

void test_light_bind() {
    Light l(12, NUM_CLUSTERS);

    /* Bind 0 and 1 to  LEFTSIGNAL*/
    l.bind(0, LEFTSIGNAL);
    l.bind(1, LEFTSIGNAL);

    TEST_ASSERT_FALSE(l.cluster(LEFTSIGNAL)->leds.empty()); /* 0 and 1 */
    TEST_ASSERT_TRUE(l.cluster(RIGHTSIGNAL)->leds.empty()); /* Nothing */
    TEST_ASSERT_TRUE(l.cluster(HEAD)       ->leds.empty()); /* Nothing */
    TEST_ASSERT_TRUE(l.cluster(TAIL)       ->leds.empty()); /* Nothing */
    TEST_ASSERT_TRUE(l.cluster(LEFTSENSOR) ->leds.empty()); /* Nothing */
    TEST_ASSERT_TRUE(l.cluster(RIGHTSENSOR)->leds.empty()); /* Nothing */

    TEST_ASSERT_EQUAL_PTR(l.node(0), l.cluster(LEFTSIGNAL)->leds.head()); /* led 0 */
    TEST_ASSERT_EQUAL_PTR(l.node(1), l.cluster(LEFTSIGNAL)->leds.tail()); /* led 1 */

    /* Bind the rest*/
    l.bind(0, LEFTSIGNAL);
    l.bind(1, LEFTSIGNAL);
    l.bind(2, RIGHTSIGNAL);
    l.bind(3, RIGHTSIGNAL);
    l.bind(4, HEAD);
    l.bind(5, HEAD);
    l.bind(6, TAIL);
    l.bind(7, TAIL);
    l.bind(8, LEFTSENSOR);
    l.bind(9, LEFTSENSOR);
    l.bind(10, RIGHTSENSOR);
    l.bind(11, RIGHTSENSOR);

    TEST_ASSERT_FALSE(l.cluster(LEFTSIGNAL) ->leds.empty());
    TEST_ASSERT_FALSE(l.cluster(RIGHTSIGNAL)->leds.empty());
    TEST_ASSERT_FALSE(l.cluster(HEAD)       ->leds.empty());
    TEST_ASSERT_FALSE(l.cluster(TAIL)       ->leds.empty());
    TEST_ASSERT_FALSE(l.cluster(LEFTSENSOR) ->leds.empty());
    TEST_ASSERT_FALSE(l.cluster(RIGHTSENSOR)->leds.empty());

    TEST_ASSERT_EQUAL_PTR(l.node(0), l.cluster(LEFTSIGNAL)->leds.head());
    TEST_ASSERT_EQUAL_PTR(l.node(1), l.cluster(LEFTSIGNAL)->leds.tail());
    TEST_ASSERT_EQUAL_PTR(l.node(2), l.cluster(RIGHTSIGNAL)->leds.head());
    TEST_ASSERT_EQUAL_PTR(l.node(3), l.cluster(RIGHTSIGNAL)->leds.tail());
    TEST_ASSERT_EQUAL_PTR(l.node(4), l.cluster(HEAD)->leds.head());
    TEST_ASSERT_EQUAL_PTR(l.node(5), l.cluster(HEAD)->leds.tail());
    TEST_ASSERT_EQUAL_PTR(l.node(6), l.cluster(TAIL)->leds.head());
    TEST_ASSERT_EQUAL_PTR(l.node(7), l.cluster(TAIL)->leds.tail());
    TEST_ASSERT_EQUAL_PTR(l.node(8), l.cluster(LEFTSENSOR)->leds.head());
    TEST_ASSERT_EQUAL_PTR(l.node(9), l.cluster(LEFTSENSOR)->leds.tail());
    TEST_ASSERT_EQUAL_PTR(l.node(10), l.cluster(RIGHTSENSOR)->leds.head());
    TEST_ASSERT_EQUAL_PTR(l.node(11), l.cluster(RIGHTSENSOR)->leds.tail());

    /* Re-bind leds to a different cluster */
    l.bind(2, LEFTSIGNAL);
    l.bind(3, LEFTSIGNAL);

    TEST_ASSERT_FALSE(l.cluster(LEFTSIGNAL)->leds.empty()); // Old cluster becomes empty
    TEST_ASSERT_TRUE(l.cluster(RIGHTSIGNAL)->leds.empty()); // New cluster is not

    /* Bind leds to a RIGHTSIGNAL cluster again*/
    l.bind(2, RIGHTSIGNAL);
    l.bind(3, RIGHTSIGNAL);

    TEST_ASSERT_FALSE(l.cluster(LEFTSIGNAL)->leds.empty());  // Contains led 0 and 1
    TEST_ASSERT_FALSE(l.cluster(RIGHTSIGNAL)->leds.empty()); // Contains led 2 and 3
}

void test_light_bind2() {
    Light l(12, NUM_CLUSTERS);

    /* Sigle element range */
    l.bind(0, 0, LEFTSIGNAL);
    TEST_ASSERT_FALSE(l.cluster(LEFTSIGNAL)->leds.empty()); /* 0 and 1 */
    TEST_ASSERT_EQUAL_PTR(l.node(0), l.cluster(LEFTSIGNAL)->leds.head()); /* led 0 */

    /* 2 element inclusive */
    l.bind(1, 2, RIGHTSIGNAL);
    TEST_ASSERT_FALSE(l.cluster(RIGHTSIGNAL)->leds.empty()); /* 1 and 2 */
    TEST_ASSERT_EQUAL_PTR(l.node(1), l.cluster(RIGHTSIGNAL)->leds.head()); /* led 1 */
    TEST_ASSERT_EQUAL_PTR(l.node(2), l.cluster(RIGHTSIGNAL)->leds.tail()); /* led 2 */

    /* Swapped order */
    l.bind(4, 3, HEAD);
    TEST_ASSERT_FALSE(l.cluster(HEAD)->leds.empty()); /* 0 and 1 */
    TEST_ASSERT_EQUAL_PTR(l.node(3), l.cluster(HEAD)->leds.head()); /* led 3 */
    TEST_ASSERT_EQUAL_PTR(l.node(4), l.cluster(HEAD)->leds.tail()); /* led 4 */

    /* Some what long range */
    l.bind(5, 11, TAIL);
    TEST_ASSERT_FALSE(l.cluster(TAIL)->leds.empty()); /* 0 and 1 */
    TEST_ASSERT_EQUAL_PTR(l.node(5), l.cluster(TAIL)->leds.head()); /* led 5 */
    TEST_ASSERT_EQUAL_PTR(l.node(11), l.cluster(TAIL)->leds.tail()); /* led 11 */

    TEST_ASSERT_TRUE(l.cluster(LEFTSENSOR)->leds.empty()); /* 0 and 1 */
    TEST_ASSERT_TRUE(l.cluster(RIGHTSENSOR)->leds.empty()); /* 0 and 1 */

    /* Re-bind */
    l.bind(0, 4, TAIL);
    TEST_ASSERT_FALSE(l.cluster(TAIL)->leds.empty()); /* 0 and 1 */
    TEST_ASSERT_EQUAL_PTR(l.node(5), l.cluster(TAIL)->leds.head()); /* led 5 */
    TEST_ASSERT_EQUAL_PTR(l.node(4), l.cluster(TAIL)->leds.tail()); /* led 4 */
}

void process() {
    UNITY_BEGIN();
    RUN_TEST(test_light_constructor);
    RUN_TEST(test_light_bind);
    RUN_TEST(test_light_bind2);
    UNITY_END();
}

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

#endif /* UNIT_TEST && ARDUINO */