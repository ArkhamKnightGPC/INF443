#include "pyramid.hpp"
#include <math.h>       /* sin, cos */

# define M_PI           3.14159265358979323846  /* pi */

using namespace cgp;
mesh create_pyramid_mesh(float radius, float height, float z_offset)
{
    mesh m;

    // Base of the pyramid
    m.position.push_back({radius, -radius, 0});
    m.position.push_back({radius, radius, 0});
    m.position.push_back({-radius, radius, 0});
    m.position.push_back({-radius, -radius, 0});
    // apex
    m.position.push_back({0,0,height+z_offset});

    // connectivity
    m.connectivity.push_back({0, 1, 4});
    m.connectivity.push_back({1, 2, 4});
    m.connectivity.push_back({2, 3, 4});
    m.connectivity.push_back({3, 0, 4});

    m.fill_empty_field();
    return m;
}