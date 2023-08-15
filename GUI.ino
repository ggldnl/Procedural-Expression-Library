#include "gui.h"
#include <math.h>


// GUI object
GUI gui;

// Loop iteration
int iteration = 0;

// Define frames per second.
// Total animation time: animation_frames * FPS
// My hardware can run at 20 FPS
const uint8_t FPS = 30;
const unsigned long frame_duration = 1000 / FPS;

void setup() {

  Serial.begin(9600);
  Serial.println("Initializing GUI");

  gui.init();

  Serial.println("Initialization done");
}

void loop() {

  //Serial.print("Iteration ");
  //Serial.println(iteration);
  ++iteration;

  // Annotate when the loop starts
  unsigned long start = millis();

  uint8_t probability;

  // Randomly decide to blink
  probability = random(0, 100); // [0, 99]
  if (probability < 1) {
    //Serial.println("Blinking");
    gui.blink();
  }

  // Randomly look at a point; the probability for this to happen should be 
  // very low to prevent constant eye movement
  probability = random(0, 100); // [0, 99]
  if (probability < 2) { // 2% probability

    // Randomly choose a distance and an orientation for a new point to look at
    float alpha = random(0, 361) * (PI / 180);

    // Compute the horizontal and vertical offsets from the center to a point 
    // on the box in the given direction
    uint8_t horizontal_offset = (gui.width / 2) * cos(alpha);
    uint8_t vertical_offset = (gui.height / 2) * sin(alpha);

    // Compute the coordinates of the point on the box
    uint8_t box_point_x = (gui.width / 2) + horizontal_offset;
    uint8_t box_point_y = (gui.height / 2) + vertical_offset;

    // Compute the distance between the center and the point
    float distance = sqrt(pow(box_point_x - gui.width / 2, 2) + pow(box_point_y - gui.height / 2, 2));

    // Randomly choose a distance from the center to look at
    float ro = random(distance / 5, distance / 2);

    // Translate from polar to cartesian
    uint8_t x = gui.width / 2 + ro * cos(alpha);
    uint8_t y = gui.height / 2 + ro * sin(alpha);

    // Print the new point
    //Serial.print("Looking at ( ");
    //Serial.print(x);
    //Serial.print(", ");
    //Serial.print(y);
    //Serial.println(")");
  
    // Make the robot look at the point
    gui.look(x, y);

  }

  if (iteration == 100)
    gui.confused();
  
  if (iteration == 200)
    gui.normal();

  gui.step();

  // Loop ends, compute the remaining time
  unsigned long elapsed_time = millis() - start;

  // Maintain the desired frame rate by sleeping for the remaining amount
  // of time
  if (elapsed_time < frame_duration) {
      delay(frame_duration - elapsed_time);
  }
}