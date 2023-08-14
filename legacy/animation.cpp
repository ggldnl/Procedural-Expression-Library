#include "animation.h"
#include <iostream>

Animation::Animation() : isRunning(false), currentStep(0), totalTimeSteps(0), animationFunction(nullptr) {}

void Animation::set_animation_function(std::function<void()> func) {
    animationFunction = func;
}

void Animation::start(int totalSteps) {
    isRunning = true;
    currentStep = 0;
    totalTimeSteps = totalSteps;

    while (isRunning && currentStep < totalTimeSteps) {
        if (animationFunction) {
            animationFunction();  // Execute the specified animation function
        }

        ++currentStep;
    }

    stop();
}

void Animation::stop() {
    isRunning = false;
    currentStep = 0;
    totalTimeSteps = 0;
}

// Example animation functions
void fadeInAnimation() {
    std::cout << "Fading in..." << std::endl;
}

void rotateAnimation() {
    std::cout << "Rotating..." << std::endl;
}
