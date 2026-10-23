#include "FragmentManager.hpp"


void FragmentBuffer::Add(int x, int y, float z, const RGBAValue& c)
{
    frags.push_back({x, y, z, c});
}

void FragmentBuffer::Clear()
{
    frags.clear();
}
