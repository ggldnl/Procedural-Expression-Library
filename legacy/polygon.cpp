#include "polygon.h"
#include <math.h>

#undef max
#define max(a,b) ((a)>(b)?(a):(b))

#undef min
#define min(a,b) ((a)<(b)?(a):(b))


Polygon::Polygon(): num_points(0) {}


Polygon::Polygon(const float pointsArray[][2], uint8_t n) : num_points(min(n, MAX_POINTS)) {
  for (uint8_t i = 0; i < num_points; ++i) {
    points[i][0] = pointsArray[i][0];
    points[i][1] = pointsArray[i][1];
  }
  updateBoundingBox();
}


void Polygon::updateBoundingBox() {

  min_x = points[0][0], max_x = points[0][0];
  min_y = points[0][1], max_y = points[0][1];

  for (uint8_t i = 0; i < num_points; ++i) {
    min_x = min(min_x, points[i][0]);
    max_x = max(max_x, points[i][0]);
    min_y = min(min_y, points[i][1]);
    max_y = max(max_y, points[i][1]);
  }

  center_x = (min_x + max_x) / 2;
  center_y = (min_y + max_y) / 2;
  width = max_x - min_x;
  height = max_y - min_y;
}


void Polygon::add(float x, float y) {
  if (num_points < MAX_POINTS - 1) {
    points[num_points][0] = x;
    points[num_points][1] = y;
    ++num_points;
    updateBoundingBox();
  }
}


const float* Polygon::get(uint8_t index) const {
    // Check if the index is within the valid range
    if (index >= 0 && index < num_points)
      return points[index];
    return nullptr; // Return nullptr if the index is out of range
}


const float* Polygon::remove(uint8_t index) {
  float* ptr = nullptr;
  if (index >= 0 && index < num_points) {
    ptr = points[index];
    for (uint8_t i = index; i < num_points - 1; ++i) {
      points[i][0] = points[i + 1][0];
      points[i][1] = points[i + 1][1];
    }
    --num_points;
    updateBoundingBox();
  }
  return ptr;
}


uint8_t Polygon::length() const {
  return num_points;
}

void Polygon::inplace_scale(float x_scale, float y_scale) {
    for (uint8_t i = 0; i < num_points; ++i) {
        points[i][0] = points[i][0] * x_scale;
        points[i][1] = points[i][1] * y_scale;
    }
    
    updateBoundingBox();
}

void Polygon::scale(float x_scale, float y_scale) {
    for (uint8_t i = 0; i < num_points; ++i) {
        points[i][0] -= center_x;
        points[i][1] -= center_y;
    }

    for (uint8_t i = 0; i < num_points; ++i) {
        points[i][0] = points[i][0] * x_scale;
        points[i][1] = points[i][1] * y_scale;
    }

    for (uint8_t i = 0; i < num_points; ++i) {
        points[i][0] += center_x;
        points[i][1] += center_y;
    }

    updateBoundingBox();
}


void Polygon::scale_to(float w, float h) {
    scale(w / width, h / height);
}


void Polygon::shift(float x_offset, float y_offset) {
    for (uint8_t i = 0; i < num_points; ++i) {
        points[i][0] += x_offset;
        points[i][1] += y_offset;
    }
    updateBoundingBox();
}


void Polygon::shift_to(float x, float y) {
  shift(x - center_x, y - center_y);
}


bool Polygon::contains(float x, float y) const {

  bool inside = false;

  // float x_test = x + center.x;
  // float y_test = y + center.y;

  for (uint8_t i = 0, j = num_points - 1; i < num_points; j = i++) { // for each point

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

    // Approximation
    
    // Check if the point lies on a horizontal segment
    if ((yi == y && yj == y) && 
      (x >= min(xi, xj) && x <= max(xi, xj))) {
      return true;
    }

    // Check if the point lies on a vertical segment
    if ((xi == xj && xi == x) && 
      (y >= min(yi, yj) && y <= max(yi, yj))) {
      return true;
    }

    // Check if the point falls on the right side of the segment
    if ((yi > y) != (yj > y) && x < (xj - xi) * (y - yi) / (yj - yi) + xi) {
        inside = !inside;
    }
  }

  return inside;
}

void Polygon::interpolate_to(const Polygon& polygon, float factor) {
  for (uint8_t i = 0; i < num_points; ++i) {
    points[i][0] += (polygon.points[i][0] - points[i][0]) * factor;
    points[i][1] += (polygon.points[i][1] - points[i][1]) * factor;
  }
  updateBoundingBox();
}

void Polygon::hmirror(void) {
  for (uint8_t i = 0; i < num_points; ++i) {
    points[i][1] = 2 * center_y - points[i][1];
  }
}

void Polygon::vmirror(void) {
  for (uint8_t i = 0; i < num_points; ++i) {
    points[i][0] = 2 * center_x - points[i][0];
  }
}
