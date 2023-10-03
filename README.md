# Desktop Robot Companion

This repository serves as a foundation for developing a social robot with expressive eyes, taking inspiration from Vector and Cozmo. This README provides an overview of the project and the underlying eye expression mechanism. The current version is a proof of concept. It is not intended to be a finished product and has no real usage now besides being cute and keeping you company.

## Introduction

This project aims to create a concept for a social robot. In the future, the expressions that the robot is now limited to could be accompanied by other animations to notify the user of something. Possible use cases are:

- notify users of calendar events, activities, meetings and so on.

- remind users of specific times to do some activity, effectively helping in scheduling tasks.

## Eye Expression Mechanism

The core of this project lies in its eye expression mechanism. Rather than relying on predefined image sequences for the animations, with this approach each animation is procedurally generated. The user can define an 8-point polygon representing the desired eye expression, or a pair of them for asymmetrical expressions. Each point within this polygon has coordinates in range [0,1]. This enables us to scale the expression proportionally to the robot screen's size and translate the polygons to match the point where the robot is currently looking. Transitioning between expressions is achieved through interpolation: each point of the current polygon gradually transitions to match the target polygon. The same mechanism is used to implement the blinking of the eyes: the current polygon is interpolated to a thin rectangle.

## License

This project is licensed under the MIT License, granting you the freedom to use, modify, and distribute it in accordance with the terms specified in the license.
