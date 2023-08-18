#include "gui.h"


GUI::GUI(): 

  /*
   * Display driver
   */
  u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE),

  /*
   * Eye look
   */
  left_eye_distance_from_center_x(gui_defaults::LEFT_EYE_DISTANCE_FROM_CENTER),
  right_eye_distance_from_center_x(gui_defaults::RIGHT_EYE_DISTANCE_FROM_CENTER),
  min_bbox_height(gui_defaults::MIN_BBOX_HEIGHT),
  max_bbox_height(gui_defaults::MAX_BBOX_HEIGHT),
  min_bbox_width (gui_defaults::MIN_BBOX_WIDTH),
  max_bbox_width (gui_defaults::MAX_BBOX_WIDTH),
  vertical_perspective_enabled(gui_defaults::VERTICAL_PERSPECTIVE_ENABLED),
  horizontal_perspective_enabled (gui_defaults::HORIZONTAL_PERSPECTIVE_ENABLED),

  /* 
   * Eye movement control
   */
  min_pixel_distance_threshold(gui_defaults::MIN_PIXEL_DISTANCE_THRESHOLD),

  /* 
   * Animation control
   */ 
  interpolation_total_steps(gui_defaults::INTERPOLATION_ANIMATION_DEFAULT_STEPS),
  blinking_total_steps(gui_defaults::BLINKING_ANIMATION_DEFAULT_STEPS)
  {

  u8g2.begin();

  width = u8g2.getWidth();
  height = u8g2.getHeight();

  // Center of the screen
  center_x = width / 2;
  center_y = height / 2;

  // Actual point the robot is looking at
  current_x = center_x;
  current_y = center_y;

  // Point the robot should be looking; default is center of the screen
  target_x = center_x;
  target_y = center_y;

  // Left and right eye current coordinates
  left_current_x = center_x - eye_bbox_width / 2 - left_eye_distance_from_center_x;
  left_current_y = center_y;

  right_current_x = center_x + eye_bbox_width / 2 + right_eye_distance_from_center_x;
  right_current_y = center_y;

  // Left and right eye target coordinates
  left_target_x = left_current_x;
  left_target_y = left_current_y;

  right_target_x = right_current_x;
  right_target_y = right_current_y;

  // Eye size
  eye_bbox_width = max_bbox_width;
  eye_bbox_height = max_bbox_height;
}

void GUI::init() {

  // Clear the buffer
  u8g2.clearBuffer();
  u8g2.sendBuffer();

  // Init left and right polygon
  
  copy(expr_normal_points, current_left_polygon, num_points);
  copy(expr_normal_points, current_right_polygon, num_points);

  // Init target left and right polygon

  target_left_polygon = expr_normal_points;
  target_right_polygon = expr_normal_points;
  mirror_left = false;

  // Interpolation among current and target polygons
  interpolation_current_step = 0.0;
  interpolation_factor = 0.0;

  interpolation_enabled = true; // set this to false to disable changing expression
  interpolation_occurring = false;

  // Blinking animation
  blinking_current_step = 0.0;
  blinking_factor = 0.0;
  
  blinking_enabled = true; // set this to false to disable blinking
  blinking_occurring = false;
  unblinking_occurring = false;

}

void GUI::update() {

  // Clear the display
  u8g2.clearBuffer();

  for (uint16_t y = 0; y < eye_bbox_height + 1; ++y)
    for (uint16_t x = 0; x < eye_bbox_width + 1; ++x)
      
      // Normalize the points
      if (contains(current_left_polygon, num_points, (x + 0.0) / eye_bbox_width, (y + 0.0) / eye_bbox_height))
        u8g2.drawPixel(
          left_current_x - eye_bbox_width / 2 + x,
          left_current_y + eye_bbox_height / 2 - y
        );

  for (uint16_t y = 0; y < eye_bbox_height + 1; ++y)
    for (uint16_t x = 0; x < eye_bbox_width + 1; ++x)
      if (contains(current_right_polygon, num_points, (x + 0.0) / eye_bbox_width, (y + 0.0) / eye_bbox_height))
        if (mirror_left)
          u8g2.drawPixel(
            right_current_x + eye_bbox_width / 2 - x,
            right_current_y + eye_bbox_height / 2 - y
          );
        else
          u8g2.drawPixel(
            right_current_x - eye_bbox_width / 2 + x,
            right_current_y + eye_bbox_height / 2 - y
          );

  // Update the display
  u8g2.sendBuffer();
}


void GUI::step() {

  /*
   * Blinking
   */

  if (blinking_enabled && blinking_occurring) {

    blinking_factor = blinking_current_step / blinking_total_steps;
    interpolate_to(current_left_polygon, target_left_polygon, num_points, blinking_factor);
    interpolate_to(current_right_polygon, target_right_polygon, num_points, blinking_factor);

    if (blinking_current_step == blinking_total_steps) {
  
      // We are done with the first part of the blinking animation; reset the counter
      blinking_current_step = 0.0;

      blinking_occurring = false;
      unblinking_occurring = true;
      
      // Set a new target polygon
      unblink();
    } else {
      ++blinking_current_step;
    }
  }

  if (blinking_enabled && unblinking_occurring) {

    blinking_factor = blinking_current_step / blinking_total_steps;
    interpolate_to(current_left_polygon, target_left_polygon, num_points, blinking_factor);
    interpolate_to(current_right_polygon, target_right_polygon, num_points, blinking_factor);

    if (blinking_current_step == blinking_total_steps) {
      
      // Done with the second part of the animation
      blinking_current_step = 0.0;
      
      // We can enable again the interpolation to change expression
      unblinking_occurring = false;
      interpolation_enabled = true;
    } else {
      ++blinking_current_step;
    }
  }

  /*
   * Interpolate the current polygons to the target polygons (shape of the eyes)
   */

  if (interpolation_enabled && interpolation_occurring) {

    interpolation_factor = interpolation_current_step / interpolation_total_steps;
    interpolate_to(current_left_polygon, target_left_polygon, num_points, interpolation_factor);
    interpolate_to(current_right_polygon, target_right_polygon, num_points, interpolation_factor);

    if (interpolation_current_step == interpolation_total_steps) {
      interpolation_current_step = 0.0;
      interpolation_occurring = false;
    } else {
      ++interpolation_current_step;
    }
  }

  // Interpolate the current point to reach the target point (movement of the eyes).
  // I sincerely hope the compiler will do its magic.

  uint16_t distance_x = target_x - current_x;
  uint16_t distance_y = target_y - current_y;
  float distance = sqrt(distance_x * distance_x + distance_y * distance_y);
  float increment = distance / 10;

  // Check if movement is needed
  if (distance > min_pixel_distance_threshold) {

    // Compute direction vector

    float direction_x = (target_x - current_x) / distance;
    float direction_y = (target_y - current_y) / distance; 
    //uint8_t direction_x = target_x < current_x ? -1 : 1;
    //uint8_t direction_y = target_y < current_y ? -1 : 1; 

    // Update current position
    current_x += direction_x * increment;
    current_y += direction_y * increment;

  } else {
    
    current_x = target_x;
    current_y = target_y;

  }

  // Update left and right eye coordinates based on current point
  left_current_x = current_x - eye_bbox_width / 2 - left_eye_distance_from_center_x;
  left_current_y = current_y;
  right_current_x = current_x + eye_bbox_width / 2 + right_eye_distance_from_center_x;
  right_current_y = current_y;

  // Vertical perspective
  if (vertical_perspective_enabled) {

    float y_factor = (abs(distance_y) * 2.0) / height;  // y_distance / (height / 2) 
    eye_bbox_height = max_bbox_height - y_factor * (max_bbox_height - min_bbox_height); 

  }

  // Horizontal perspective
  if (horizontal_perspective_enabled) {
  
    float x_factor = (abs(distance_x) * 2.0) / width;
    eye_bbox_width = max_bbox_width - x_factor * (max_bbox_width - min_bbox_width); 
  }

  // When looking left or right, the eye get closer
  // TODO

  // Finally, update the screen
  update();
}

/*
 * Eye movement control
 */

void GUI::look(const float x, const float y) {
  target_x = x;
  target_y = y;
}

bool GUI::is_looking_at(const float x, const float y) {
  return current_x == target_x && current_y == target_y;
}

/*
 * Eye perspective control
 */
void GUI::enable_height_perspective() {
  vertical_perspective_enabled = true;
}

void GUI::disable_height_perspective() {
  vertical_perspective_enabled = false;
}

void GUI::set_height_perspective(bool perspective_enabled) {
  vertical_perspective_enabled = perspective_enabled;
}

void GUI::enable_width_perspective() {
  horizontal_perspective_enabled = true;
}

void GUI::disable_width_perspective() {
  horizontal_perspective_enabled = false;
}

void GUI::set_width_perspective(bool perspective_enabled) {
  horizontal_perspective_enabled = perspective_enabled;
}

/*
 * Blinking and unblinking steps for the blink animation 
 */

void GUI::blink(void) {

  target_left_polygon = expr_blinking_points;
  target_right_polygon = expr_blinking_points; 
  mirror_left = false; 

  blinking_occurring = true;
  interpolation_enabled = false;
}

// Private method, reverse of the blink animation
void GUI::unblink(void) {

  target_left_polygon = expr_normal_points;
  target_right_polygon = expr_normal_points;
}

/*
 * Facial expressions
 */

void GUI::angry(void) {

  target_left_polygon = expr_angry_points;
  target_right_polygon = expr_angry_points;
  interpolation_occurring = true;
  mirror_left = true;
}

void GUI::normal(void) {

  target_left_polygon = expr_normal_points;
  target_right_polygon = expr_normal_points;
  interpolation_occurring = true;
  mirror_left = false;
}

void GUI::confused(void) {

  // we will use epxr_normal_points
  // target_left_polygon = expr_curious_points;
  // target_right_polygon = expr_curious_points;

  // Randomly decide to squish the left or right eye
  uint8_t probability = random(0, 100); // [0, 99]
  if (probability < 50) {
    target_left_polygon = expr_normal_points;
    target_right_polygon = expr_confused_points;
  } else {
    target_left_polygon = expr_confused_points;
    target_right_polygon = expr_normal_points;
  }

  interpolation_occurring = true;
  mirror_left = false;
}

void GUI::worried(void) {

  target_left_polygon = expr_worried_points;
  target_right_polygon = expr_worried_points;
  interpolation_occurring = true;
  mirror_left = true;
}

void GUI::happy(void) {

  target_left_polygon = expr_happy_points;
  target_right_polygon = expr_happy_points;
  interpolation_occurring = true;
  mirror_left = false;
}

