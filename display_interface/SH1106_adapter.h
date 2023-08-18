#include "display_interface.h"

// Include the actual SH1106 library's header
#include "SH1106/SH1106.h"

// Declaration for SSD1306 display connected using I2C
#define OLED_RESET 4
#define OLED_ADDRESS 0x3C

/**
 * Implementation of the DisplayInterface class for the SH1106 controller.
 */
class SH1106Adapter : public DisplayInterface {
public:

    SH1106Adapter() : driver(OLED_ADDRESS, SDA, SCL) {}

    virtual void init() override {
      
      // Initialize the display
      driver.init();

      // Clear the buffer
      driver.clear();

      // Rotate the display
      driver.flipScreenVertically();
    }

    virtual uint16_t getWidth() override {
      return driver.getWidth();
    }

    virtual uint16_t getHeight() override {
      return driver.getHeight();
    }

    virtual void setPixel(int x, int y) override {
      driver.setPixel(x, y); // Call the SH1106 library's method
    }

    virtual void clear() override {
      driver.clear();
    }

    virtual void display() override {
      driver.display();
    }

private:
    SH1106 driver;
};
