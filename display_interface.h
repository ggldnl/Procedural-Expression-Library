#ifndef DISPLAY_INTERFACE_H
#define DISPLAY_INTERFACE_H


/**
 * Interface that provide a common interface for the oled driver
 * we want to use. Originally i used U8g2. U8g2 is a monochrome 
 * graphics library for embedded devices that lets you work
 * seamlessly with all kinds of oled screens. I switched to this 
 * library since it is lighter and faster. If you want to do the 
 * same but your display has a different controller, redefine this
 * interface. 
 */
class DisplayInterface {

  public:

    /**
     * Initialize the display based on your needs (e.g. after initializing
     * the library that I'm using, i need to vertically flip the screen).
     */
    virtual void init(void) = 0;

    /**
     * Return screen width.
     */
    virtual uint16_t getWidth(void) = 0;

    /**
     * Return screen height.
     */
    virtual uint16_t getHeight(void) = 0;

    /**
     * Set the pixel on the given coordinates.
     */
    virtual void setPixel(int x, int y) = 0;

    /**
     * Clear the screen.
     */
    virtual void clear(void) = 0;

    /**
     * Update the screen.
     */
    virtual void display(void) = 0;
};

#endif
