#ifndef LIGHT_H
#define LIGHT_H
#include <stddef.h>
#include "list.h"
#include <Adafruit_NeoPixel.h>
#include <Arduino.h>

struct LED;
typedef struct LED LED;
class Light;

enum LightMode {
    FLASHING,
    SOLID,
    OFF
};

/* Timer. Default interval is 1000 milisecond. */
class Timer {
    private:
        long m_start;
        long m_interval; /* miliseconds */

    public:
        /* Constructor */
        Timer();
        /* Set interval in miliseconds */
        void setInterval(long interval);
        /* Reset timer */
        void reset();
        /* Return time elapse in miliseconds */
        long elapse();
        /* Return false if time is up, else reset timer then return true */
        bool timesUp();
        long interval() { return m_interval; };
        long startTime() { return m_start; };

};

typedef struct Effect {
    LightMode mode;     // Mode
    long interval;      // Update interval
    long t0;            // Start time, used to determine when to update
    Timer timer;
    bool state;
    int color = 0;
    Effect(){
        mode = OFF;
        interval = 0;
        t0 = 0;
    };
    Effect(LightMode mode){
        this->mode = mode;
        interval = 0;
        t0 = 0;
    };
    Effect(LightMode mode, long interval){
        this->mode = mode;
        this->interval = interval;
        t0 = 0;
    };
    Effect(LightMode mode, long interval, long t0){
        this->mode = mode;
        this->interval = interval;
        this->t0 = t0;
    };

    void update(List *leds, Adafruit_NeoPixel* pixels) {
        if (!leds || leds->empty() || !pixels || !timer.timesUp()) {
            return;
        } else {
            // turn light the opposite direction
            for (Node *n = leds->head(); !IS_SELF(n); n = n->next)
            {
                pixels->setPixelColor(n->ivalue, state == HIGH ? 0 : color++);
            }
            // Save current state
            state = !state;
            pixels->show();
        }
    }
} Effect;

typedef struct Cluster {
    Effect *effect;
    List leds;
    Cluster() {
        effect = NULL;
        List led();
    }

    void update(Adafruit_NeoPixel* pixels) {
        if(!effect)
            return;
        effect->update(&leds, pixels);
    }

} Cluster;

class Light {
    public:
        /* Constructor */
        Light(int ledCount, int clusterCount, int pin);

        /* Destructor */
        ~Light();
        void destroy();

        /* Controls */
        /* Turn all lights off */
        void off();
        /* Turn a given light array off */
        void off(int cluster);
        /* Give a given light array a color*/
        void setColor(int cluster, int color);
        /* Set light effect of a  given array */
        void setEffect(int cluster, Effect effect);
        /* Set left turn signal effect */
        void signalLeft();
        /* Set right turn signal effect */
        void signalRight();
        /* Turn the tail light (red) on */
        void breakOn();
        /* Turn the tail light (red) on at a given brightness*/
        void breakOn(int brightness);
        /* Turn the tail light off */
        void breakOff();
        /* Turn the head light on */
        void headOn();
        /* Turn the head light on at a given brightnesss */
        void headOn(int brightness);
        /* Turn the head light off */
        void headOff();

        /* Setup */
        /* Bind a `led` to a cluster, remove its connection with the previous Cluster
         * `led` and `cluster` are zero indexed` */
        void bind(int led, int cluster);
        /* Bind LEDs from `start` to `end` (both inclusive) to a cluster*/
        void bind(int start, int end, int cluster);

        /* Update the state of each cluster */
        void update();

        /* Getters */
        Adafruit_NeoPixel *pixels() { return m_pixels; };
        Node *nodes()               { return m_nodes; };
        Cluster *clusters()         { return m_clusters; };
        Node *node(int idx)         { return (idx >= 0) ? (m_nodes + (idx % m_ledCount)) : NULL; };
        Cluster *cluster(int idx)   { return (idx >= 0) ? (m_clusters + (idx % m_clusterCount)) : NULL; };
        int ledCount()              { return m_ledCount; };
        int clusterCount()          { return m_clusterCount; };

    private:
        void init(int ledCount, int clusterCount, int pin);

        /* Make and return a light LightArray struct  */
        int m_ledCount; // led count
        int m_clusterCount; // cluster count
        Node *m_nodes;
        Cluster *m_clusters;
        Adafruit_NeoPixel *m_pixels;

};
#endif