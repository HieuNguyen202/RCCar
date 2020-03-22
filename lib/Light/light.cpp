#include "light.h"

Light::Light(int ledCount, int clusterCount, int pin) {
    init(ledCount, clusterCount, pin);
}

void Light::init(int ledCount, int clusterCount, int pin){
    m_ledCount = ledCount <= 0 ? 0 : ledCount;
    m_clusterCount = clusterCount <= 0 ? 0 : clusterCount;

    // Init nodes
    if (m_ledCount <= 0 || pin < 0) {
        m_nodes = NULL;
        m_pixels = NULL;
    } else {
        // Init nodes
        m_nodes = new Node[m_ledCount];
        for (int i = 0; i < m_ledCount; i++)
        {
            m_nodes[i].ivalue = i; // set LED idx
        }
        // Init pixels
        m_pixels = new Adafruit_NeoPixel(m_ledCount, pin, NEO_GRB + NEO_KHZ800);
        m_pixels->begin(); // INITIALIZE NeoPixel strip object (REQUIRED)
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
    //Guards
    if(led < 0 ||
       m_clusterCount == 0 ||
       m_ledCount == 0 ||
       cluster < 0 ||
       cluster >= m_clusterCount)
        return;
    this->cluster(cluster)->leds.steal(m_nodes + (led % m_ledCount));
}

void Light::bind(int start, int end, int cluster){
    //Guards
    int temp;
    if (start > end) { // swap
        temp = end;
        end = start;
        start = temp;
    }
    if (start < 0 ||
        m_clusterCount == 0 ||
        m_ledCount == 0 ||
        cluster < 0 ||
        cluster >= m_clusterCount)
        return;
    for (int i = start; i <= end; i++)
    {
       bind(i, cluster);
    }
}

void Light::off(int cluster){
    setColor(cluster, 0);
}

void Light::setColor(int cluster, int color){
    if (!m_clusters || !m_pixels || !m_nodes)
        return;
    List *l = &(this->cluster(cluster)->leds);
    if (l->empty())
        return;
    for (Node *n = l->head(); !IS_SELF(n); n = n->next)
    {
        m_pixels->setPixelColor(n->ivalue, color);
    }
    m_pixels->show();
}