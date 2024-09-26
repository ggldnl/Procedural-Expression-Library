# Desktop Robot Companion

The code in this repository serves as a foundation for developing a social robot with expressive eyes, taking inspiration from Vector and Cozmo. This README provides an overview of the project and the underlying eye expression mechanism. The current version is a proof of concept.

<p align="center">
  <img width="480" height="480" src="./media/animation.gif">
</p>

## Introduction

This project aims to create a facial expression library that can be used for social robot. This library currently provides a procedural way to define expressions, transition between them when an event occurs and a way to dynamically adapt the expression to the size of the screen you are using on the robot. 
This code is heavy both computationally and memory-wise. A better approach could perhaps be to [offload the whole expression mechanism to a server](https://github.com/ggldnl/Desktop-Companion-Robot) and send back to the robot the frames it should visualize on the OLED screen.

## Eye Expression Mechanism

Rather than relying on predefined image sequences for the animations, with my approach each animation is procedurally generated. This means that it is simple to add new expressions and the transition between them and the others is handled automatically. The user can define an 8-point polygon representing the desired eye expression, or a pair of them for asymmetrical expressions. Each point within this polygon needs to have coordinates in range [0,1]. This enables scaling expressions proportionally to the robot screen's size and translate the polygons to match the point that the robot is currently looking at. Transitioning between expressions is achieved through interpolation: each point of the current polygon gradually transitions to match the same point on the target polygon. The same mechanism is used to implement the blinking of the eyes: the current polygon is interpolated to a thin rectangle and then brought back to its original form. The polygons are then discretized and only the pixels whose coordinates falls inside the polygons on the screen are lit up.

## Add a new expression

As an example, here it is showed how to add a `confused` expression in which one of the two eyes is more squished than the other.

Add the eye polygon(s) to the `expressions.h` header file. In this case the expression is symmetrical so only one polygon is needed.

```cpp
const float expr_confused_points[][2] PROGMEM = {
  { 0, 0.25 },
  { 0, 0.75 },
  { 0.05, 0.8 },
  { 0.95, 0.8 },
  { 1, 0.75 },
  { 1, 0.25 },
  { 0.95, 0.2 },
  { 0.05, 0.2 }
};
```

Add a method that will start the animation once it is called:

`gui.h`:
```cpp
void confused(void);
```

`gui.cpp`
```cpp
void GUI::confused(void) {

  target_left_polygon = expr_normal_points;
  target_right_polygon = expr_confused_points;
  interpolation_occurring = true;
  mirror_left = false;
}
```

The `interpolation_occurring` flag signals the main loop that an animation is occurring when the method is called, while the `mirror_left` flag is self explainatory. By default if we define only one eye for an expression, this will be the left eye and will then be mirrored to produce the right one.

The underlying logic can of course be more complex:
```cpp
void GUI::confused(void) {

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
```

Then you can simply do something like this:

```cpp
if (condition) {
    gui.confused();
}
```
