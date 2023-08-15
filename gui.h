#ifndef GUI_H
#define GUI_H

// Include stdint in order to use  uint8_t and some some byte
#include <stdint.h>

// Include the OLED library
#include "SH1106/SH1106.h"

// Include Polygon and Point definitions
#include "polygon.h"

// Include all the expressions Point arrays
#include "expressions.h"

// Declaration for SSD1306 display connected using I2C
#define OLED_RESET 4
#define OLED_ADDRESS 0x3C


class GUI {

  public:

    // Screen size
    uint8_t width, height;

    /**
     * Constructor.
     */
    GUI(void);

    /**
     * Initialize the GUI.
     */
    void init(void);

    /**
     * Step function to be called in an endless loop. A typical usage of the GUI 
     * will be: 
     * 
     * gui = GUI();
     * while True:
     *    gui.step();
     *    if (condition):
     *      change expression
     *
     * The step function will:
     * - interpolate the current point to reach the target point (movement of the eyes)
     * - interpolate the current polygons to the target polygons (shape of the eyes)
     *
     */
    void step(void);

    /**
     * Sets a new target for the robot to look at.
     */
    void look(const float x, const float y);

    /**
     * Tells if the robot is looking at the target or not.
     */
    bool is_looking_at(const float x, const float y);

    void blink(void);

    /*
     * expressions
     */

    void normal(void);
    void confused(void);
    void worried(void);
    void happy(void);
    void angry(void);

    void enable_height_perspective(void);
    void disable_height_perspective(void);
    void set_height_perspective(bool perspective_enabled);

    void enable_width_perspective(void);
    void disable_width_perspective(void);
    void set_width_perspective(bool perspective_enabled);

  private:

    // Display object
    SH1106 display;

    // Center of the screen
    uint8_t center_x, center_y;

    /*
     * Eye perspective: if the robot looks up or down, the eyes
     * appear squished on the height, if the robot looks left or right, 
     * the left and right eye get squished on the width.
     */
    bool height_perspective_enabled, width_perspective_enabled;
    uint8_t min_bbox_height, max_bbox_height;
    uint8_t min_bbox_width, max_bbox_width;

    // Size of the bouding box of the eyes
    uint8_t eye_bbox_width, eye_bbox_height;
    uint8_t eye_distance;

    // Current and target coordinates
    uint8_t current_x, current_y;
    uint8_t target_x, target_y;

    // Left and right eye coordinates
    uint8_t left_current_x, left_current_y; 
    uint8_t right_current_x, right_current_y;
    uint8_t left_target_x, left_target_y; 
    uint8_t right_target_x, right_target_y;

    // Expression
    float current_left_polygon[num_points][2];
    float current_right_polygon[num_points][2];
    const float (*target_left_polygon)[2];
    const float (*target_right_polygon)[2];
    bool mirror_left; // true if the right eye is the mirrored version of the left eye
    
    // If the target position is closer to the current position
    // than the trashold, then current = target
    uint8_t pixel_distance_threshold;

    // Interpolation among current and target polygons
    bool interpolation_enabled;
    uint8_t interpolation_total_steps;
    uint8_t interpolation_current_step;
    bool interpolation_occurring;

    /*
     * Blinking animation: the blinking animation is different 
     * from any other animation since the eye needs to shrink and
     * go back to normal dimension. During the two phases of this 
     * animation we must disable the transition from one expression 
     * to another (using interpolation_enabled)
     */
    uint8_t blinking_total_steps;
    uint8_t blinking_current_step;
    bool blinking_occurring; // true if the robot is blinking
    bool unblinking_occurring; // true if the robot is unblinking

    /**
     * Screen update routine:
     * - clear the screen
     * - draw the polygons
     * - wait
     */
    void update(void);

    /*
     * Second part of the blinking animation.
     */
    void unblink(void);
};

#endif // GUI_H