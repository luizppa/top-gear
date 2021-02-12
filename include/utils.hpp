#ifndef UTILS_H
#define UTILS_H

#include <math.h>

#define PI 3.14159265

namespace top_gear{

    namespace utils {

        extern char* names[];

        float max(float a, float b);
        float min(float a, float b);
        float get_delta(float W, float w, float D, float d);

    }

}

#endif