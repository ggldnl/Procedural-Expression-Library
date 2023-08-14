#ifndef POLYGON_H
#define POLYGON_H

#include <stdint.h>


class Polygon {

  private:

    static const uint8_t MAX_POINTS = 20;  // Maximum number of points
    uint8_t num_points;                    // Current number of points
    float points[MAX_POINTS][2];           // 2xN matrix to store points

    void updateBoundingBox(void);

  public:

    // Center, width and height of the bouding box enclosing the polygon.
    // This is different from the centroid.
    float center_x, center_y;
    float width, height;
    float min_x, min_y;
    float max_x, max_y;

    // Constructors
    Polygon();
    Polygon(const float pointsArray[][2], uint8_t n);

    /**
     * Add a point to the polygon from its coordinates.
     */
    void add(float x, float y);

    /**
     * Get a reference to a point in the polygon.
     */
    const float* get(uint8_t index) const;

    /**
     * Removes the point at the specified index from the 
     * polygon and returns a pointer to it.
     */
    const float* remove(uint8_t index);

    /**
     * Return the number of points in the polygon.
     */
    uint8_t length() const;

    /**
     * Scale the polygon by a scale factor. This transformation
     * does not preserve the center of the bounding box of the 
     * polygon.
     */
    void inplace_scale(float x_scale, float y_scale);

    /**
     * Scale the polygon by multiplying each point inside it by
     * a scale factor (both on x and y). The scaling preserves
     * the center of the bounding box of the polygon.
     */
    void scale(float x_scale, float y_scale);

    /**
     * Shift the polygon by adding an offset (both on x and y) 
     * to each point inside it. 
     */
    void shift(float x_offset, float y_offset);

    /**
     * Scale the polygon such that its bounding box has the 
     * desired width and height.
     */
    void scale_to(float width, float height);

    /**
     * Shift the polygon such that the center of its bounding box
     * falls at the desired coordinates.
     */
    void shift_to(float x, float y);

    /**
     * Check if a given point is inside the polygon.
     */
    bool contains(float x, float y) const;

    /**
     * Interpolate the polygon to another polygon by the specified factor.
     */
    void interpolate_to(const Polygon& polygon, float factor);

    /**
     * Mirror the polygon across vertical axis.
     */
    void hmirror(void);

    /**
     * Mirror the polygon across horizontal axis.
     */
    void vmirror(void);
};

#endif // POLYGON_H