#ifndef FRAGMENT_MANAGER_HPP
#define FRAGMENT_MANAGER_HPP

#include "RGBAValue.h"
#include <vector>

struct Fragment {
    int x, y;
    float depth;
    RGBAValue color;
};


struct FragmentBuffer {
    int width, height;
    std::vector<Fragment> frags;

    void Add(int x, int y, float z, const RGBAValue& c);

    void Clear();
};

#endif