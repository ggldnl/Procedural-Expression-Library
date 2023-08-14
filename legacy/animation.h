#ifndef ANIMATION_H
#define ANIMATION_H

#include <functional>

class Animation {
public:

    /**
     * Constructor
     */
    Animation();

    /**
     * Specify what the animation should do 
     * e.g. shaking the eyes
     */
    void set_animation_function(std::function<void()> func);

    void start(int totalSteps);

    bool status(void);

    void stop(void);

private:

    bool isRunning;
    
    uint8_t currentStep;
    
    uint8_t totalTimeSteps;

    std::function<void()> animationFunction;

};

#endif // ANIMATION_H
