#include "gui.h"
#include <math.h>

GUI::GUI(): display(OLED_ADDRESS, SDA, SCL) {

  width = display.getWidth();
  height = display.getHeight();

  // Center of the screen
  center_x = width / 2;
  center_y = height / 2;

  // Actual point the robot is looking at
  current_x = center_x;
  current_y = center_y;

  // Point the robot should be looking; default is center of the screen
  target_x = center_x;
  target_y = center_y;

  // Left and right eye coordinates
  left_current_x = center_x - eye_bbox_width;
  left_current_y = center_y;

  right_current_x = center_x + eye_bbox_width;
  right_current_y = center_y;

  left_target_x = left_current_x;
  left_target_y = left_current_y;

  right_target_x = right_current_x;
  right_target_y = right_current_y;

  // Eye size
  eye_bbox_width = width / 6;
  eye_bbox_height = height / 2.8;
  eye_distance = eye_bbox_width / 2 + 6;

  // Pixel distance threshold
  pixel_distance_threshold = 2;

}

void GUI::init() {

  // Initialize the display
  display.init();

  // Clear the buffer
  display.clear();

  // Rotate the display
  display.flipScreenVertically();

  // Init left polygon
  
  copy(expr_normal_points, current_left_polygon, num_points);

  // Scale the polygon to match the bounding box size and translate it to the current position
  //current_left_polygon.inplace_scale(eye_bbox_width, eye_bbox_height);

  // Init right polygon

  copy(expr_normal_points, current_right_polygon, num_points);
  //current_right_polygon.inplace_scale(eye_bbox_width, eye_bbox_height);

  // Init target left and right polygon

  target_left_polygon = expr_normal_points;
  // target_left_polygon.inplace_scale(eye_bbox_width, eye_bbox_height);

  target_right_polygon = expr_normal_points;
  // target_right_polygon.inplace_scale(eye_bbox_width, eye_bbox_height);

  // Interpolation among current and target polygons
  interpolation_enabled = true; // set this to false to disable changing expression
  interpolation_total_steps = 4;
  interpolation_current_step = 0;
  interpolation_occurring = false;

  // Blinking animation
  blinking_total_steps = 2;
  blinking_current_step = 0;
  blinking_occurring = false;
  unblinking_occurring = false;

  /*
  // Draw the left polygon
  for (int8_t i = 0; i < eye_bbox_height + 1; ++i) {
    
    Serial.print(i < 10 ? "i =  " : "i = ");
    Serial.print(i);
    Serial.print(" -> ");

    for (int8_t j = 0; j < eye_bbox_width + 1; ++j) {
      if (current_left_polygon.contains(j, i))
        Serial.print("■");
      else
        Serial.print("□");
    }
    Serial.println();
  }
  */

  /*
  // Print all the left polygon points
  for (uint8_t i = 0; i < current_left_polygon.length(); ++i) {
    
    const float* point = current_left_polygon.get(i);
    Serial.print("( ");
    Serial.print(point[0]);
    Serial.print(", ");
    Serial.print(point[1]);
    Serial.println(")");
  }
  Serial.println();
  */

}

void GUI::update() {

  // Clear the display
  display.clear();

  /*
  // Print the eye
  for (int8_t i = 0; i < eye_bbox_height + 1; ++i) {
    
    Serial.print(i < 10 ? "i =  " : "i = ");
    Serial.print(i);
    Serial.print(" -> ");

    for (int8_t j = 0; j < eye_bbox_width + 1; ++j) {
      if (contains(current_left_polygon, num_points, (j + 0.0) / eye_bbox_width, (i + 0.0) / eye_bbox_height))
        Serial.print("■");
      else
        Serial.print("□");
    }
    Serial.println();
  }
  */

  for (uint y = 0; y < eye_bbox_height + 1; ++y)
    for (int x = 0; x < eye_bbox_width + 1; ++x)
      
      // Normalize the points
      if (contains(current_left_polygon, num_points, (x + 0.0) / eye_bbox_width, (y + 0.0) / eye_bbox_height))
        display.setPixel(
          left_current_x - eye_bbox_width / 2 + x,
          left_current_y + eye_bbox_height / 2 - y
        );

  for (int y = 0; y < eye_bbox_height + 1; ++y)
    for (int x = 0; x < eye_bbox_width + 1; ++x)
      if (contains(current_right_polygon, num_points, (x + 0.0) / eye_bbox_width, (y + 0.0) / eye_bbox_height))
        display.setPixel(
          right_current_x - eye_bbox_width / 2 + x,
          right_current_y + eye_bbox_height / 2 - y
        );

  /*
  display.fillRect(
    left_current_x - eye_bbox_width / 2, 
    left_current_y - eye_bbox_height / 2, 
    eye_bbox_width, 
    eye_bbox_height  
  );

  display.fillRect(
    right_current_x - eye_bbox_width / 2, 
    right_current_y - eye_bbox_height / 2, 
    eye_bbox_width, 
    eye_bbox_height  
  );
  */

  // Display the drawings
  display.display();
}


void GUI::step() {

  // Blink

  if (blinking_occurring) {

    float blinking_factor = (blinking_current_step + 0.0) / blinking_total_steps;
    interpolate_to(current_left_polygon, target_left_polygon, num_points, blinking_factor);
    interpolate_to(current_right_polygon, target_right_polygon, num_points, blinking_factor);

    if (blinking_current_step == blinking_total_steps) {
  
      // We are done with the first part of the blinking animation; reset the counter
      blinking_current_step = 0;

      blinking_occurring = false;
      unblinking_occurring = true;
      
      // Set a new target polygon
      unblink();
    } else {
      ++blinking_current_step;
    }
  }

  if (unblinking_occurring) {

    float blinking_factor = (blinking_current_step + 0.0) / blinking_total_steps;
    interpolate_to(current_left_polygon, target_left_polygon, num_points, blinking_factor);
    interpolate_to(current_right_polygon, target_right_polygon, num_points, blinking_factor);

    if (blinking_current_step == blinking_total_steps) {
      
      // Done with the second part of the animation
      blinking_current_step = 0;
      
      // We can enable again the interpolation to change expression
      unblinking_occurring = false;
      interpolation_enabled = true;
    } else {
      ++blinking_current_step;
    }
  }

  // Interpolate the current polygons to the target polygons (shape of the eyes)
  
  if (interpolation_enabled && interpolation_occurring) {

    float interpolation_factor = (interpolation_current_step + 0.0) / interpolation_total_steps;
    interpolate_to(current_left_polygon, target_left_polygon, num_points, interpolation_factor);
    interpolate_to(current_right_polygon, target_right_polygon, num_points, interpolation_factor);

    if (interpolation_current_step == interpolation_total_steps) {
      interpolation_current_step = 0;
      interpolation_occurring = false;
    } else {
      ++interpolation_current_step;
    }
  }

  // Interpolate the current point to reach the target point (movement of the eyes)

  float distance = sqrt(pow((current_x - target_x), 2) + pow((current_y - target_y), 2));
  float increment = distance / 5;

  // Check if movement is needed
  if (distance > pixel_distance_threshold) {

    // Compute direction vector
    float direction_x = (target_x - current_x) / distance;
    float direction_y = (target_y - current_y) / distance; 

    // Update current position
    current_x += direction_x * increment;
    current_y += direction_y * increment;

  } else {
    
    current_x = target_x;
    current_y = target_y;

  }

  // Update left and right eye coordinates based on current point
  left_current_x = current_x - eye_distance;
  left_current_y = current_y;
  right_current_x = current_x + eye_distance;
  right_current_y = current_y;

  // Finally, update the screen
  update();
}

void GUI::look(const float x, const float y) {
  target_x = x;
  target_y = y;
}

bool GUI::is_looking_at(const float x, const float y) {
  return current_x == target_x && current_y == target_y;
}

void GUI::blink(void) {

  target_left_polygon = expr_blinking_points;
  target_right_polygon = expr_blinking_points;  

  blinking_occurring = true;
  interpolation_enabled = false;
}

// Private method, reverse of the blink animation
void GUI::unblink(void) {

  target_left_polygon = expr_normal_points;
  target_right_polygon = expr_normal_points;
}

void GUI::angry(void) {

  target_left_polygon = expr_angry_points;
  target_right_polygon = expr_angry_points;
  interpolation_occurring = true;
}

void GUI::normal(void) {

  target_left_polygon = expr_normal_points;
  target_right_polygon = expr_normal_points;
  interpolation_occurring = true;
}

void GUI::curious(void) {

  target_left_polygon = expr_curious_points;
  target_right_polygon = expr_curious_points;
  interpolation_occurring = true;
}

void GUI::worried(void) {

  target_left_polygon = expr_worried_points;
  target_right_polygon = expr_worried_points;
  interpolation_occurring = true;
}

void GUI::happy(void) {

  target_left_polygon = expr_happy_points;
  target_right_polygon = expr_happy_points;
  interpolation_occurring = true;
}

