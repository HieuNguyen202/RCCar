#include "light.h"

Light::Light(int ledCount, int clusterCount) {
    init(ledCount, clusterCount);
}

Light::Light(int ledCount, int clusterCount, int pin) {
    init(ledCount, clusterCount);
    // Init pixels
    if (m_ledCount > 0)
        m_pixels = new Adafruit_NeoPixel(m_ledCount, pin, NEO_GRB + NEO_KHZ800);
}

void Light::init(int ledCount, int clusterCount){
    m_ledCount = ledCount <= 0 ? 0 : ledCount;
    m_clusterCount = clusterCount <= 0 ? 0 : clusterCount;

    // Init nodes
    if (m_ledCount <= 0) {
        m_nodes = NULL;
    } else {
        m_nodes = new Node[m_ledCount];
        for (int i = 0; i < m_ledCount; i++)
        {
            m_nodes[i].ivalue = i; // set LED idx
        }
    }

    // Init clusters
    if (m_clusterCount <= 0) {
        m_clusters = NULL;
    } else {
        m_clusters = new Cluster[m_clusterCount];
        for (int i = 0; i < m_clusterCount; i++)
        {
            m_clusters[i].effect = new Effect(OFF, 0, 0);
        }
    }
}

void Light::bind(int led, int cluster){
    //Guard
    if(led < 0 ||
       led >= m_ledCount ||
       cluster < 0 ||
       cluster >= m_clusterCount)
        return;
    m_clusters[cluster].leds.steal(m_nodes + led);
}

void Light::bind(int start, int end, int cluster){
    //Guard
    int temp;
    if (start < 0 ||
        start >= m_ledCount ||
        end < 0 ||
        end >= m_ledCount ||
        cluster < 0 ||
        cluster >= m_clusterCount)
        return;
    if (start > end) { // swap
        temp = end;
        end = start;
        start = temp;
    }
    for (int i = start; i <= end; i++)
    {
       bind(i, cluster);
    }
}

void Light::off(int cluster){
    Cluster *c = this->cluster(cluster);
    for (Node *n = c->leds.head(); !IS_STANDALONE(n); n = n->next)
    {
        int pixel = n->ivalue;
    }
}