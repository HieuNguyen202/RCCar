// #if defined(UNIT_TEST) && defined(ARDUINO)

#include <light.h>
#include <unity.h>
#include <stddef.h>
#include <Arduino.h>
#define PIN 6 // On Trinket or Gemma, suggest changing this to 1

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

void test_timer() {
    /* Test default constructor */
    Timer timer;
    /* Default interval is 1000 miliseconds */
    TEST_ASSERT_EQUAL(1000, timer.interval());
    /* Default start time is milis() at the moment the timer is created */
    TEST_ASSERT_INT_WITHIN(100, millis(), timer.startTime());

    /* Test reset() */
    delay(500);
    timer.reset();
    TEST_ASSERT_INT_WITHIN(100, millis(), timer.startTime());

    /* Test elapse() */
    delay(500);
    TEST_ASSERT_INT_WITHIN(100, 500, timer.elapse());

    /* Test timesUp() */
    timer.setInterval(500);
    TEST_ASSERT_EQUAL(500, timer.interval());
    timer.reset();
    TEST_ASSERT_INT_WITHIN(100, millis(), timer.startTime()); // Check if timer is automatically reset
    delay(100);
    TEST_ASSERT_FALSE(timer.timesUp()); // not yet
    delay(401);
    TEST_ASSERT_TRUE(timer.timesUp()); // it's up
    TEST_ASSERT_INT_WITHIN(100, millis(), timer.startTime()); // Check if timer is automatically reset

}

void test_light_constructor() {

    TEST_ASSERT_EQUAL(6, NUM_CLUSTERS);

    // Bad parameters
    Light l1(-1, 0, -1);
    TEST_ASSERT_EQUAL(0, l1.ledCount());
    TEST_ASSERT_EQUAL(0, l1.clusterCount());
    TEST_ASSERT_NULL(l1.nodes());
    TEST_ASSERT_NULL(l1.clusters());
    TEST_ASSERT_NULL(l1.pixels());
    l1.destroy();
    TEST_ASSERT_NULL(l1.pixels());

    // Typical parameters
    Light l3(10, NUM_CLUSTERS, PIN);
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
    
    // Pixels
    TEST_ASSERT_NOT_NULL(l3.pixels());
    TEST_ASSERT_EQUAL(10, l3.pixels()->numPixels());
    TEST_ASSERT_EQUAL(6,  l3.pixels()->getPin());

    /* Test destructor */
    l3.destroy();
    TEST_ASSERT_NULL(l3.pixels());
    TEST_ASSERT_NULL(l3.clusters());
    TEST_ASSERT_NULL(l3.nodes());
}

void test_light_bind() {
    Light l(12, NUM_CLUSTERS, PIN);

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
    l.destroy();
    TEST_ASSERT_NULL(l.pixels());
}

void test_light_bind2() {
    Light l(12, NUM_CLUSTERS, PIN);

    /* Sigle element range */
    l.bind(0, 0, LEFTSIGNAL);
    TEST_ASSERT_FALSE(l.cluster(LEFTSIGNAL)->leds.empty()); /* 0 */
    TEST_ASSERT_EQUAL_PTR(l.node(0), l.cluster(LEFTSIGNAL)->leds.head()); /* led 0 */
    TEST_ASSERT_EQUAL(1, l.cluster(LEFTSIGNAL)->leds.count());

    /* 2 element inclusive */
    l.bind(1, 2, RIGHTSIGNAL);
    TEST_ASSERT_FALSE(l.cluster(RIGHTSIGNAL)->leds.empty()); /* 1 and 2 */
    TEST_ASSERT_EQUAL_PTR(l.node(1), l.cluster(RIGHTSIGNAL)->leds.head()); /* led 1 */
    TEST_ASSERT_EQUAL_PTR(l.node(2), l.cluster(RIGHTSIGNAL)->leds.tail()); /* led 2 */
    TEST_ASSERT_EQUAL(2, l.cluster(RIGHTSIGNAL)->leds.count());

    /* Swapped order */
    l.bind(4, 3, HEAD);
    TEST_ASSERT_FALSE(l.cluster(HEAD)->leds.empty());
    TEST_ASSERT_EQUAL_PTR(l.node(3), l.cluster(HEAD)->leds.head()); /* led 3 */
    TEST_ASSERT_EQUAL_PTR(l.node(4), l.cluster(HEAD)->leds.tail()); /* led 4 */
    TEST_ASSERT_EQUAL(2, l.cluster(HEAD)->leds.count());

    /* Some what long range */
    l.bind(5, 11, TAIL);
    TEST_ASSERT_FALSE(l.cluster(TAIL)->leds.empty());
    TEST_ASSERT_EQUAL_PTR(l.node(5), l.cluster(TAIL)->leds.head()); /* led 5 */
    TEST_ASSERT_EQUAL_PTR(l.node(11), l.cluster(TAIL)->leds.tail()); /* led 11 */
    TEST_ASSERT_EQUAL(7, l.cluster(TAIL)->leds.count());

    TEST_ASSERT_TRUE(l.cluster(LEFTSENSOR)->leds.empty());
    TEST_ASSERT_EQUAL(0, l.cluster(LEFTSENSOR)->leds.count());

    l.bind(12, 12, RIGHTSENSOR);
    TEST_ASSERT_FALSE(l.cluster(RIGHTSENSOR)->leds.empty());
    TEST_ASSERT_EQUAL(1, l.cluster(RIGHTSENSOR)->leds.count());

    /* Re-bind */
    l.bind(0, 4, TAIL);
    TEST_ASSERT_FALSE(l.cluster(TAIL)->leds.empty()); /* 0 and 1 */
    TEST_ASSERT_EQUAL_PTR(l.node(5), l.cluster(TAIL)->leds.head()); /* led 5 */
    TEST_ASSERT_EQUAL_PTR(l.node(4), l.cluster(TAIL)->leds.tail()); /* led 4 */
    l.destroy();
    TEST_ASSERT_NULL(l.pixels());
}

void _test_light_set_color_helper(Light * l, int cluster, int from, int to) {
    int blue = 100;
    l->bind(from, to, cluster);
    TEST_ASSERT_FALSE(l->cluster(cluster)->leds.empty());
    l->setColor(cluster, blue);
    for (int i = from; i <= to; i++)
    {
        TEST_ASSERT_EQUAL(blue, l->pixels()->getPixelColor(i % l->ledCount()));
    }
}

void _test_light_off_helper(Light * l, int cluster, int from, int to) {
    l->bind(from, to, cluster);
    TEST_ASSERT_FALSE(l->cluster(cluster)->leds.empty());
    l->off(cluster);
    for (int i = from; i <= to; i++)
    {
        TEST_ASSERT_EQUAL(0, l->pixels()->getPixelColor(i % l->ledCount()));
    }
}

void test_light_set_color_helper(Light * l, int cluster, int from, int to) {
    _test_light_set_color_helper(l, cluster, from, to);
}

void test_light_off_helper(Light * l, int cluster, int from, int to) {
    _test_light_off_helper(l, cluster, from, to);
}

void test_light_set_color() {
    Light l(26, NUM_CLUSTERS, PIN);
    test_light_set_color_helper(&l, HEAD, 7, 10);            delay(200);
    test_light_set_color_helper(&l, LEFTSENSOR, 11, 12);     delay(200);
    test_light_set_color_helper(&l, LEFTSIGNAL, 13, 21);     delay(200);
    test_light_set_color_helper(&l, TAIL, 20, 23);           delay(200);
    test_light_set_color_helper(&l, RIGHTSIGNAL, 22, 30);    delay(200);  // reset to 0 if exceeds number of leds
    test_light_set_color_helper(&l, RIGHTSENSOR, 5, 6);      delay(200);

    test_light_off_helper(&l, HEAD, 7, 10);                  delay(200);
    test_light_off_helper(&l, LEFTSENSOR, 11, 12);           delay(200);
    test_light_off_helper(&l, LEFTSIGNAL, 13, 21);           delay(200);
    test_light_off_helper(&l, TAIL, 20, 23);                 delay(200);
    test_light_off_helper(&l, RIGHTSIGNAL, 22, 30);          delay(200); // reset to 0 if exceeds number of leds
    test_light_off_helper(&l, RIGHTSENSOR, 5, 6);            delay(200);
    l.destroy();
    TEST_ASSERT_NULL(l.pixels());
}

void test_effect_blinking() {
    Light l(26, NUM_CLUSTERS, PIN);
    l.bind(7, 10, HEAD);
    l.bind(11, 12, LEFTSENSOR);
    l.bind(13, 19, LEFTSIGNAL);
    l.bind(20, 23, TAIL);
    l.bind(24, 30, RIGHTSIGNAL);
    l.bind(5, 6, RIGHTSENSOR);
    l.cluster(HEAD)->effect->timer.setInterval(50);
    l.cluster(LEFTSENSOR)->effect->timer.setInterval(100);
    l.cluster(LEFTSIGNAL)->effect->timer.setInterval(200);
    l.cluster(TAIL)->effect->timer.setInterval(400);
    l.cluster(RIGHTSIGNAL)->effect->timer.setInterval(800);
    l.cluster(RIGHTSENSOR)->effect->timer.setInterval(1600);
    int count = 1000000 / 100;
    while (count--) {
        l.update();
        delay(10);
    }
    l.off(TAIL);
    l.destroy();
    TEST_ASSERT_NULL(l.pixels());
}

void process() {
    UNITY_BEGIN();
    RUN_TEST(test_timer);
    RUN_TEST(test_light_constructor);
    RUN_TEST(test_light_bind);
    RUN_TEST(test_light_bind2);  // BUG
    RUN_TEST(test_light_set_color);
    RUN_TEST(test_effect_blinking);
    UNITY_END();
}

void setup() {
    process();
}

void loop() {
    digitalWrite(13, HIGH);
    delay(100);
    digitalWrite(13, LOW);
    delay(500);
}

// #endif /* UNIT_TEST && ARDUINO */