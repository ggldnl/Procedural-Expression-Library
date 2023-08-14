#include "polygon.h"

#undef max
#define max(a,b) ((a)>(b)?(a):(b))

#undef min
#define min(a,b) ((a)<(b)?(a):(b))


void scale(float points[][2], uint8_t num_points, float x_scale, float y_scale) {
  for (uint8_t i = 0; i < num_points; ++i) {
    points[i][0] = points[i][0] * x_scale;
    points[i][1] = points[i][1] * y_scale;
  }
}

void shift(float points[][2], uint8_t num_points, float x_offset, float y_offset) {
  for (uint8_t i = 0; i < num_points; ++i) {
    points[i][0] += x_offset;
    points[i][1] += y_offset;
  }
}

bool contains(const float points[][2], uint8_t num_points, float x, float y) {

  bool inside = false;

  for (uint8_t i = 0, j = num_points - 1; i < num_points; j = i++) {
    
    // Take two consecutive points to have a segment
    float xi = points[i][0];
    float yi = points[i][1];
    float xj = points[j][0];
    float yj = points[j][1];

    /*

    // This is too computationally intensive, need to approximate somehow

    // Check if the point lies on the segment
    float ds_start = sqrt((x - xi) * (x - xi) + (y - yi) * (y - yi));
    float ds_end = sqrt((x - xj) * (x - xj) + (y - yj) * (y - yj));
    float ds_len = sqrt((xj - xi) * (xj - xi) + (yj - yi) * (yj - yi));
    
    if (abs(ds_start + ds_end - ds_len) <= 1e-6)
        return true;
    */

    // The following is an pproximation

    // Check if the point lies on a horizontal segment
    if ((yi == y && yj == y) && (x >= min(xi, xj) && x <= max(xi, xj))) {
      return true;
    }

    // Check if the point lies on a vertical segment
    if ((xi == xj && xi == x) && (y >= min(yi, yj) && y <= max(yi, yj))) {
      return true;
    }

    // Check if the point falls on the right side of the segment
    if ((yi > y) != (yj > y) && x < (xj - xi) * (y - yi) / (yj - yi) + xi) {
      inside = !inside;
    }
  }

  return inside;
}

void interpolate_to(float from_points[][2], const float to_points[][2], uint8_t num_points, float factor) {
  for (uint8_t i = 0; i < num_points; ++i) {
    from_points[i][0] += (to_points[i][0] - from_points[i][0]) * factor;
    from_points[i][1] += (to_points[i][1] - from_points[i][1]) * factor;
  }
}

void hmirror(float points[][2], uint8_t num_points, float center_y) {
  for (uint8_t i = 0; i < num_points; ++i) {
    points[i][1] = 2 * center_y - points[i][1];
  }
}

void vmirror(float points[][2], uint8_t num_points, float center_x) {
  for (uint8_t i = 0; i < num_points; ++i) {
    points[i][0] = 2 * center_x - points[i][0];
  }
}

void copy(const float source[][2], float dest[][2], uint8_t num_points) {
    for (uint8_t i = 0; i < num_points; ++i) {
        dest[i][0] = source[i][0];
        dest[i][1] = source[i][1];
    }
}
