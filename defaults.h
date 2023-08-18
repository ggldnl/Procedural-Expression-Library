#ifndef DEFAULTS_H
#define DEFAULTS_H

// Include stdint in order to use  uint8_t and some some byte
#include <stdint.h>


namespace gui_defaults {
  
  /**
   * We draw the left and right eye starting from the current 
   * positoon (current_x, current_y). We subtract half the 
   * width of the bounding box to obtain the center of the left eye
   * and add half the width to obtain the center of the right eye. 
   * We subtract LEFT_EYE_OFFSET to further shift 
   * the left eye by the specified number of pixels on the left.
   * Same thing for the right eye (RIGHT_EYE_OFFSET).
   */
  constexpr uint8_t LEFT_EYE_OFFSET = 4;
  
  /**
   * We draw the left and right eye starting from the current 
   * positoon (current_x, current_y). We subtract half the 
   * width of the bounding box to obtain the center of the left eye
   * and add half the width to obtain the center of the right eye. 
   * We subtract LEFT_EYE_OFFSET to further shift 
   * the left eye by the specified number of pixels on the left.
   * Same thing for the right eye (RIGHT_EYE_OFFSET).
   */
  constexpr uint8_t RIGHT_EYE_OFFSET = 4;

  /**
   * Minimum bounding box height.
   */
  constexpr uint16_t MIN_BBOX_HEIGHT = 20;

  /**
   * Maximum bounding box height.
   */
  constexpr uint16_t MAX_BBOX_HEIGHT = 30;

  /**
   * Minimum bounding box width.
   */
  constexpr uint16_t MIN_BBOX_WIDTH = 15;

  /**
   * Maximum bounding box width.
   */
  constexpr uint16_t MAX_BBOX_WIDTH = 25;

  /**
   * When looking up and down, the eye bounding box height get
   * squished.
   */
  constexpr bool VERTICAL_PERSPECTIVE_ENABLED = false;
  
  /**
   * When looking left and right, the eye bouding box width get
   * squished.
   */
  constexpr bool HORIZONTAL_PERSPECTIVE_ENABLED = false;

  /**
   * When moving from the current position to the target one, 
   * in order to mimic an organic movement the distance is first
   * computed and then a fraction of it is used as increment in
   * the direction of the movement. When the eye almost reaches
   * the target position and its distance is less than this 
   * threshold, the current position is set to the target one
   * without further computations. 
   */
  constexpr uint8_t MIN_PIXEL_DISTANCE_THRESHOLD = 2;

  /**
   * Number of frames for the interpolation animation.
   */
  constexpr uint8_t INTERPOLATION_DEFAULT_STEPS = 4;

  /**
   * Number of frames for the blinking animation.
   */
  constexpr uint8_t BLINKING_DEFAULT_STEPS = 2;

  /**
   * Interpolation control: true if the robot can change expression.
   */
  constexpr bool INTERPOLATION_ENABLED = true;

  /**
   * Blinking control: true if the robot can blink.
   */
  constexpr bool BLINKING_ENABLED = true;

}

#endif