#ifndef EXPRESSIONS_H
#define EXPRESSIONS_H

#include "polygon.h"

// Fixed-sized array of points for the expressions. The arrays must all have
// the same number of points in order to interpolate between them and pass
// from an expression to another.
// All expressions are array of points with coordinates in [0, 1] range
// that will be scaled to the desired dimension.

/*
const float expr_normal_points [][2] PROGMEM = {
  {0, 0.5},
  {0, 0.8},
  {0.1, 0.95},
  {0.3, 1},
  {0.5, 1},
  {0.7, 1},
  {0.9, 0.95},
  {1, 0.8},
  {1, 0.5},
  {1, 0.2},
  {0.9, 0.05},
  {0.7, 0},
  {0.5, 0},
  {0.3, 0},
  {0.1, 0.05},
  {0, 0.2}
};

const uint8_t num_points = sizeof(expr_normal_points) / sizeof(expr_normal_points[0]);

const float expr_angry_left [][2] PROGMEM = {
  {0, 0.2},
  {0, 0.4},
  {0.1, 0.55},
  {0.3, 0.6},
  {0.5, 0.5},
  {0.7, 0.4},
  {0.9, 0.3},
  {1, 0.25},
  {1, 0.2},
  {1, 0.2},
  {0.9, 0.05},
  {0.7, 0},
  {0.5, 0},
  {0.3, 0},
  {0.1, 0.05},
  {0, 0.2}
};

*/

const float expr_normal_points [][2] PROGMEM = {
  {0, 0},
  {0, 1},
  {0.3, 1},
  {0.7, 1},
  {1, 1},
  {1, 0},
  {0.7, 0},
  {0.3, 0}
};

const uint8_t num_points = sizeof(expr_normal_points) / sizeof(expr_normal_points[0]);

const float expr_blinking_points [][2] PROGMEM = {
  {0, 0.2},
  {0, 0.3},
  {0.3, 0.3},
  {0.7, 0.3},
  {1, 0.3},
  {1, 0.2},
  {0.7, 0.2},
  {0.3, 0.2}
};

const float expr_angry_points [][2] PROGMEM = {
  {0, 0},
  {0, 0.6},
  {0.3, 0.54},
  {0.7, 0.46},
  {1, 0.4},
  {1, 0},
  {0.7, 0},
  {0.3, 0}
};

const float expr_curious_points [][2] PROGMEM = {

};

const float expr_worried_points [][2] PROGMEM = {

};

const float expr_happy_points [][2] PROGMEM = {
  {0, 0.4},
  {0, 0.95},
  {0.05, 1},
  {0.95, 1},
  {1, 0.95},
  {1, 0.4},
  {0.95, 0.55},
  {0.05, 0.55}
};


#endif // EXPRESSIONS_H