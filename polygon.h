#ifndef POLYGON_OPERATIONS_H
#define POLYGON_OPERATIONS_H

#include <stdint.h>

/**
 * Scale the polygon by multiplying each point inside it by a scale factor 
 * (both on x and y).
 */
void scale(float points[][2], uint8_t num_points, float x_scale, float y_scale);

/**
 * Shift the polygon by adding an offset (both on x and y) 
 * to each point inside it. 
 */
void shift(float points[][2], uint8_t num_points, float x_offset, float y_offset);

/**
 * Check if a given point is inside the polygon.
 */
bool contains(const float points[][2], uint8_t num_points, float x, float y);

/**
 * Interpolate the polygon to another polygon by the specified factor.
 */
void interpolate_to(float dest_points[][2], const float src_points[][2], uint8_t num_points, float factor);

/**
 * Mirror the polygon across vertical axis.
 */
void hmirror(float points[][2], uint8_t num_points, float center_y);

/**
 * Mirror the polygon across horizontal axis.
 */
void vmirror(float points[][2], uint8_t num_points, float center_x);

/**
 * Copy the source polygon to #dest
 */
void copy(const float source[][2], float dest[][2], uint8_t num_points);

#endif // POLYGON_OPERATIONS_H
