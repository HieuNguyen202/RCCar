#ifdef ARDUINO
#include <Arduino.h>
#include <Wire.h>
#include <Ultrasonic.h>
#include <Adafruit_NeoPixel.h>
#include "misc.h"

/* Distance sensors */
Ultrasonic lDist(2, 3);
Ultrasonic rDist(4, 5);

#define MAX_NUMBER_LED_IN_GROUP 5
#define NUMBER_LED_GROUPS 8
struct ArrayList{
    int size;
    int data[MAX_NUMBER_LED_IN_GROUP];
};
typedef struct ArrayList ArrayList;

ArrayList right_leds       =  ArrayList{5, {0, 1, 2, 3, 4}};
ArrayList front_right_leds =  ArrayList{2, {5, 6}};
ArrayList head_leds        =  ArrayList{4, {7, 8, 9, 10}};
ArrayList front_left_leds  =  ArrayList{2, {11, 12}};
ArrayList left_leds        =  ArrayList{5, {13, 14, 15, 16, 17}};
ArrayList rear_left_leds   =  ArrayList{2, {18, 19}};
ArrayList tail_leds        =  ArrayList{4, {20, 21, 22, 23}};
ArrayList rear_right_leds  =  ArrayList{2, {24, 25}};

ArrayList *all_leds[NUMBER_LED_GROUPS] = { &right_leds,
                                           &front_right_leds,
                                           &head_leds,
                                           &front_left_leds,
                                           &left_leds,
                                           &rear_left_leds,
                                           &tail_leds,
                                           &rear_right_leds };
#define NUMPIXELS 26 // Popular NeoPixel ring size
#define PIN        6 // On Trinket or Gemma, suggest changing this to 1
#define DELAYVAL 500 // Time (in milliseconds) to pause between pixels

Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

void setup() {
  Serial.begin(9600);           // set up Serial library at 9600 bps
  Serial.println("Hugh's RC car!");

//   AFMS.begin();  // create with the default frequency 1.6KHz
  pixels.begin(); // INITIALIZE NeoPixel strip object (REQUIRED)

}

void setColor(ArrayList *ledGroup, uint32_t color){
    for (size_t i = 0; i < ledGroup->size; i++)
    {
        pixels.setPixelColor(ledGroup->data[i], color);
    }
}

// Fill strip pixels one after another with a color. Strip is NOT cleared
// first; anything there will be covered pixel by pixel. Pass in color
// (as a single 'packed' 32-bit value, which you can get by calling
// pixels.Color(red, green, blue) as shown in the loop() function above),
// and a delay time (in milliseconds) between pixels.
void colorWipe(uint32_t color, int wait) {
  for(int i=0; i<pixels.numPixels(); i++) { //  For each pixel in pixels...
    pixels.setPixelColor(i, color);         //  Set pixel's color (in RAM)
    pixels.show();                          //  Update strip to match
    delay(wait);                            //  Pause for a moment
  }
}

void whiteOverRainbow(int whiteSpeed, int whiteLength) {

  if(whiteLength >= pixels.numPixels()) whiteLength = pixels.numPixels() - 1;

  int      head          = whiteLength - 1;
  int      tail          = 0;
  int      loops         = 3;
  int      loopNum       = 0;
  uint32_t lastTime      = millis();
  uint32_t firstPixelHue = 0;

  for(;;) { // Repeat forever (or until a 'break' or 'return')
    for(int i=0; i<pixels.numPixels(); i++) {  // For each pixel in pixels...
      if(((i >= tail) && (i <= head)) ||      //  If between head & tail...
         ((tail > head) && ((i >= tail) || (i <= head)))) {
        pixels.setPixelColor(i, pixels.Color(0, 0, 0, 255)); // Set white
      } else {                                             // else set rainbow
        int pixelHue = firstPixelHue + (i * 65536L / pixels.numPixels());
        pixels.setPixelColor(i, pixels.gamma32(pixels.ColorHSV(pixelHue)));
      }
    }

    pixels.show(); // Update strip with new contents
    // There's no delay here, it just runs full-tilt until the timer and
    // counter combination below runs out.

    firstPixelHue += 40; // Advance just a little along the color wheel

    if((millis() - lastTime) > whiteSpeed) { // Time to update head/tail?
      if(++head >= pixels.numPixels()) {      // Advance head, wrap around
        head = 0;
        if(++loopNum >= loops) return;
      }
      if(++tail >= pixels.numPixels()) {      // Advance tail, wrap around
        tail = 0;
      }
      lastTime = millis();                   // Save time of last movement
    }
  }
}

void pulseWhite(uint8_t wait) {
  for(int j=0; j<256; j++) { // Ramp up from 0 to 255
    // Fill entire strip with white at gamma-corrected brightness level 'j':
    pixels.fill(pixels.Color(0, 0, 0, pixels.gamma8(j)));
    pixels.show();
    delay(wait);
  }

  for(int j=255; j>=0; j--) { // Ramp down from 255 to 0
    pixels.fill(pixels.Color(0, 0, 0, pixels.gamma8(j)));
    pixels.show();
    delay(wait);
  }
}

void rainbowFade2White(int wait, int rainbowLoops, int whiteLoops) {
  int fadeVal=0, fadeMax=100;

  // Hue of first pixel runs 'rainbowLoops' complete loops through the color
  // wheel. Color wheel has a range of 65536 but it's OK if we roll over, so
  // just count from 0 to rainbowLoops*65536, using steps of 256 so we
  // advance around the wheel at a decent clip.
  for(uint32_t firstPixelHue = 0; firstPixelHue < rainbowLoops*65536;
    firstPixelHue += 256) {

    for(int i=0; i<pixels.numPixels(); i++) { // For each pixel in pixels...

      // Offset pixel hue by an amount to make one full revolution of the
      // color wheel (range of 65536) along the length of the strip
      // (pixels.numPixels() steps):
      uint32_t pixelHue = firstPixelHue + (i * 65536L / pixels.numPixels());

      // pixels.ColorHSV() can take 1 or 3 arguments: a hue (0 to 65535) or
      // optionally add saturation and value (brightness) (each 0 to 255).
      // Here we're using just the three-argument variant, though the
      // second value (saturation) is a constant 255.
      pixels.setPixelColor(i, pixels.gamma32(pixels.ColorHSV(pixelHue, 255,
        255 * fadeVal / fadeMax)));
    }

    pixels.show();
    delay(wait);

    if(firstPixelHue < 65536) {                              // First loop,
      if(fadeVal < fadeMax) fadeVal++;                       // fade in
    } else if(firstPixelHue >= ((rainbowLoops-1) * 65536)) { // Last loop,
      if(fadeVal > 0) fadeVal--;                             // fade out
    } else {
      fadeVal = fadeMax; // Interim loop, make sure fade is at max
    }
  }

  for(int k=0; k<whiteLoops; k++) {
    for(int j=0; j<256; j++) { // Ramp up 0 to 255
      // Fill entire strip with white at gamma-corrected brightness level 'j':
      pixels.fill(pixels.Color(0, 0, 0, pixels.gamma8(j)));
      pixels.show();
    }
    delay(1000); // Pause 1 second
    for(int j=255; j>=0; j--) { // Ramp down 255 to 0
      pixels.fill(pixels.Color(0, 0, 0, pixels.gamma8(j)));
      pixels.show();
    }
  }

  delay(500); // Pause 1/2 second
}

void loop() {
    int ld, rd;
    ld = lDist.read();
    rd = rDist.read();
    // if        (TOO_CLOSE(ld) && !TOO_CLOSE(rd)){
    //     backupAndTurn(DIR_RIGHT, 150, 250);
    // } else if (!TOO_CLOSE(ld) && TOO_CLOSE(rd)){
    //     backupAndTurn(DIR_LEFT, 150, 250);
    // } else if (TOO_CLOSE(ld) && TOO_CLOSE(rd)){
    //     backupAndTurn(DIR_RIGHT, 150, 500);
    // } else {
    //     veer(150, calculateTurn(ld, rd), calculateDir(ld, rd));
    // }
    // // displayDistance();
    // delay(100);

  // Fill along the length of the strip in various colors...
//   colorWipe(pixels.Color(255,   0,   0)     , 50); // Red
//   colorWipe(pixels.Color(  0, 255,   0)     , 50); // Green
//   colorWipe(pixels.Color(  0,   0, 255)     , 50); // Blue
//   colorWipe(pixels.Color(  0,   0,   0, 255), 50); // True white (not RGB white)

//   whiteOverRainbow(75, 5);

//   pulseWhite(5);

//   rainbowFade2White(3, 3, 1);
    int hue = map(rd, 0, 20 , 0, 11000);
    setColor(&head_leds, pixels.ColorHSV(hue, 255, 20));
    pixels.show();
    delay(100);
}

#endif