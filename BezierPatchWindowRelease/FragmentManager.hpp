#ifndef FRAGMENT_MANAGER_HPP
#define FRAGMENT_MANAGER_HPP

#include "RGBAValue.h"
#include <vector>

// Store each pixel we want to draw as a fragment so we can sort by depth later
struct Fragment {
    int x, y;
    float depth;
    RGBAValue color;
};

// Store the fragments as a vector, and store the width and height of the framebuffer for clipping purposes
struct FragmentBuffer {
    int width, height;
    std::vector<Fragment> frags;

    // Add a new fragment to the buffer
    void Add(int x, int y, float z, const RGBAValue& c);

    // Clear the fragment buffer
    void Clear();
};

#endif