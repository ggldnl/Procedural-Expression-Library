#include "gui.h"
#include "display_interface/SH1106_adapter.h"


// Create instances of adapters
SH1106Adapter display;
//SSD1306Adapter ssd1306Adapter;

GUI gui(&display);

// Define frames per second.
// Total animation time: animation_frames * FPS
// My hardware can run at 20 FPS
const uint8_t FPS = 30;
const unsigned long frame_duration = 1000 / FPS;

// Timing and loop
unsigned long iteration = 0;
unsigned long start_time;
unsigned long elapsed_time;

// Probabilities
const uint8_t blinking_probability_threshold = 1;   // 1%
const uint8_t looking_probability_threshold = 2;    // 2%
const uint8_t expression_probability_threshold = 2; // 2%
uint8_t outcome;

// How many iterations an expression should last?
const uint8_t expression_duration_in_iterations = 100;
unsigned long last_expression_set_time = 0;
bool neutral_expression = true;

void setup() {

  Serial.begin(9600);
  Serial.println("Initializing GUI");

  gui.init();

  Serial.println("Initialization done");
}

void loop() {

  // Annotate when the loop starts
  start_time = millis();


  // Randomly decide to blink
  outcome = random(0, 100); // [0, 99]
  if (outcome < blinking_probability_threshold)
    gui.blink();


  // Randomly look at a point; the probability for this to happen should be 
  // very low to prevent constant eye movement
  outcome = random(0, 100); // [0, 99]
  if (outcome < looking_probability_threshold) {

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
  
    // Make the robot look at the point
    gui.look(x, y);

  }


  // Randomly decide to change expression if some time (expressed in iterations) has passed
  outcome = random(0, 100);
  if (iteration - last_expression_set_time >= expression_duration_in_iterations && outcome < expression_probability_threshold) {
    if (neutral_expression)
      gui.happy();
    else
      gui.normal();
    last_expression_set_time = iteration;
    neutral_expression = !neutral_expression;
  }

  // Finalize the changes
  gui.step();

  // Loop ends, compute the remaining time and increase current iteration counter
  elapsed_time = millis() - start_time;
  ++iteration;

  // Maintain the desired frame rate by sleeping for the remaining amount
  // of time
  if (elapsed_time < frame_duration) {
      delay(frame_duration - elapsed_time);
  }
}