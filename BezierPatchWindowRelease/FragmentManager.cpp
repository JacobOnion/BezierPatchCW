#include "FragmentManager.hpp"

void FragmentBuffer::Add(int x, int y, float z, const RGBAValue& c)
{
    if (x < 0 || y < 0 || x >= width || y >= height) return;
        frags.push_back({x, y, z, c});
}